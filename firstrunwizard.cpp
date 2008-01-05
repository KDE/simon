//
// C++ Implementation: firstrunwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QWizardPage>
#include "firstrunwizard.h"
#include "externalprogrammanager.h"
#include "systemwidgetpage.h"
#include "selectlanguagemodelsourcepage.h"
#include "modelsettings.h"

FirstRunWizard::FirstRunWizard(QWidget* parent): SimonWizard(parent)
{
	setWindowTitle(tr("simon Erstkonfiguration"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/firstrun.png"));
	addPage(createIntroPage());
	addPage(createCheckList());
	addPage(createExternalProgramsPage());
	addPage(createLanguageModelDescription());
	addPage(createLanguageModelSelectSource());
	addPage(createNewModelDescription());
	addPage(createModelSettings());
	addPage(createFinishedPage());
}

QWizardPage* FirstRunWizard::createLanguageModelDescription()
{
	QWizardPage *description = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(description);
	QLabel *desc = new QLabel(description);
	lay->addWidget(desc);
	description->setLayout(lay);

	desc->setWordWrap(true);
	description->setTitle(tr("Das Sprachmodell"));
	desc->setText(tr("<html><head /><body><p>Sie benötigen die folgenden Einzelteile:</p><ul><li>(Schatten-) Wörterbuch, (Schatten-) Vokabeln, Grammatik und Trainingsdaten (pfad, prompts, wav_config)<br />Personenbezogen<li>Sprachskripte (tree1.hed)<br />Sprachbezogen<li>Modellskripte (sil.hed, config, proto, mkphones0.led und -1.led, global.ded, mktri.led)<br />Generell</ul><p>Aus diesen Eingabedaten werden dann folgende Dateien erstellt:<ul><li>hmmdefs<br />Beinhaltet eine technische Repräsentation der Stimme<li>tiedlist<br />Eine Liste der verwendeten Lautkombinationen<li>dict / dfa<br />Eine von julian lesbare Form der Grammatik</body></html>"));

	return description;
}

QWizardPage* FirstRunWizard::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(intro);
	QLabel *desc = new QLabel(intro);
	lay->addWidget(desc);
	intro->setLayout(lay);

	desc->setWordWrap(true);
	intro->setTitle(tr("Willkommen bei simon"));
	desc->setText(tr("Dieser Assistent wird Ihnen helfen, simon zu konfigurieren.\n\nAlle Einstellungen die Sie in diesem Assistenten treffen können später über das Systemmenü wieder geändert werden. Bitte beachten Sie, das die Einstellungen hier essientiell für die Funktionstüchtigkeit von simon sind.\n\nSollten Sie sich bei einigen Punkten nicht sicher sein, sehen Sie bitte im Handbuch nach oder fragen Sie gegebenenfalls ihren Systemadministrator."));

	return intro;
}

QWizardPage* FirstRunWizard::createCheckList()
{
	QWizardPage *checklist = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(checklist);
	QLabel *desc = new QLabel(checklist);
	desc->setOpenExternalLinks(true);
	lay->addWidget(desc);
	checklist->setLayout(lay);

	desc->setWordWrap(true);
	checklist->setTitle(tr("Checkliste"));
	desc->setText(tr("<html><head/><body><p>Um simon erfolgreich zu benutzen werden einige externe Programme und Dateien benötigt.</p><ul><li>HTK Toolkit<br />(Erhältlich von der <a href=\"http://htk.eng.cam.ac.uk/\">Universität Cambridge</a>)<li>Julius<br />(Erhältlich von der offiziellen <a href=\"http://julius.sourceforge.jp/en/\">Julius Homepage</a>)<li>BZip2<br />(Erhältlich von der <a href=\"http://www.bzip.org/\">Homepage</a>)<li>Diverse Sprachmodell Skripte und Konfigurationsdateien<br />Halten Sie diese für die anschliesende Konfiguration bereit. Für die Englische Sprache erhalten Sie die Dateien zum Beispiel vom GPL Sprachmodell voxforge (<a href=\"http://www.voxforge.org\">Voxforge Homepage</a>). Im Zweifelsfall wenden Sie sich bitte an den simon support (<a href=\"mailto:support@simon-listens.org\">support@simon-listens.org</a>) oder besuchen Sie das <a href=\"http://www.simon-listens.org/wiki/\">Wiki</a> (im Aufbau)</ul><p>Bitte installieren Sie diese, bevor Sie hier fortfahren.</p></body></html>"));

	return checklist;
}



QWizardPage* FirstRunWizard::createNewModelDescription()
{
	QWizardPage *newModel = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(newModel);
	QLabel *desc = new QLabel(newModel);
	desc->setOpenExternalLinks(true);
	lay->addWidget(desc);
	newModel->setLayout(lay);

	desc->setWordWrap(true);
	newModel->setTitle(tr("Neues Sprachmodell"));
	desc->setText(tr("<html><head /><body><p>Sie haben ausgewählt ein neues Sprachmodell zu erstellen.</p><p>Bitte bedenken Sie, dass sie dafür trozdem noch einige Skripte benötigen die von simon möglicherweise nicht mitgeliefert werden.</p><p>Für den nächsten Schritt brauchen Sie gültige, teilweise Ihrer Sprache entsprechenden, Exemplare der Dateien:<br />wav_config (wenn Sie simon zum Training verwenden können Sie bedenkenlos die voxforge-Version der Datei verwenden), tree1.hed (diese Datei muss auf Ihre Sprache, in diesem Fall Deutsch, angepasst sein), config, proto, mkphones0.led, mkphones1.led, global.ded, mktri.led<br /><b>Sie können die meisten dieser Dateien aus einem aktuellen Voxforge-Snapshot entnehmen. (<a href=\"http://www.voxforge.org\">Voxforge Homepage</a>)</b></p><p>Außerdem benötigen Sie <i>leere</i> prompts, lexicon, shadow-lexicon, vocab, shadow-vocab und grammar Dateien, die Sie im nächsten Schritt angeben müssen.</p><p>Eine ausführliche Anleitung finden Sie im <a href=\"http://www.cyber-byte.at/wiki/index.php?title=Initial_Configuration\">Wiki</a></body></html>"));

	return newModel;
}

QWizardPage* FirstRunWizard::createModelSettings()
{
	SystemWidgetPage *page = new SystemWidgetPage(this);
	page->setChild(new ModelSettings(page));
	return (QWizardPage*) page;
}


QWizardPage* FirstRunWizard::createExternalProgramsPage()
{
	SystemWidgetPage *page = new SystemWidgetPage(this);
	page->setChild(new ExternalProgramManager(page));
	return (QWizardPage*) page;
}

QWizardPage* FirstRunWizard::createLanguageModelSelectSource()
{
	return (QWizardPage*) new SelectLanguageModelSourcePage(this);
}

QWizardPage* FirstRunWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(finished);
	QLabel *desc = new QLabel(finished);
	lay->addWidget(desc);
	finished->setLayout(lay);

	desc->setWordWrap(true);
	finished->setTitle(tr("Konfiguration abgeschlossen"));
	desc->setText(tr("Die Konfiguration von simon ist hiermit abgeschlossen.\n\nSie können alle Einstellungen im System-Menü bearbeiten."));

	return finished;
}


FirstRunWizard::~FirstRunWizard()
{
}


