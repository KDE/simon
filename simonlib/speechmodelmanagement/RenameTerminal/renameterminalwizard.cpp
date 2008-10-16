/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "renameterminalwizard.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <KStandardDirs>
#include "renameterminalselectparameterspage.h"
#include "renameterminalworkingpage.h"
#include "../grammarmanager.h"

RenameTerminalWizard::RenameTerminalWizard(QWidget* parent): QWizard(parent)
{
	setWindowTitle(i18n("Terminal umbenennen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/editterminal.png")));
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


