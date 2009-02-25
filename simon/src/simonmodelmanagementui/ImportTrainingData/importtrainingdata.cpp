/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
#include <speechmodelmanagement/trainingmanager.h>
// #include "coreconfiguration.h"
#include <QDir>
#include <QDate>
#include <KStandardDirs>
#include <QTime>
#include <KLocalizedString>

ImportTrainingData::ImportTrainingData(QObject* parent): QThread(parent)
{
	this->pp = new PostProcessing();
}


void ImportTrainingData::run()
{
	prog=0;
	emit progress(0,0); //waiting...
	emit status(i18n("Collecting files..."));
	QString wavDestDir=TrainingManager::getInstance()->getTrainingDir();

	QDir d(wavDestDir);
	if (!d.exists())
		if (!d.mkpath(wavDestDir))
			emit error(i18n("Couldn't create output folder %1", wavDestDir));
	
	QStringList *dataFiles = this->searchDir(directory);
	if (!dataFiles) return;
	

	emit progress(0, dataFiles->count());
	emit status(i18n("Importing %1 Files...", dataFiles->count()));
	kDebug() << "Sent status";

	dataFiles = processSounds(*dataFiles, wavDestDir);
	
	if (!dataFiles) return;
	
	emit status(i18n("Creating automatic transcriptions..."));
	
	if (!createPrompts(*dataFiles)) return;
	emit status(i18n("Finished"));
	emit done();
}

bool ImportTrainingData::import(QString directory)
{
	this->directory = directory;
	start();
	return true;
}

/**
 * \brief Walks the given dir. recursively and returns all ,wav files
 * @param dir The dir to walk
 * @return All files found
 */
QStringList* ImportTrainingData::searchDir(QString dir)
{
	QDir *dirHandle = new QDir(dir);
	if ((!dirHandle) || (!dirHandle->isReadable())) return NULL;

	QStringList dirsToCheck;
	QStringList *dataFiles = new QStringList();
	dirsToCheck<<dir;

	QStringList allowedFileTypes;
	allowedFileTypes << "*.wav";
	allowedFileTypes << "*.mp3";
	allowedFileTypes << "*.ogg";
	allowedFileTypes << "*.flac";
	QStringList dirs;
	QStringList files;

	while (!dirsToCheck.isEmpty())
	{
		dirHandle->setPath(dirsToCheck.takeAt(0));
		dirs = dirHandle->entryList(QDir::Dirs);
		for (int i=2; i < dirs.count(); i++)
			dirsToCheck.append(dirHandle->path()+"/"+dirs[i]);

		files = dirHandle->entryList(allowedFileTypes,  QDir::Files);
		for (int i=0; i < files.count(); i++)
			dataFiles->append(dirHandle->path()+"/"+files[i]);
	}
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

	for (int i=0; i <dataFiles.count(); i++)
	{
		fileInfo.setFile(dataFiles[i]);
		fileName = fileInfo.fileName();

		said = extractSaid(fileName);
		train->addSample(fileName.left(fileName.lastIndexOf(".")), said.toUpper());
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
	QString said = source.left(source.lastIndexOf("."));
	said.remove(QRegExp("([0-9]|\\.|\\(|\\)|\\[|\\]|\\-)"));
	said.replace("_", " ");
	said.remove(QRegExp(" S$"));
	return said.trimmed();
}


/**
 * \brief Process the sound files from given in the list to the destDir
 * 
 * Resamples the audio to 16khz and normalizes it afterwards.
 * 
 * @param dataFiles The given datafiles
 * @param destDir The destination directory
 * @return the datafiles - if not successful it returns NULL
 */
QStringList* ImportTrainingData::processSounds(QStringList dataFiles, 
		QString destDir)
{
	kDebug() << "Entering processSounds()";
	QString newFileName;
	QFileInfo fInfo;
	QStringList *newFiles = new QStringList();
	
	kDebug() << "Hier" << dataFiles.count();
	for (int i=0; i < dataFiles.count(); i++)
	{
		kDebug() << "Importing" << i;
		fInfo.setFile(dataFiles[i]);
		QString dateTime = QDate::currentDate().toString ( "yyyy-MM-dd" ) +"_"+QTime::currentTime().toString("hh-mm-ss");
		newFileName = destDir+"/"+fInfo.fileName().left(fInfo.fileName().lastIndexOf(".")).replace(" ", "_")+"_"+QString::number(i)+"_"+dateTime+".wav";


		if (!pp->process(dataFiles[i], newFileName, false /*don't delete input*/,
					true /*silent*/))
		{
			emit error(i18n("Couldn't process soundfiles"));
			return NULL;
		}
		newFiles->append(newFileName);
		emit progress(++prog);
	}
	kDebug() << "Files processed" << newFiles->count();
	
	return newFiles;
}

ImportTrainingData::~ImportTrainingData()
{
	delete pp;
}


