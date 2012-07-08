/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QFile>

#include <KDebug>
#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KLocale>
#include <simonutils/fileutils.h>


#include "modelcompilersphinx.h"
#include <QDir>

/* Plan:
 * locate sphinxtrain
 * change using lm to jsgf
 * copy audio to proper wav directory or change in config?
 * cd model directory and using sphinxtrain-snapshot pre-configure model
 * modify proper config files and run training
 * log
 */


bool ModelCompilerSPHINX::parseConfiguration()
{
  KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
  KConfigGroup programGroup(&config, "Programs");

  //   if ((compilationType & ModelCompilerHTK::CompileSpeechModel)||
  //   (compilationType & ModelCompilerHTK::AdaptSpeechModel)) {
  m_SphinxTrain = programGroup.readEntry("sphinxtrain", KUrl(KStandardDirs::findExe("sphinxtrain"))).toLocalFile();

  //   }

  if (!QFile::exists(m_SphinxTrain))
  {
    //SphinxTrain not found
    QString errorMsg = i18n("The SphinxTrain cannot be found. Please make sure it is installed correctly.");
    emit error(errorMsg);
    return false;
  }


  return true;
}

bool ModelCompilerSPHINX::startCompilation(ModelCompiler::CompilationType compilationType, const QString &modelDestination, 
                                           const QString &baseModelPath, const QHash<QString, QString> &args)
{
  if(args.isEmpty())
  {
    emit error(i18n("There no arguments to compile"));
    return false;
  }

  this->compilationType = compilationType;
  m_ModelDir = baseModelPath;
  m_ModelName = args.value("modelName");
  m_WavPath = args.value("audioPath");
  
  kDebug() << modelDestination << baseModelPath << args;
  kDebug() << "Compiling model";
  
  keepGoing = true;
  
  clearLog();
  
  emit  status(i18n("Compiling model..."), 0, 100);

  kDebug() << "Parsing configuration";

  if (!parseConfiguration())
  {
    return false;
  }

  emit  status(i18n("Compiling model..."), 5, 100);
  kDebug() << "Setting up model";

  if(!setupModel(m_ModelDir, m_ModelName))
  {
    //
    emit error(i18n("Cannot setup model"));
    return false;
  }

  //kDebug() << "";

  emit  status(i18n("Compiling model..."), 15, 100);

  //fix config..
  QHash<QString, QString> params;
  params.insert("CFG_WAVFILES_DIR", m_WavPath);
  params.insert("CFG_WAVFILE_EXTENSION", "wav");
  params.insert("CFG_WAVFILE_TYPE", "mswav");


  QString configPath = m_ModelDir+"/"+m_ModelName+"/etc/sphinx_train.cfg";
  if(!modifyConfig(configPath, params))
  {
    emit error(i18n("Cannot modify config at \"%1\"",configPath));
    return false;
  }

  emit  status(i18n("Compiling model..."), 20, 100);
  kDebug() << "Starting actually compilation";

  if(!compileModel(m_ModelDir, m_ModelName))
  {
    //
    emit error(i18n("Cannot compile model"));
    return false;
  }

  emit  status(i18n("Compiling model..."), 95, 100);
  kDebug() << "Сopying model to destination";

  if(!pack(modelDestination, m_ModelName))
  {
    emit error(i18n("Cannot copy model to destination"));
    return false;
  }

  emit  status(i18n("Compiling model..."), 100, 100);
  kDebug() << "Compilation complete";

  return true;
}

bool ModelCompilerSPHINX::processError()
{
  //TODO: implement
  return true;
}

bool ModelCompilerSPHINX::pack(const QString &targetArchive, const QString &name)
{

  QHash<QString, QByteArray> fm;
  fm.insert("metadata.xml", getMetaData(name, "SPHINX").toUtf8());

  QHash<QString, QString> efm;

  QString srcDirName = m_ModelDir+QLatin1String("/")+m_ModelName+QLatin1String("/model_parameters/")+
                       m_ModelName+QLatin1String(".ci_cont/");
  QDir sourceDir(srcDirName);

  if(!sourceDir.exists())
  {
    emit error(i18n("Failed to pack to archive. Source directory does not exist( \"%1\")", srcDirName));
    return false;
  }

  foreach (QString tFileName, sourceDir.entryList())
  {
    efm.insert(srcDirName+tFileName, tFileName);
  }

  QString fetc = m_ModelDir+QLatin1String("/")+m_ModelName+QLatin1String("/etc/");
  efm.insert(fetc+QLatin1String(".jsgf"), m_ModelName+QLatin1String(".jsgf"));
  efm.insert(fetc+QLatin1String(".dic"), m_ModelName+QLatin1String(".dic"));

  return FileUtils::pack(targetArchive, fm, efm);
}

bool ModelCompilerSPHINX::setupModel(const QString &modelDir, const QString &modelName)
{
  QString execString = m_SphinxTrain +" -t " +modelName+" setup";
  if(execute(execString, modelDir+"/"+modelName))
    return true;
  else
  {
    processError();
    return false;
  }
}

bool ModelCompilerSPHINX::compileModel(const QString &modelDir, const QString &modelName)
{
  QString execString = m_SphinxTrain +" run";
  if(execute(execString, modelDir+"/"+modelName))
    return true;
  else
  {
    processError();
    return false;
  }
}

bool ModelCompilerSPHINX::modifyConfig(const QString &filename, const QHash<QString, QString> &args)
{
  QFile configFile(filename);
  if (!configFile.open(QIODevice::ReadOnly))
  {
    emit error(i18n("Failed to read config at  \"%1\"", filename));
    return false;
  }

  QRegExp pLine;
  pLine.setPattern("^( {0,}\\$[0-9a-zA-Z_]+ =)");

  QByteArray outArr;
  QTextStream out(&outArr);

  QTextStream in(&configFile);
  while (!in.atEnd())
  {
    QString line = in.readLine();
    if(pLine.indexIn(line) == -1)
    {
      out << line <<"\n";
      //        kDebug()<<line;
    }
    else
    {
      //        kDebug()<< "wheee";
      QStringList capturedList = pLine.capturedTexts();
      QString key = capturedList.first().mid(1, capturedList.first().size() - 3); //3 chars: $, ,=
      if(!args.contains(key))
      {
        out << line <<"\n";
        //          kDebug()<<line;
        continue;
      }

      out << capturedList.first() +" \""+args.value(key)+"\";" <<"\n";
      kDebug()<<capturedList.first() +" \""+args.value(key)+"\";";
    }

  }

  configFile.close();

  out.flush();
  if (!configFile.open(QIODevice::WriteOnly))
  {
    emit error(i18n("Failed to read config at  \"%1\"", filename));
    return false;
  }

  configFile.write(outArr);
  configFile.close();

  //  QString content = QString(configFile.read);

  return true;
}

//bool ModelCompilerSPHINX::copyDirsContent(const QString &source, const QString &destination)
//{
//  QDir destDir(destination);
//  QDir sourceDir(source);

//  if(!sourceDir.exists())
//  {
//    emit error(i18n("Failed to copy. Source directory does not exist( \"%1\")", source));
//    return false;
//  }

//  if (!destDir.exists())
//  {
//    kDebug()<<destDir.canonicalPath();
//    if (!destDir.mkpath(destination))
//    {
//      emit error(i18n("Failed to copy files to destination. Can't create destination directory( \"%1\")", destination));
//      return false;
//    }
//  }
//  foreach (QString tFileName, sourceDir.entryList())
//  {
//    if(!QFile::copy(sourceDir.canonicalPath() + tFileName, destination+tFileName))
//    {
//      emit error(i18n("Failed to copy file ( \"%1\")", tFileName));
//      return false;
//    }
//  }

//  return true;
//}

//bool ModelCompilerSPHINX::copyModelToDestination(const QString &destination)
//{
//  return copyDirsContent(m_ModelDir+"/"+m_ModelName+"/model_parameters/"+m_ModelName+".ci_cont/", destination);
//}

QString ModelCompilerSPHINX::information(bool condensed) const
{
  //TODO:implement
  return "";
}


ModelCompilerSPHINX::ModelCompilerSPHINX(const QString &userName, QObject *parent):ModelCompiler(userName, parent)
{

}
