//
// C++ Implementation: modelmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "modelmanager.h"
#include "settings.h"
#include "logger.h"
#include <QCoreApplication>

#include <QMessageBox>
#include <QDebug>
#include <QVariant>
#include <QProgressDialog>

ModelManager::ModelManager(QWidget *parent) : QThread(parent)
{
	processDialog = new QProgressDialog();
	connect(processDialog, SIGNAL(canceled()), this, SLOT(terminate()));
	connect(this, SIGNAL(status(QString)), this, SLOT(setStatus(QString)));
	connect(this, SIGNAL(progress(int,int)), this, SLOT(setProgress(int,int)));
	connect(this, SIGNAL(error(QString)), this, SLOT(displayError(QString)));
	processDialog->setWindowTitle(tr("Generiere Sprachmodell..."));
}

void ModelManager::cancel()
{
	
}

void ModelManager::setStatus(QString status)
{
	processDialog->setLabelText(status);
}

void ModelManager::setProgress(int now, int max)
{
	processDialog->setMaximum(max);
	processDialog->setValue(now);
}

void ModelManager::displayError(QString error)
{
	QMessageBox::critical(0, tr("Fehler"), tr("Beim Kompilieren des Modells ist ein Fehler aufgetreten:\n\n%1").arg(error));
	processDialog->hide();
}

bool ModelManager::compileModel()
{
	if (isRunning()) return false;
	
	processDialog->show();
	start();
	return true;
}

bool ModelManager::generateDirectoryStructure()
{
	bool success=true;
	QDir dir(Settings::get("TempDir").toString());
	//remove if it exists
	
	if (!dir.exists("modeltmp"))
		success = dir.mkdir("modeltmp");
	
	return success;
}

void ModelManager::run()
{
	Logger::log(tr("[INF] Modell wird generiert..."));
	emit status(tr("Vorbereitung"));
	emit progress(0,1000);
	
	proc = new QProcess();
	
	if (!generateDirectoryStructure())
	{
		emit error(tr("Fehler beim Erstellen der Ordnerstruktur. Bitte überprüfen Sie die Berechtigungen für den Temporären Ordner (%1)").arg(Settings::getS("TempDir")));
		return;
	}
	emit progress(2);
	generateInputFiles();
	emit progress(50);
	
	emit status(tr("Abgeschlossen"));
	emit progress(1000);
}

bool ModelManager::generateInputFiles()
{
// 	proc->execute(Settings::get("Programs/Interpreter/Perl").toString()+" "
// 	+Settings::getS("Programs/HTK/prompts2wlist")+" "+Settings::getS("Model/PathToPrompts")+" "+
// 	+ proc->workingDirectory()+"/wlist");
	QString tmpDir = Settings::getS("TempDir")+"/modeltmp/";
	
	emit status(tr("Generiere Wordliste..."));
	//wlist
	if (!generateWlist())
	{
		emit error(tr("Erstellen der Wordliste fehlgeschlagen. Bitte überprüfen Sie die Berechtigungen für den Temporären Pfad (%1) und ob die Prompts-File richtig gesetzt ist (%2)")
				.arg(Settings::getS("TempDir")).arg(Settings::getS("Model/PathToPrompts")));
		return false;
	}
	
	emit progress(25);
	emit status(tr("Erstelle Monophone..."));
	
	
	proc->execute(Settings::getS("Programs/HTK/HDMan")+" -A -D -T 1 -m -w "+tmpDir+"/wlist -e "+tmpDir+" -n "+tmpDir+"/monophones1 -i "+tmpDir+"/dict "+Settings::getS("Model/PathToLexicon"));
	//    HDMan -A -D -T 1 -m -w ./interim_files/wlist -e ./input_files -n ./interim_files/monophones1 -i -l logs/Step2_HDMan_log ./interim_files/dict ../../lexicon/obelisk_lexicon > logs/Step2_HDMan.log
	
	return true;
}

bool ModelManager::generateWlist()
{
	QFile promptsFile(Settings::getS("Model/PathToPrompts"));
	if (!promptsFile.open(QIODevice::ReadOnly))
		return false;
	
	QStringList words;
	QStringList lineWords;
	QString line;
	while (!promptsFile.atEnd())
	{
		line = promptsFile.readLine(3000);
		lineWords = line.split(QRegExp("( |\n)"), QString::SkipEmptyParts);
		lineWords.removeAt(0); //ditch the file-id
		words << lineWords;
	}
	promptsFile.close();
	
	words << "SENT-END" << "SENT-START";
	words.sort();
	
	//remove doubles
	int i=1;
	while (i < words.count())
	{
		if (words[i] == words[i-1])
			words.removeAt(i);
		else i++;
	}
	
	QFile wlistFile(Settings::getS("TempDir")+"/modeltmp/wlist");
	if (!wlistFile.open(QIODevice::WriteOnly))
		return false;
	for (int i=0; i < words.count(); i++)
	{
		wlistFile.write(words[i].toLatin1()+"\n");
	}
	wlistFile.close();
	return true;
}

ModelManager::~ModelManager()
{
}


