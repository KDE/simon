//
// C++ Implementation: mergeterminalswizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mergeterminalswizard.h"
#include "mergeterminalsselectterminalspage.h"
#include "mergeterminalsworkingpage.h"
#include "grammarmanager.h"
#include "wordlistmanager.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>

MergeTerminalsWizard::MergeTerminalsWizard(GrammarManager *grammarManager, QWidget* parent): SimonWizard(parent)
{
	setWindowTitle(tr("Terminale verbinden"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/merge.png"));
	this->grammarManager = grammarManager;
	addPage(createIntroPage());
	addPage(createSelectTerminalsPage());
	addPage(createWorkingPage());
	addPage(createFinishedPage());
}

QWizardPage* MergeTerminalsWizard::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(intro);
	QLabel *desc = new QLabel(intro);
	lay->addWidget(desc);
	intro->setLayout(lay);

	desc->setWordWrap(true);
	intro->setTitle(tr("Willkommen zum Verbinden mehrerere Terminale"));
	desc->setText(tr("Dieser Assistent wird Ihnen erlauben, zwei verschiedene Terminale zu einem zusammenzufassen.\n\nDies ermöglicht es, die Grammatik sauber zu halten und große Gruppen von Wörtern gleichzeitig grammatikalisch einzufügen. (Wenn zum Beispiel durch das Importieren eines Wörterbuches anders benannte Terminale hinzugefügt wurden, die auf andere, bestehende Wortgruppen gebunden werden sollen)."));

	return intro;
}

QWizardPage* MergeTerminalsWizard::createSelectTerminalsPage()
{
	return new MergeTerminalsSelectTerminalsPage(grammarManager->getWordListManager(), this);
}

QWizardPage* MergeTerminalsWizard::createWorkingPage()
{
	MergeTerminalsWorkingPage *working = new MergeTerminalsWorkingPage(grammarManager, grammarManager->getWordListManager(), this);
	connect(working, SIGNAL(done()), this, SLOT(next()));
	return working;
}

QWizardPage* MergeTerminalsWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(finished);
	QLabel *desc = new QLabel(finished);
	lay->addWidget(desc);
	finished->setLayout(lay);

	desc->setWordWrap(true);
	finished->setTitle(tr("Import abgeschlossen"));
	desc->setText(tr("Die Terminale wurden verbunden.\n\nVielen Dank, dass Sie simon verbessert haben."));

	return finished;
}


MergeTerminalsWizard::~MergeTerminalsWizard()
{
}


