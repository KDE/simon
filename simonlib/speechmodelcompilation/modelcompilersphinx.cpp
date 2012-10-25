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

#include "modelcompilersphinx.h"

#include <simonutils/fileutils.h>

#include <QFile>
#include <KDebug>
#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KLocale>
#include <QDir>

bool ModelCompilerSPHINX::parseConfiguration()
{
  KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
  KConfigGroup programGroup(&config, "Programs");

  if(compilationType & ModelCompiler::CompileSpeechModel)
  {
    //   (compilationType & ModelCompilerHTK::AdaptSpeechModel)) {
    m_SphinxTrain = programGroup.readEntry("sphinxtrain", KUrl(KStandardDirs::findExe("sphinxtrain"))).toLocalFile();

    if (!QFile::exists(m_SphinxTrain))
    {
      //SphinxTrain not found
      QString errorMsg = i18n("The SphinxTrain cannot be found. Please make sure it is installed correctly.");
      emit error(errorMsg);
      return false;
    }
  }

  if(compilationType & ModelCompiler::AdaptSpeechModel)
  {
    m_Bw = programGroup.readEntry("bw", KUrl(KStandardDirs::findExe("bw"))).toLocalFile();
    m_Sphinx_fe = programGroup.readEntry("sphinx_fe", KUrl(KStandardDirs::findExe("sphinx_fe"))).toLocalFile();
    m_Pocketsphinx_mdef_convert = programGroup.readEntry("pocketsphinx_mdef_convert", KUrl(KStandardDirs::findExe("pocketsphinx_mdef_convert"))).toLocalFile();
    m_Map_adapt = programGroup.readEntry("map_adapt", KUrl(KStandardDirs::findExe("map_adapt"))).toLocalFile();
    m_Mllr_solve = programGroup.readEntry("mllr_solve", KUrl(KStandardDirs::findExe("mllr_solve"))).toLocalFile();

    if (!QFile::exists(m_Bw) || !QFile::exists(m_Sphinx_fe) || !QFile::exists(m_Pocketsphinx_mdef_convert) ||
        !QFile::exists(m_Map_adapt) || !QFile::exists(m_Mllr_solve))
    {
      //adapting tools not found
      QString errorMsg = i18n("Tools for adaption of the model cannot be found. Please make sure they are installed correctly.");
      emit error(errorMsg);
      return false;
    }
  }

  return true;
}

bool ModelCompilerSPHINX::startCompilation(ModelCompiler::CompilationType compilationType, const QString &modelDestination, 
                                           const QStringList& droppedTranscriptions, const QString &baseModelPath,
                                           const QHash<QString, QString> &args)
{
  if(args.isEmpty())
  {
    emit error(i18n("There no arguments to compile"));
    return false;
  }

  this->compilationType = compilationType;
  m_ModelDir = args.value("modelDir");
  m_BaseModelPath = baseModelPath;
  m_ModelName = args.value("modelName");
  m_WavPath = args.value("audioPath");
  m_ModelDestination = modelDestination;
  
  m_droppedTranscriptions = droppedTranscriptions;

  m_ConfigPath = m_ModelDir+"/"+m_ModelName+"/etc/sphinx_train.cfg";
  
  kDebug() << modelDestination << baseModelPath << args;
  kDebug() << "Compiling model";
  
  keepGoing = true;
  
  clearLog();
  
  switch (compilationType)
  {
    case ModelCompiler::CompileLanguageModel:
    {
      return CompileLanguageModel();
    }
    case ModelCompiler::AdaptSpeechModel: //WARNING: is it OK?
    {
      return AdaptBaseModel();
    }
    default:
    {
      return CompileWholeModel();
    }
  }

  return true;
}

bool ModelCompilerSPHINX::CompileLanguageModel()
{
  emit  status(i18n("Packing new language model to existing static acoustic model..."), 0, 100);
  if(!pack(m_ModelDestination, m_ModelName))
  {
    emit error(i18n("Cannot copy model to destination"));
    return false;
  }
  emit  status(i18n("Packing new language model to existing static acoustic model..."), 100, 100);
  return true;
}

bool ModelCompilerSPHINX::AdaptBaseModel()
{

  emit  status(i18n("Adapting model..."), 0, 100);

  kDebug() << "Parsing configuration";

  if (!parseConfiguration())
  {
    return false;
  }

  emit  status(i18n("Adapting model..."), 15, 100);
  kDebug() << "Copying model from base model directory to working directory";

  if(!copyModelForAdapting(m_BaseModelPath, m_ModelDir+"/"+m_ModelName))
  {
    return false;
  }

  emit  status(i18n("Adapting model..."), 30, 100);
  kDebug() << "Generating acoustic feature files (sphinx_fe)";

  if(!generateAcousticFeatureFiles())
  {
    emit error(i18n("Cannot generate acoustic feature files"));
    return false;
  }

  emit  status(i18n("Adapting model..."), 45, 100);
  kDebug() << "Converting mdef (pocketsphinx_mdef_convert)";

  if(!convertMdef())
  {
     emit error(i18n("Cannot convert mdef"));
    return false;
  }

  emit  status(i18n("Adapting model..."), 60, 100);
  kDebug() << "Collecting statistics from the adaptation data (bw)";

  if(!getStatistics())
  {
     emit error(i18n("Cannot collect statistics from the adaptation data"));
    return false;
  }

  emit  status(i18n("Adapting model..."), 75, 100);
  kDebug() << "Performing adaption with map method";

  if(!mapUpdate())
  {
     emit error(i18n("Cannot performing adaption with map method"));
    return false;
  }

  emit  status(i18n("Adapting model..."), 95, 100);

  kDebug() << "Сopying model to destination";
  if(!pack(m_ModelDestination, m_ModelName))
  {
    emit error(i18n("Cannot copy model to destination"));
    return false;
  }

  emit  status(i18n("Adapting model..."), 100, 100);

  kDebug() << "Adaption complete";

  return true;
}

bool ModelCompilerSPHINX::CompileWholeModel()
{
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

  emit  status(i18n("Compiling model..."), 15, 100);

  //fix config..
  QHash<QString, QString> params;
  params.insert("CFG_WAVFILES_DIR", m_WavPath);
  params.insert("CFG_WAVFILE_EXTENSION", "wav");
  params.insert("CFG_WAVFILE_TYPE", "mswav");
  params.insert("CFG_HMM_TYPE", MODEL_TYPE);
  params.insert("CFG_QUEUE_TYPE", "Queue::POSIX");


  if(!modifyConfig(m_ConfigPath, params))
  {
    emit error(i18n("Cannot modify config at \"%1\"",m_ConfigPath));
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

  if(!pack(m_ModelDestination, m_ModelName))
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
  /*._.*/
  return true;
}

bool ModelCompilerSPHINX::pack(const QString &targetArchive, const QString &name)
{

  QHash<QString, QByteArray> fm;
  QDomDocument DomDocument;
  getMetaData(name, "SPHINX").serializeXml(DomDocument);
  //  kDebug() << DomDocument.toString();
  fm.insert("metadata.xml", DomDocument.toByteArray());

  QHash<QString, QString> efm;

  QString srcDirName;
  switch(compilationType)
  {
    case ModelCompiler::CompileLanguageModel:
    {
      srcDirName = m_BaseModelPath;
      break;
    }
    case ModelCompiler::AdaptSpeechModel:
    {
      srcDirName = m_ModelDir+QLatin1String("/")+m_ModelName+"/"+m_AdaptingModelName;
      break;
    }
    default:
    {
      srcDirName = m_ModelDir+QLatin1String("/")+m_ModelName+QLatin1String("/model_parameters/")+
                   m_ModelName+QLatin1String(MODEL_POSTFIX)+QLatin1String(SENONES_COUNT) + QLatin1String("/");
      break;
    }
  }

  kDebug() << QLatin1String("Model data from")+srcDirName;
  QDir sourceDir(srcDirName);

  if(!sourceDir.exists())
  {
    emit error(i18n("Failed to pack to archive. Source directory does not exist( \"%1\")", srcDirName));
    return false;
  }

  foreach (QString tFileName, sourceDir.entryList())
  {
    efm.insert(srcDirName+tFileName, tFileName);
    kDebug()<<srcDirName+tFileName;
  }

  QString fetc = m_ModelDir+QLatin1String("/")+m_ModelName+QLatin1String("/etc/")+m_ModelName;
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

QHash<QString, QString> ModelCompilerSPHINX::readFeatParams(const QString &filename)
{
  QFile configFile(filename);
  QHash<QString, QString> args;
  if (!configFile.open(QIODevice::ReadOnly))
  {
    emit error(i18n("Failed to read feat.params  \"%1\"", filename));
    return args;
  }

  QTextStream in(&configFile);
  while (!in.atEnd())
  {
    QString line = in.readLine();
    args.insert(line.mid(1, line.indexOf(" ") -1), line.right(line.indexOf(" ")+1));
  }

  return args;
}

bool ModelCompilerSPHINX::copyModelForAdapting(const QString &source, const QString &destination)
{
  if(!FileUtils::copyDirRecursive(source, destination))
  {
    emit error(i18n("Failed to copy model for adapting."));
    return false;
  }

  m_AdaptingModelName = QDir(source).dirName();

  return true;
}

bool ModelCompilerSPHINX::generateAcousticFeatureFiles()
{

  QString execString = m_Sphinx_fe +QLatin1String(" -argfile ") +m_BaseModelPath+"/feat.params "
                       "-samprate "+ DEFAULT_SAMPRATE+" -c " +m_ModelDir+"/"+m_ModelName+"/etc/"+m_ModelName+"_train.fileids "
                       "-di " + m_WavPath + " -do "+ m_ModelDir+"/"+m_ModelName +" -ei wav -eo mfc -mswav yes";
  if(execute(execString, m_ModelDir+"/"+m_ModelName))
    return true;
  else
  {
    processError();
    return false;
  }
  return true;
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
    emit error(i18n("Failed to write config at  \"%1\"", filename));
    return false;
  }

  configFile.write(outArr);
  configFile.close();

  //  QString content = QString(configFile.read);

  return true;
}

bool ModelCompilerSPHINX::convertMdef()
{
  QString execString = m_Pocketsphinx_mdef_convert +" -text " + m_BaseModelPath+"/mdef " + m_ModelDir+"/"+m_ModelName+"/"+m_AdaptingModelName+"/mdef.txt";
  if(execute(execString, m_ModelDir+"/"+m_ModelName))
    return true;
  else
  {
    processError();
    return false;
  }
  return true;
}

bool ModelCompilerSPHINX::getStatistics()
{
  QHash<QString, QString> args = readFeatParams(m_BaseModelPath+"/feat.params");
  QString fetc = m_ModelDir+QLatin1String("/")+m_ModelName+QLatin1String("/etc/")+m_ModelName;
  QString execString = m_Bw +" -hmmdir " + m_ModelDir + "/"+m_ModelName+"/"+m_AdaptingModelName +
                       " -moddeffn " +m_ModelDir+"/"+m_ModelName+"/"+m_AdaptingModelName+"/mdef.txt"+
                       " -ts2cbfn " + MODEL_TYPE +" -feat "+args.value("feat")+ " -svspec " + args.value("svspec")+
                       " -cmn " + args.value("cmn")+ " -agc "+args.value("agc")+
                       " -dictfn " + fetc+".dic " + "-ctlfn " +fetc+"_train.fileids " + "-lsnfn " + fetc+"_train.transcription "+
                       + "-accumdir " + m_ModelDir+"/"+m_ModelName;

  if(execute(execString, m_ModelDir+"/"+m_ModelName))
    return true;
  else
  {
    processError();
    return false;
  }
  return true;
}

bool ModelCompilerSPHINX::mapUpdate()
{
  QString modelOutDir = m_ModelDir+"/"+m_ModelName+"/"+m_AdaptingModelName;

  QString execString = m_Map_adapt + " -meanfn " + m_BaseModelPath+"/means "+
                       "-varfn " + m_BaseModelPath+"/variances "+
                       "-mixwfn " + m_BaseModelPath+"/mixture_weights "+
                       "-tmatfn " + m_BaseModelPath+"/transition_matrices "+
                       "-accumdir " + m_ModelDir+"/"+m_ModelName +
                       " -mapmeanfn " + modelOutDir+"/means "+
                       "-mapvarfn " + modelOutDir+"/variances "+
                       "-mapmixwfn " + modelOutDir+"/mixture_weights "+
                       "-maptmatfn " + modelOutDir+"/transition_matrices";

  if(execute(execString, m_ModelDir+"/"+m_ModelName))
    return true;
  else
  {
    processError();
    return false;
  }
  return true;
}

QString ModelCompilerSPHINX::information(bool condensed) const
{
  return i18n("ShinxTrain: (unknown version)");
}


ModelCompilerSPHINX::ModelCompilerSPHINX(const QString &userName, QObject *parent):ModelCompiler(userName, parent)
{
  keepGoing = false;
}
