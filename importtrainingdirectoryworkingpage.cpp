//
// C++ Implementation: importtrainingdirectoryworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingdirectoryworkingpage.h"
#include <QPushButton>
#include <QDir>
#include <QPushButton>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileInfo>
#include <QRegExp>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include <QLabel>
#include <QCoreApplication>
#include <QVariant>
#include "settings.h"
#include <math.h>

/**
 * \brief Constructor - creates the GUI
 * \author Peter Grasch
 * @param parent 
 * Parent of the widget
 */
ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(tr("Verarbeite Ordner..."));
	completed = false;
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbMain = new QLabel(this);
	lbMain->setText(tr("Der angegebene Ordner wird verarbeit.\nSie können den Fortschritt mit dem Fortschrittsbalken\nüberprüfen.\n\nBitte haben Sie einen Moment Geduld.\n\n"));
	
	pbMain = new QProgressBar(this);
	pbMain->setMaximum(0);

	lay->addWidget(lbMain);
	lay->addWidget(pbMain);
	setLayout(lay);
}

/**
 * \brief Starts the importing process and calls all the other methods
 * \author Peter Grasch
 * @param dir The directory to import
 * @return success
 */
bool ImportTrainingDirectoryWorkingPage::importDir(QString dir)
{
	prog=0;
	QString wavDestdir = Settings::get("Model/PathToSamples").toString();
	
	QStringList *dataFiles = this->searchDir(dir);
	if (!dataFiles) return error();
	

	pbMain->setMaximum((dataFiles->count()-1)*10);
	pbMain->setValue(0);
	
	if (!createPrompts(*dataFiles, Settings::get("Model/PathToPrompts").toString())) return error();

// 	delete dataFiles;
	dataFiles = processSounds(*dataFiles, wavDestdir);
	if (!dataFiles) return error();
	

	completed = true;

	completeChanged();
	return true;
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
bool ImportTrainingDirectoryWorkingPage::createPrompts(QStringList dataFiles, QString dest)
{
	QFile *prompts = new QFile(dest, this);
	if (!prompts->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Fehler"), 
			tr("Beim Öffnen der Ausgabe-Dateie \"prompts\" ist ein Fehler aufgetreten."));
		return false;
	}
	QTextStream *promptsStream = new QTextStream(prompts);
	promptsStream->setCodec("UTF-8");

	QFileInfo fileInfo;
	QString said, fileName;
	
	for (int i=0; i <dataFiles.count(); i++)
	{
		fileInfo.setFile(dataFiles[i]);
		fileName = fileInfo.fileName();

		said = extractSaid(fileName);
		(*promptsStream) << "*/" << fileName.left(fileName.lastIndexOf(".")) << " " << said << endl;
		
		pbMain->setValue(++prog);
	}
	promptsStream->flush();
	prompts->close();
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
QString ImportTrainingDirectoryWorkingPage::extractSaid(QString source)
{
	QString said = source.left(source.lastIndexOf("."));
	said.remove(QRegExp("([0-9]|\\.|\\(|\\)|\\[|\\])"));
	said.replace("_", " ");
	return said.toUpper();
}

/**
 * \brief Walks the given dir. recursively and returns all ,wav files
 * @param dir The dir to walk
 * @return All files found
 */
QStringList* ImportTrainingDirectoryWorkingPage::searchDir(QString dir)
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
 * \brief Process the sound files from given in the list to the destDir
 * 
 * Resamples the audio to 16khz and normalizes it afterwards.
 * 
 * @param dataFiles The given datafiles
 * @param destDir The destination directory
 * @return the datafiles - if not successful it returns NULL
 */
QStringList* ImportTrainingDirectoryWorkingPage::processSounds(QStringList dataFiles, 
		QString destDir)
{
	QString newFileName;
	QFileInfo fInfo;
	QStringList *newFiles = new QStringList();
	
	for (int i=0; i < dataFiles.count(); i++)
	{
		//resample
		fInfo.setFile(dataFiles[i]);
		newFileName = destDir+"/"+fInfo.fileName();
		
		if (QFile::exists(newFileName) && (!QFile::remove(newFileName)))
		{
			QMessageBox::critical(this, tr("Fehler"), tr("Konnte %1 nicht überschreiben. Bitte überprüfen Sie, ob Sie die nötigen Rechte besitzen.").arg(newFileName));
			return NULL;
		}
		
		QStringList filters = Settings::get("Model/ProcessingFilters").toString().split(" && ");
		float filterstep = 8/filters.count();
		float filtertot=0;
		QString filter;
		for (int j=0; j < filters.count(); j++)
		{
			QString execStr = filters.at(j);
			execStr.replace("\%1", dataFiles[i]);
			execStr.replace("\%2", newFileName);
			execStr.replace("\%3", Settings::get("Model/SampleRate").toString());
			execStr.replace("\%4", Settings::get("Model/Channels").toString());
			int ret = QProcess::execute(execStr);
			if (ret)
			{
				//something went wrong
				//resample always returns ERROR - crap
				QMessageBox::critical(this, tr("Fehler"), tr("Konnte %1 nicht nach %2 bearbeiten. Bitte ueberpruefen Sie ob Sie das Programm, installiert haben, der Pfad in den Einstellungen richtig angegeben wurde und ob Sie all die nötigen Berechtigungen besitzen. (Rückgabewert %3) (Ausgefuehrtes Kommando: %4)").arg(dataFiles[i]).arg(newFileName).arg(ret).arg(execStr));
				return NULL;
			}
			filtertot = filtertot+filterstep;
			prog += round(filtertot);
			pbMain->setValue(prog);
			QCoreApplication::processEvents();
		}
	}

	pbMain->setValue(prog);
	
	return newFiles;
}
