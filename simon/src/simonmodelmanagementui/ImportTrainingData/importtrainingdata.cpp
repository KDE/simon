/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "importtrainingdata.h"
#include <KDebug>
#include <simonsound/postprocessing.h>
#include <simonscenarios/trainingmanager.h>
#include <QDir>
#include <QDate>
#include <KStandardDirs>
#include <QTime>
#include <KLocalizedString>

ImportTrainingData::ImportTrainingData(QObject* parent) : QThread(parent),
  pp(new PostProcessing()),
  stopRequest(false)
{
  connect(pp, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
}


void ImportTrainingData::run()
{
  stopRequest = false;
  prog=0;
  emit progress(0,0);                             //waiting...
  QString wavDestDir=TrainingManager::getInstance()->getTrainingDir();

  if (isPrompts) {
    emit status(i18n("Reading prompts file..."));
    PromptsTable *prompts = TrainingManager::getInstance()->readPrompts(promptsPath);
    if (!prompts) {
      emit error(i18nc("%1 is path", "Could not parse input prompts: %1", promptsPath));
      return;
    }

    emit progress(0, prompts->count());
    emit status(i18ncp("%1 is file count", "Importing %1 File...", "Importing %1 Files...", prompts->count()));

    QStringList files = prompts->keys();
    QStringList filesFullPath;

    QStringList allowedFileTypes = getAllowedFileTypes();

    bool fileFound;
    foreach (const QString& file, files) {
      if (stopRequest) return;
      fileFound = false;
      foreach (const QString& extension, allowedFileTypes)
      {
        QString thisFile = basePath+QDir::separator()+file+'.'+extension;
        if (QFile::exists(thisFile))
        {
          filesFullPath << thisFile;
          fileFound = true;
          break;
        }
      }
      if (!fileFound)
      {
        emit error(i18nc("%1 is path", "Cannot find file referenced in prompts: %1", file));
        return;
      }
    }

    QStringList *newFiles = processSounds(filesFullPath, wavDestDir);

    if ((!newFiles) || (files.count() != newFiles->count())) return;

    int i=0;
    foreach (const QString& file, files) {
      //adding to trainingmanager
      QString content = prompts->value(file);
      QString filename = newFiles->at(i);
      filename = filename.left(filename.lastIndexOf('.'));
      filename = filename.mid(filename.lastIndexOf(QDir::separator())+1);

      //TODO: allow imported files to have customized sample groups
      TrainingManager::getInstance()->addSample(filename, "default", content.toUpper());
      i++;
    }

    delete newFiles;
    delete prompts;

    TrainingManager::getInstance()->savePrompts();
  }
  else {
    emit status(i18n("Collecting files..."));

    QStringList *dataFiles = this->searchDir(directory);
    if (!dataFiles) return;

    emit progress(0, dataFiles->count());
    emit status(i18ncp("%1 is file count", "Importing %1 File...", "Importing %1 Files...", dataFiles->count()));

    QStringList *newFiles = processSounds(*dataFiles, wavDestDir);
    delete dataFiles;

    if (!newFiles) return;

    emit status(i18n("Creating transcriptions..."));
    if (!createPrompts(*newFiles)) return;

    delete newFiles;
  }
  emit status(i18nc("The import process has finished", "Finished"));
  emit done();
}


bool ImportTrainingData::import(bool isPrompts, QString path, QString basePath)
{
  if (isPrompts) {
    this->promptsPath = path;
    this->basePath = basePath;
  }
  else {
    this->directory = path;
  }
  this->isPrompts = isPrompts;

  start();
  return true;
}

QStringList ImportTrainingData::getAllowedFileTypes()
{
  QStringList allowedFileTypes;
  allowedFileTypes << "wav";
  allowedFileTypes << "mp3";
  allowedFileTypes << "ogg";
  allowedFileTypes << "flac";
  allowedFileTypes << "aac";
  return allowedFileTypes;
}

/**
 * \brief Walks the given dir. recursively and returns all ,wav files
 * @param dir The dir to walk
 * @return All files found
 */
QStringList* ImportTrainingData::searchDir(QString dir)
{
  QDir *dirHandle = new QDir(dir);
  if (!dirHandle->isReadable()) {
    delete dirHandle;
    return 0;
  }

  QStringList dirsToCheck;
  QStringList *dataFiles = new QStringList();
  dirsToCheck<<dir;

  QStringList allowedFileTypes = getAllowedFileTypes();
  allowedFileTypes.replaceInStrings(QRegExp("^"), "*.");
  kDebug() << "Allowed: " << allowedFileTypes;

  QStringList dirs;
  QStringList files;

  while (!dirsToCheck.isEmpty()) {
    if (stopRequest)
    {
      delete dirHandle;
      return 0;
    }

    dirHandle->setPath(dirsToCheck.takeAt(0));
    dirs = dirHandle->entryList(QDir::Dirs);
    for (int i=2; i < dirs.count(); i++)
      dirsToCheck.append(dirHandle->path()+'/'+dirs[i]);

    files = dirHandle->entryList(allowedFileTypes,  QDir::Files);
    for (int i=0; i < files.count(); i++)
      dataFiles->append(dirHandle->path()+'/'+files[i]);
  }
  delete dirHandle;
  return dataFiles;
}


/**
 * \brief Creates the promptsfile from the given stringlist
 * Using extractSaid()
 * \author Peter Grasch
 * \see extractSaid()
 * @param dataFiles The dataFiles to include in the prompts-file
 * @param destDir The destination file
 * @return success
 */
bool ImportTrainingData::createPrompts(QStringList dataFiles)
{
  TrainingManager *train = TrainingManager::getInstance();

  QFileInfo fileInfo;
  QString fileName, said;

  for (int i=0; i <dataFiles.count(); i++) {
    if (stopRequest)
      return false;
    fileInfo.setFile(dataFiles[i]);
    fileName = fileInfo.fileName();

    said = extractSaid(fileName);
    //TODO: allow imported files to have customized sample groups
    train->addSample(fileName.left(fileName.lastIndexOf('.')), "default", said.toUpper());
  }
  train->savePrompts();
  return true;
}


/**
 * \brief Extracts the information that has been said out of the source
 *
 * Returns the cleaned fielname without the extension and in uppercase
 *
 * \author Peter Grasch
 *
 * @param source The string to parse
 * @return cleaned string
 */
QString ImportTrainingData::extractSaid(QString source)
{
  QString said = source.left(source.lastIndexOf('.'));
  said.remove(QRegExp("_S[0-9]+_"));
  said.remove(QRegExp("([0-9]|\\.|\\(|\\)|\\[|\\]|\\-)"));
  said.replace('_', ' ');
  //	said.remove(QRegExp(" S$"));
  return said.trimmed();
}


/**
 * \brief Process the sound files from given in the list to the destDir
 *
 * @param dataFiles The given datafiles
 * @param destDir The destination folder
 * @return the datafiles - if not successful it returns 0
 */
QStringList* ImportTrainingData::processSounds(QStringList dataFiles,
    QString destDir)
{
  QString newFileName;
  QFileInfo fInfo;
  QStringList *newFiles = new QStringList();

  for (int i=0; i < dataFiles.count(); i++) {
    if (stopRequest) {
      delete newFiles;
      return 0;
    }

    fInfo.setFile(dataFiles[i]);
    QString dateTime = QDate::currentDate().toString ( "yyyy-MM-dd" ) +'_'+QTime::currentTime().toString("hh-mm-ss");
    newFileName = destDir+QDir::separator()+fInfo.fileName().left(fInfo.fileName().lastIndexOf('.')).replace(' ', '_')+'_'+QString::number(i)+'_'+dateTime+".wav";

    if (!pp->process(dataFiles[i], newFileName, false /*do not delete input*/,
    true /*silent*/)) {
      emit error(i18n("Could not process sound files"));
      return 0;
    }
    newFiles->append(newFileName);
    emit progress(++prog);
  }
  kDebug() << "Files processed" << newFiles->count();

  return newFiles;
}

void ImportTrainingData::terminate()
{
  stopRequest = true;
  QThread::terminate();
  wait();
}

ImportTrainingData::~ImportTrainingData()
{
  delete pp;
}
