//
// C++ Implementation: renameterminalwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "renameterminalwizard.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include "renameterminalselectparameterspage.h"
#include "renameterminalworkingpage.h"
#include "../grammarmanager.h"

RenameTerminalWizard::RenameTerminalWizard(QWidget* parent): QWizard(parent)
{
	setWindowTitle(i18n("Terminal umbenennen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/editterminal.png"));
	addPage(createIntroPage());
	addPage(createSelectParametersPage());
	addPage(createWorkingPage());
	addPage(createFinishedPage());
}


QWizardPage* RenameTerminalWizard::createSelectParametersPage()
{
	return  new RenameTerminalSelectParametersPage(this);
}

QWizardPage* RenameTerminalWizard::createWorkingPage()
{
	RenameTerminalWorkingPage *work =  new RenameTerminalWorkingPage(this);
	connect(work, SIGNAL(done()), this, SLOT(next()));
	return work;
}


QWizardPage* RenameTerminalWizard::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(intro);
	QLabel *desc = new QLabel(intro);
	lay->addWidget(desc);
	intro->setLayout(lay);

	desc->setWordWrap(true);
	intro->setTitle(i18n("Willkommen zum Umbenennen eines Terminals"));
	desc->setText(i18n("Dieser Assistent wird Ihnen erlauben, einen bestehenden Terminal der Grammatik und der Wortliste umzubennennen.\n\nSie können dabei konfigurieren, ob Sie all diese Bereiche in die Aktion einbeziehen möchten.\n\nWenn Sie mehrere Terminale zusammenfügen wollen, benutzen Sie bitte den \"Terminal verbinden\"-Wizard. Er bietet Ihnen ebenfalls die Möglichkeit, dem verbundenen Terminal einen neuen Nahmen zu verleihen."));

	return intro;
}

QWizardPage* RenameTerminalWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(finished);
	QLabel *desc = new QLabel(finished);
	lay->addWidget(desc);
	finished->setLayout(lay);

	desc->setWordWrap(true);
	finished->setTitle(i18n("Unbenennen Abgeschlossen"));
	desc->setText(i18n("Der Terminal wurde erfolgreich umbenannt.\n\nVielen Dank, dass Sie simon verbessert haben."));

	return finished;
}


RenameTerminalWizard::~RenameTerminalWizard()
{
}


