//
// C++ Implementation: firstruncreatedictionarypage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "firstruncreatedictionarypage.h"
#include <QHBoxLayout>
#include <QLabel>
#include "wordlistmanager.h"
#include "trainingmanager.h"

FirstRunCreateDictionaryPage::FirstRunCreateDictionaryPage(QWidget* parent): QWizardPage(parent)
{
	this->setTitle(tr("Lese Konfiguration..."));
}

void FirstRunCreateDictionaryPage::initializePage()
{
	TrainingManager *trainingManager = new TrainingManager();
	WordListManager *wordListManager = new WordListManager(trainingManager);

	emit trainingManagerCreated(trainingManager);
	emit wordListManagerCreated(wordListManager);
	emit done();
}


FirstRunCreateDictionaryPage::~FirstRunCreateDictionaryPage()
{
}


