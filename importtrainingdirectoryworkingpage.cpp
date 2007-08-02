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
#include <QDebug>

/**
 * \brief Constructor - creates the GUI
 * \author Peter Grasch
 * @param parent 
 * Parent of the widget
 */
ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle("Verarbeite Ordner...");
	completed = false;
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbMain = new QLabel(this);
	lbMain->setText("Der angegebene Ordner wird verarbeit.\nSie können den Fortschritt mit dem Fortschrittsbalken\nüberpruefen.\n\nBitte haben Sie einen Moment Geduld.\n\n");
	
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
// 	QString destdir = QDir::currentPath()+"/tmp";
// 	QString wavDestdir = destdir+"/wav";
	QString destdir = "/home/bedahr/model/voxforge_tutorial_auto-0.1/voxforge/auto";
	QString wavDestdir = "/home/bedahr/model/voxforge_tutorial_auto-0.1/voxforge/auto/train/16khz/wav";
	
	QStringList *dataFiles = this->searchDir(dir);
	if (!dataFiles) return error();
	

	pbMain->setMaximum((dataFiles->count()-1)*10);
	pbMain->setValue(0);

	QDir *dirHandle = new QDir();
	dirHandle->mkpath(destdir);
	dirHandle->mkpath(wavDestdir);
	delete dirHandle;
	
	if (!createPrompts(*dataFiles, destdir)) return error();

// 	delete dataFiles;
	dataFiles = processSounds(*dataFiles, wavDestdir);
	if (!dataFiles) return error();

	if(!createScp(*dataFiles, destdir)) return error();
	

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
 * @param destDir The destination directory
 * @return success
 */
bool ImportTrainingDirectoryWorkingPage::createPrompts(QStringList dataFiles, QString destDir)
{
	QFile *prompts = new QFile(destDir+"/prompts", this);
	if (!prompts->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Fehler"), 
			tr("Beim Öffnen der Ausgabe-Dateie \"prompts\" ist ein Fehler aufgetreten."));
		return false;
	}
	QTextStream *promptsStream = new QTextStream(prompts);

	QFileInfo fileInfo;
	QString said, fileName;
	
	for (int i=0; i <dataFiles.count(); i++)
	{
		fileInfo.setFile(dataFiles[i]);
		fileName = fileInfo.fileName();

		said = extractSaid(fileName);
		(*promptsStream) << "*/" << fileName << " " << said << endl;
		
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
		
		if (!QProcess::execute("resample -to 16000 "+dataFiles[i]+" "+newFileName))
		{
			//something went wrong
			//resample always returns ERROR - crap
// 			QMessageBox::critical(this, tr("Fehler"), tr("Konnte %1 nicht nach %2 resamplen. Bitte ueberpruefen Sie ob Sie das Programm \"resample\" installiert haben und es im Suchpfad liegt und ob Sie all die noetigen Berechtigungen besitzen.").arg(dataFiles[i]).arg(newFileName));
// 			return NULL;
		}
		prog += 5;
		pbMain->setValue(prog);
		if (!QProcess::execute("normalize-audio "+newFileName))
		{
			//something went wrong
// 			QMessageBox::critical(this, tr("Fehler"), tr("Konnte %1 nicht nach %2 normalisieren. Bitte ueberpruefen Sie ob Sie das Programm \"resample\" installiert haben und es im Suchpfad liegt und ob Sie all die noetigen Berechtigungen besitzen.").arg(dataFiles[i]).arg(newFileName));
// 			return NULL;
		}
		*newFiles << newFileName;
		prog += 3;
		pbMain->setValue(prog);
	}

	pbMain->setValue(prog);
	
	return newFiles;
}

/**
 * \brief Creates the codetrain.scp with the dataFiles in the destDir
 * @param dataFiles The datafiles to process
 * @param destDir The directory where we should write the file
 * @return success
 */
bool ImportTrainingDirectoryWorkingPage::createScp(QStringList dataFiles, QString destDir)
{
	QFile *scp = new QFile(destDir+"/codetrain.scp", this);
	if (!scp->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Fehler"), 
			tr("Beim öffnen der Ausgabedateien ist ein Fehler aufgetreten."));
		return false;
	}
	
	QTextStream *scpStream = new QTextStream(scp);
	QFileInfo fileInfo;
	QString fileName;
	
	for (int i=0; i <dataFiles.count(); i++)
	{
		fileInfo.setFile(dataFiles[i]);
		fileName = fileInfo.fileName();
		(*scpStream) << dataFiles[i] << " " << "interim_files/mfc/" << fileName.left(fileName.lastIndexOf(".")) << ".mfc" << endl;
		
		pbMain->setValue(++prog);
	}
	scpStream->flush();
	scp->close();
	return true;
}

