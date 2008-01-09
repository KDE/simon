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

FirstRunCreateDictionaryPage::FirstRunCreateDictionaryPage(AddWordView* addWordView, QWidget* parent): QWizardPage(parent)
{
	this->setTitle(tr("Lese Konfiguration..."));
    this->addWordView = addWordView;
}

void FirstRunCreateDictionaryPage::initializePage()
{
	TrainingManager *trainingManager = new TrainingManager(addWordView);
	WordListManager *wordListManager = new WordListManager(trainingManager);

	emit trainingManagerCreated(trainingManager);
	emit wordListManagerCreated(wordListManager);
	emit done();
}


FirstRunCreateDictionaryPage::~FirstRunCreateDictionaryPage()
{
}


