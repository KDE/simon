//
// C++ Implementation: importgrammarworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importgrammarworkingpage.h"
#include "wordlistmanager.h"
#include "importgrammar.h"
#include <QFile>

ImportGrammarWorkingPage::ImportGrammarWorkingPage(WordListManager *wordListManager, QWidget* parent): QWizardPage(parent)
{
	completed=false;
	setTitle(tr("Analyse läuft..."));
	ui.setupUi(this);
	this->grammarImporter = new ImportGrammar(wordListManager,this);
	connect(grammarImporter, SIGNAL(status(QString)), this, SLOT(printStatus(QString)));
	connect(grammarImporter, SIGNAL(fileProgress(int, int)), this, SLOT(displayFileProgress(int, int)));
	connect(grammarImporter, SIGNAL(allProgress(int, int)), this, SLOT(displayWholeProgress(int, int)));
	connect(grammarImporter, SIGNAL(grammarCreated(QStringList)), this, SIGNAL(grammarCreated(QStringList)));
	this->wordListManager = wordListManager;
}

void ImportGrammarWorkingPage::printStatus(QString status)
{
	ui.lbCurrentAction->setText(status);
	QCoreApplication::processEvents();
}

void ImportGrammarWorkingPage::displayFileProgress(int progress, int max)
{
	ui.pbProgress->setMaximum(max);
	ui.pbProgress->setValue(progress);
	QCoreApplication::processEvents();
}

void ImportGrammarWorkingPage::displayWholeProgress(int progress, int max)
{
	ui.pbTotal->setMaximum(max);
	ui.pbTotal->setValue(progress);
	QCoreApplication::processEvents();
}

void ImportGrammarWorkingPage::initializePage()
{
	QStringList files = field("files").toString().split("||");
	
	grammarImporter->setFiles(files);
	grammarImporter->setIncludeUnknown(field("includeUnknown").toBool());
	grammarImporter->start();
}


ImportGrammarWorkingPage::~ImportGrammarWorkingPage()
{
}


