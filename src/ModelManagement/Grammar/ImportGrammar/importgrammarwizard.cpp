//
// C++ Implementation: importgrammarwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importgrammarwizard.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include "importgrammarselectfilespage.h"
#include "importgrammarworkingpage.h"

ImportGrammarWizard::ImportGrammarWizard(QWidget* parent): QWizard(parent)
{
	setWindowTitle(tr("Grammatikstrukturen Importieren"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/importgrammar.png"));
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
	intro->setTitle(tr("Willkommen zum Importieren der Grammatik"));
	desc->setText(tr("Dieser Assistent wird Ihnen erlauben, beliebig viele Textdateien auszuwählen aus denen dann ein Grammatikprofil erstellt wird.\n\nDazu werden die einzelnen Wörter mit Hilfe der Wortliste in ihre grammatikalischen Kategorien (\"Terminale\") aufgelöst und anhand dieser die Satzkonstrukte kreiert.\n\nBitte überprüfen Sie die Wortliste auf Korrektheit bevor Sie diesen Assistenten ausführen."));

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
	finished->setTitle(tr("Import abgeschlossen"));
	desc->setText(tr("Die Dateien wurden erfolgreich importiert.\n\nBitte überprüfen Sie die importierten Strukturen noch auf offensichtliche Fehler.\n\nVielen Dank dass Sie simon verbessert haben!"));

	return finished;
}


ImportGrammarWizard::~ImportGrammarWizard()
{
}


