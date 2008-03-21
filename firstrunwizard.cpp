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

#include "firstrunmodelsettings.h"
#include "firstruncreatedictionarypage.h"

#include "firstrunimportdictselectsourcepage.h"
#include "firstrunimportbomppage.h"
#include "importlexiconpage.h"
#include "firstrunimportdictwiktionarypage.h"
#include "importdictworkingpage.h"


#include "firstrunimportgrammardescriptionpage.h"
#include "importgrammarselectfilespage.h"
#include "firstrunimportgrammarworkingpage.h"

#include "grammarmanager.h"
#include "grammarsettings.h"

#include "wordlistmanager.h"

#include "soundsettings.h"
#include "networksettings.h"
#include "passwordsettings.h"
#include "configuretriggerspage.h"
#include "definedefaultvaluespage.h"
#include "addwordview.h"


/**
 * \brief Constructor - adds all the pages and initializes the wordlistmanager and trainingmanager with NULL
 * \author Peter Grasch
 * @param addWordView The AddWordView to pass on 
 * @param parent Parent of the wizard
 */
FirstRunWizard::FirstRunWizard(QWidget* parent): QWizard(parent)
{
	this->wordListManager=0;
	this->trainingManager=0;
	this->grammarManager=0;
	firstRunImportGrammarWorkingPage=0;
	importDictWorkingPage=0;
	grammarSettingsPage=0;


	setWindowTitle(tr("simon Erstkonfiguration"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/firstrun.png"));
	addPage(createIntroPage());
	addPage(createDefineDefaultValuesPage());
	addPage(createCheckList());
	addPage(createExternalProgramsPage());
	addPage(createLanguageModelDescription());
	addPage(createLanguageModelSelectSource());
	addPage(createNewModelDescription());
	addPage(createModelSettings());

	addPage(createCreateDictionaryPage());

	addPage(createImportDictSelectTypePage());
	addPage(createImportDictBOMPPage());
	addPage(createImportDictWiktionaryPage());
	addPage(createImportDictLexiconPage());
	addPage(createImportDictWorkingPage());

	addPage(createGrammarDescriptionPage());
	addPage(createSelectFilesPage());
	addPage(createGrammarWorkingPage());
	addPage(createGrammarSettings());

	addPage(createSoundSettingsPage());

	addPage(createPasswordDescriptionPage());
	addPage(createPasswordSettingsPage());

	addPage(createJuliusdDescriptionPage());
	addPage(createJuliusdSettingsPage());

	addPage(createConfigureTriggersPage());

	addPage(createFinishedPage());
}

/**
 * \brief Creates the wizardpage to configure the action triggers
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createConfigureTriggersPage()
{
	return new ConfigureTriggersPage(this);
}

/**
 * \brief Sets the WordListManager to use to the given one;
 * 
 * If we already set a WLM. we delete it.
 * 
 * Passes on the given WordListManager to:
 * 	* FirstRunImportGrammarWorkingPage
 *
 * Creates a new GrammarSettings and sets the grammarSettingsPages child to the newly generated GrammarSettings object
 *
 * @param wordListManager 
 */
void FirstRunWizard::setWordListManager(WordListManager *wordListManager)
{
	if (this->wordListManager) delete this->wordListManager;

	this->wordListManager = wordListManager;
	
	connect(importDictWorkingPage, SIGNAL(wordListImported(WordList*)), this, SLOT(importDict(WordList*)));

	this->grammarManager = GrammarManager::getInstance();
	
	GrammarSettings *grammarSettings = new GrammarSettings(grammarSettingsPage);
	connect(firstRunImportGrammarWorkingPage, SIGNAL(grammarCreated(QStringList)), 
		grammarSettings, SLOT(mergeGrammar(QStringList)));

	grammarSettingsPage->setChild(grammarSettings);
}

/**
 * \brief Imports the given WordList
 * \author Peter Grasch
 * 
 * Adds the (sorted) list of words to the shadowlist
 * 
 * @param words The words to add - THEY HAVE TO BE SORTED! If in doubt, use qSort()
 */
void FirstRunWizard::importDict(WordList *words)
{
	if (this->wordListManager)
	{
		this->wordListManager->addWords(words, true, true);
	}
	next();
}


/**
 * \brief Creates the wizardpage to configure the define default values
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createDefineDefaultValuesPage()
{
	DefineDefaultValuesPage *page = new DefineDefaultValuesPage(this);
	connect(page, SIGNAL(done()), this, SLOT(next()));
	return page;
}


/**
 * \brief Sets the Trainingmanager to the given one
 * 
 * If there is already a TrainingManager set, it is deleted and replaced.
 * 
 * \author Peter Grasch
 * @param trainingManager The Trainingmanager to set to
 */
void FirstRunWizard::setTrainingManager(TrainingManager *trainingManager)
{
	if (this->trainingManager) delete this->trainingManager;

	this->trainingManager = trainingManager;
}


/**
 * \brief Creates the wizardpage to create the WordListManager and TrainingManager with the newly set configuration
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createCreateDictionaryPage()
{
	FirstRunCreateDictionaryPage *page = new FirstRunCreateDictionaryPage(this);
	connect(page, SIGNAL(wordListManagerCreated(WordListManager*)), this, SLOT(setWordListManager(WordListManager*)));
	connect(page, SIGNAL(trainingManagerCreated(TrainingManager*)), this, SLOT(setTrainingManager(TrainingManager*)));
	connect(page, SIGNAL(done()), this, SLOT(next()));
	return page;
}


/**
 * \brief Creates the wizardpage to select the type of the dictionary to import
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createImportDictSelectTypePage()
{
	return (QWizardPage*) new FirstRunImportDictSelectSourcePage(this);
}

/**
 * \brief Creates the wizardpage to import a bomp dictionary
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createImportDictBOMPPage()
{
	return (QWizardPage*) new FirstRunImportBOMPPage(this);
}

/**
 * \brief Creates the importwiktionarypage
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createImportDictWiktionaryPage()
{
	return (QWizardPage*) new FirstRunImportDictWiktionaryPage(this);
}

/**
 * \brief Creates the importlexiconpage
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createImportDictLexiconPage()
{
	return (QWizardPage*) new ImportLexiconPage(this);
}

/**
 * \brief Creates the wizardpage to configure import the dictionary (the working-page)
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createImportDictWorkingPage()
{
	this->importDictWorkingPage = new ImportDictWorkingPage(this);
	return (QWizardPage*) importDictWorkingPage;
}


/**
 * \brief Creates the wizardpage to display some information about the grammar-settings-page that is about to be shown
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createGrammarDescriptionPage()
{
	return new FirstRunImportGrammarDescriptionPage(this);
}



/**
 * \brief Creates the wizardpage to configure the sound
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createSoundSettingsPage()
{
	SystemWidgetPage *sound = new SystemWidgetPage(this);
	sound->setChild(new SoundSettings(sound));
	return sound;
}

/**
 * \brief Creates the wizardpage to display some information about the password configuration
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createPasswordDescriptionPage()
{
	QWizardPage *description = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(description);
	QLabel *desc = new QLabel(description);
	lay->addWidget(desc);
	description->setLayout(lay);

	desc->setWordWrap(true);
	description->setTitle(tr("Über den Passwortschutz"));
	desc->setText(tr("<html><head /><body><p>simon unterscheidet generell zwischen normaler Benutzung (dies umfasst das Trainieren, Hinzufügen von Wörtern, durchsuchen der Wortliste, etc.) und einem \"Systemverwaltungsmodus\". Dieser umfasst zum Beispiel die Systemeinstellungen.</p><p>Der Systemverwaltungsmodus kann mit einem Passwort gegen unlauteren Zugriff gesperrt werden. </p></body></html>"));

	return description;
}

/**
 * \brief Creates the wizardpage to configure the password
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createPasswordSettingsPage()
{
	SystemWidgetPage *passpg = new SystemWidgetPage(this);
	passpg->setChild(new PasswordSettings(this));
	return passpg;
}

/**
 * \brief Creates the wizardpage to display some information about juliusd
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createJuliusdDescriptionPage()
{
	QWizardPage *description = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(description);
	QLabel *desc = new QLabel(description);
	lay->addWidget(desc);
	description->setLayout(lay);

	desc->setWordWrap(true);
	description->setTitle(tr("Über Juliusd"));
	desc->setText(tr("<html><head /><body><p>Das simon-System besteht eigentlich aus zwei Teilen:</p><ul><li>simon (client; Dieses Programm)<li>juliusd (server, verwaltet julius)</ul><p>Der juliusd kann beliebig viele simon-clients gleichzeitig bedienen.</p><p>Meist wird der Juliusd auf einem zentralen Server ausgeführt werden, zu dem alle Clients verbinden. Es ist jedoch eine Einzelinstallation möglich. Starten Sie dafür Juliusd lokal und geben auf der nächsten Seite als Adresse ihre Loopbackadresse (meist 127.0.0.1 / localhost) und den konfigurierten Port (standardmäsig 4444) an.</p><p>Nähere Informationen zu Juliusd finden Sie im Handbuch</p></body></html>"));

	return description;
}

/**
 * \brief Creates the wizardpage to configure juliusd
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createJuliusdSettingsPage()
{
	SystemWidgetPage *julius = new SystemWidgetPage(this);
	julius->setChild(new NetworkSettings(julius));
	return julius;
}


/**
 * \brief Creates the wizardpage to select the files to read for the gramamr-import
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createSelectFilesPage()
{
	return new ImportGrammarSelectFilesPage(this);
}

/**
 * \brief Creates the wizardpage to import the grammar (workingpage)
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createGrammarWorkingPage()
{
	this->firstRunImportGrammarWorkingPage = new FirstRunImportGrammarWorkingPage(this);
	
	return (QWizardPage*) firstRunImportGrammarWorkingPage;
}

/**
 * \brief Creates the wizardpage to configure the grammar
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createGrammarSettings()
{
	grammarSettingsPage = new SystemWidgetPage(this);
	return (QWizardPage*) grammarSettingsPage;
	
}

/**
 * \brief Creates the wizardpage to display how to configure the language model
 * \author Peter Grasch
 * @return The page
 */
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

/**
 * \brief Creates the wizardpage to introduce the user to simon
 * \author Peter Grasch
 * @return The page
 */
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

/**
 * \brief Creates the wizardpage to display the checklist of programs / files the user needs for simon
 * \author Peter Grasch
 * @return The page
 */
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
	desc->setText(tr("<html><head/><body><p>Um simon erfolgreich zu benutzen werden einige externe Programme und Dateien benötigt.</p><ul><li>HTK Toolkit<br />(Erhältlich von der <a href=\"http://htk.eng.cam.ac.uk/\">Universität Cambridge</a>)<li>Julius<br />(Erhältlich von der offiziellen <a href=\"http://julius.sourceforge.jp/en/\">Julius Homepage</a>)<li>BZip2<br />(Erhältlich von der <a href=\"http://www.bzip.org/\">Homepage</a>)<li>Diverse Sprachmodell Skripte und Konfigurationsdateien<br />Für die Englische Sprache erhalten Sie die Dateien zum Beispiel vom GPL Sprachmodell voxforge (<a href=\"http://www.voxforge.org\">Voxforge Homepage</a>). Wenn sie selbst ein Sprachmodell erstellen wollen, können Sie einfach die mit simon ausgelieferten Default-Dateien verwenden (sie sind als Standardwerte in den Modelleinstellungen ausgewählt). Im Zweifelsfall wenden Sie sich bitte an den simon support (<a href=\"mailto:support@simon-listens.org\">support@simon-listens.org</a>) oder besuchen Sie das <a href=\"http://www.simon-listens.org/wiki/\">Wiki</a> (im Aufbau)</ul><p>Bitte installieren Sie diese, bevor Sie hier fortfahren.</p></body></html>"));

	return checklist;
}


/**
 * \brief Creates the wizardpage to inform the user about the creation process of a new language model
 * \author Peter Grasch
 * @return The page
 */
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
	desc->setText(tr("<html><head /><body><p>Sie haben ausgewählt ein neues Sprachmodell zu erstellen.</p><p>Von den folgenden Dateien finden sie Standard-Dateien im model-Ordner.</p><p>Für den nächsten Schritt brauchen Sie gültige, teilweise Ihrer Sprache entsprechenden, Exemplare der Dateien:<br />wav_config (wenn Sie simon zum Training verwenden können Sie bedenkenlos die voxforge-Version der Datei verwenden), tree1.hed (diese Datei muss auf Ihre Sprache, in diesem Fall Deutsch, angepasst sein), config, proto, mkphones0.led, mkphones1.led, global.ded, mktri.led<br /><b>Sie können die meisten dieser Dateien aus einem aktuellen Voxforge-Snapshot entnehmen. (<a href=\"http://www.voxforge.org\">Voxforge Homepage</a>)</b></p><p>Außerdem benötigen Sie <i>leere</i> prompts, lexicon, shadow-lexicon, vocab, shadow-vocab und grammar Dateien, die Sie im nächsten Schritt angeben müssen.</p><p>Eine ausführliche Anleitung finden Sie im <a href=\"http://www.cyber-byte.at/wiki/index.php?title=Initial_Configuration\">Wiki</a></body></html>"));

	return newModel;
}

/**
 * \brief Creates the wizardpage to create the modelsettings
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createModelSettings()
{
	return (QWizardPage*) new FirstRunModelSettings(this);
}


/**
 * \brief Creates the wizardpage to configure the external programs
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createExternalProgramsPage()
{
	SystemWidgetPage *page = new SystemWidgetPage(this);
	page->setChild(new ExternalProgramManager(page));
	return (QWizardPage*) page;
}

/**
 * \brief Creates the wizardpage to select the source of the language model
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createLanguageModelSelectSource()
{
	return (QWizardPage*) new SelectLanguageModelSourcePage(this);
}


/**
 * \brief Creates the wizardpage to gratulate the user for configuring simon
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(finished);
	QLabel *desc = new QLabel(finished);
	lay->addWidget(desc);
	finished->setLayout(lay);

	desc->setWordWrap(true);
	finished->setTitle(tr("Konfiguration abgeschlossen"));
	desc->setText(tr("Die Konfiguration von simon ist hiermit abgeschlossen.\n\nSie können alle Einstellungen im System-Menü bearbeiten.\n\nSollten Sie soeben ein neues Sprachmodell erstellt haben, müssen Sie diese vor der ersten Verwendung trainieren. Selektieren Sie hierzu den Menüpunkt \"Training\".\n\nViel Spaß mit simon!"));

	return finished;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
FirstRunWizard::~FirstRunWizard()
{
	if (firstRunImportGrammarWorkingPage) firstRunImportGrammarWorkingPage->deleteLater();
	if (importDictWorkingPage) importDictWorkingPage->deleteLater();
	if (grammarSettingsPage) grammarSettingsPage->deleteLater();
}


