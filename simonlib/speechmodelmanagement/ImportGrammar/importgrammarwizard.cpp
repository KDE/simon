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


#include "importgrammarwizard.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <KStandardDirs>
#include "importgrammarselectfilespage.h"
#include "importgrammarworkingpage.h"

ImportGrammarWizard::ImportGrammarWizard(QWidget* parent): QWizard(parent)
{
	setWindowTitle(i18n("Grammatikstrukturen Importieren"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/importgrammar.png")));
	addPage(createIntroPage());
	addPage(createSelectFilesPage());
	addPage(createWorkingPage());
	addPage(createFinishedPage());
}

QWizardPage* ImportGrammarWizard::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(intro);
	QLabel *desc = new QLabel(intro);
	lay->addWidget(desc);
	intro->setLayout(lay);

	desc->setWordWrap(true);
	intro->setTitle(i18n("Willkommen zum Importieren der Grammatik"));
	desc->setText(i18n("Dieser Assistent wird Ihnen erlauben, beliebig viele Textdateien auszuwählen aus denen dann ein Grammatikprofil erstellt wird.\n\nDazu werden die einzelnen Wörter mit Hilfe der Wortliste in ihre grammatikalischen Kategorien (\"Terminale\") aufgelöst und anhand dieser die Satzkonstrukte kreiert.\n\nBitte überprüfen Sie die Wortliste auf Korrektheit bevor Sie diesen Assistenten ausführen."));

	return intro;
}

QWizardPage* ImportGrammarWizard::createSelectFilesPage()
{
	return (QWizardPage*) new ImportGrammarSelectFilesPage(this);
}


QWizardPage* ImportGrammarWizard::createWorkingPage()
{
	ImportGrammarWorkingPage *working = new ImportGrammarWorkingPage(this);
	connect(working, SIGNAL(grammarCreated(QStringList)), this, SIGNAL(grammarCreated(QStringList)));
	connect(working, SIGNAL(grammarCreated(QStringList)), this, SLOT(next()));
	connect(button(QWizard::CancelButton), SIGNAL(clicked()), working, SLOT(cancel()));
	return working;
}

QWizardPage* ImportGrammarWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(finished);
	QLabel *desc = new QLabel(finished);
	lay->addWidget(desc);
	finished->setLayout(lay);

	desc->setWordWrap(true);
	finished->setTitle(i18n("Import abgeschlossen"));
	desc->setText(i18n("Die Dateien wurden erfolgreich importiert.\n\nBitte überprüfen Sie die importierten Strukturen noch auf offensichtliche Fehler.\n\nVielen Dank dass Sie simon verbessert haben!"));

	return finished;
}


ImportGrammarWizard::~ImportGrammarWizard()
{
}


