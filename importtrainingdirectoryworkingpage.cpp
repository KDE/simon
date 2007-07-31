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
#include <QLabel>

ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle("Verarbeite Ordner...");
	completed = false;
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbMain = new QLabel(this);
	lbMain->setText("Der angegebene Ordner wird verarbeit.\nSie koennen den Fortschritt mit dem Fortschrittsbalken\nueberprüfen.\n\nBitte haben Sie einen Moment Geduld.");
	
	pbMain = new QProgressBar(this);
	pbMain->setMaximum(0);

	lay->addWidget(lbMain);
	lay->addWidget(pbMain);
	setLayout(lay);
}

bool ImportTrainingDirectoryWorkingPage::importDir(QString dir)
{
// 	QMessageBox::information(this, "Importiere...", "Importiere den Ordner: "+dir);

	QDir *dirHandle = new QDir(dir);

	QStringList dirsToCheck;
	QStringList dataFiles;
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
			dataFiles.append(dirHandle->path()+"/"+files[i]);
	}

	pbMain->setMaximum(dataFiles.count());
	pbMain->setValue(0);


	dirHandle->mkpath(QDir::currentPath()+"/tmp");
	QFile *prompts = new QFile(QDir::currentPath()+"/tmp/prompts", this);
	QFile *scp = new QFile(QDir::currentPath()+"/tmp/codetrain.scp", this);
	if (!prompts->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text) ||
		!scp->open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
	{
		QMessageBox::critical(this, tr("Fehler"), 
			tr("Beim öffnen der Ausgabe-Dateien ist ein Fehler aufgetreten."));
		completed = false;
		return false;
	}
	QTextStream *promptsStream = new QTextStream(prompts);
	QTextStream *scpStream = new QTextStream(scp);
	QFileInfo fileInfo;
	QString said, fileName;
	
	for (int i=0; i <dataFiles.count(); i++)
	{
// 		QMessageBox::information(this, "Hallo", dataFiles[i]);
		fileInfo.setFile(dataFiles[i]);
		fileName = fileInfo.fileName();

		said = fileName;
		said.remove(QRegExp("([0-9]|\\.|\\(|\\)|\\[|\\])"));
		said.replace(QRegExp("\\_*"), " ");

		said = said.toUpper();

		(*promptsStream) << "*/" << fileName << " " << said << endl;
		(*scpStream) << dataFiles[i] << " " << "interim_files/mfc/" << fileName.left(fileName.lastIndexOf(".")) << ".mfc" << endl;
		
		pbMain->setValue(i);
	}
	promptsStream->flush();
	scpStream->flush();
	prompts->close();
	scp->close();

	completed = true;

	completeChanged();
	return true;
}

ImportTrainingDirectoryWorkingPage::~ImportTrainingDirectoryWorkingPage()
{
}


