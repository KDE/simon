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

ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle("Verarbeite Ordner...");
	completed = false;
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbMain = new QLabel(this);
	lbMain->setText("Der angegebene Ordner wird verarbeit.\nSie koennen den Fortschritt mit dem Fortschrittsbalken\nueberpruefen.\n\nBitte haben Sie einen Moment Geduld.\n\n");
	
	pbMain = new QProgressBar(this);
	pbMain->setMaximum(0);

	lay->addWidget(lbMain);
	lay->addWidget(pbMain);
	setLayout(lay);
}

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


bool ImportTrainingDirectoryWorkingPage::createPrompts(QStringList dataFiles, QString destDir)
{
	QFile *prompts = new QFile(destDir+"/prompts", this);
	if (!prompts->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Fehler"), 
			tr("Beim öffnen der Ausgabe-Dateie \"prompts\" ist ein Fehler aufgetreten."));
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


QString ImportTrainingDirectoryWorkingPage::extractSaid(QString source)
{
	QString said = source.left(source.lastIndexOf("."));
	said.remove(QRegExp("([0-9]|\\.|\\(|\\)|\\[|\\])"));
	said.replace("_", " ");
	return said.toUpper();
}

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

// 	QString allFiles;
// 	for (int i=0; i < newFiles->size(); i++) allFiles += " "+newFiles->at(i);
	
// 	QProcess::execute("normalize-audio "+allFiles);
// 	prog += newFiles->size()-1;
	pbMain->setValue(prog);
	
	return newFiles;
}

bool ImportTrainingDirectoryWorkingPage::createScp(QStringList dataFiles, QString destDir)
{
	QFile *scp = new QFile(destDir+"/codetrain.scp", this);
	if (!scp->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Fehler"), 
			tr("Beim oeffnen der Ausgabedateien ist ein Fehler aufgetreten."));
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


ImportTrainingDirectoryWorkingPage::~ImportTrainingDirectoryWorkingPage()
{
}


