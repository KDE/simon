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
#include "firstrunwizard.h"

#include <QWizardPage>
#include "portaudio.h"
#include "../../SimonLib/Settings/settings.h"
#include "../externalprogrammanager.h"
#include "systemwidgetpage.h"
#include "selectlanguagemodelsourcepage.h"

#include "firstrunmodelsettings.h"

#include "firstrunimportdictselectsourcepage.h"
#include "firstrunimportbomppage.h"

#include "../../ModelManagement/WordList/ImportDict/importlexiconpage.h"
#include "firstrunimportdictwiktionarypage.h"
#include "../../ModelManagement/WordList/ImportDict/importdictworkingpage.h"


#include "../../ModelManagement/Grammar/grammarsettings.h"
#include "../../ModelManagement/WordList/wordlistmanager.h"

#include "../../SimonLib/Sound/soundsettings.h"
#include "../../RecognitionControl/networksettings.h"
#include "../passwordsettings.h"
#include "../../Actions/Commands/commandsettings.h"


/**
 * \brief Constructor - adds all the pages and initializes the wordlistmanager and trainingmanager with NULL
 * \author Peter Grasch
 * @param parent Parent of the wizard
 */
FirstRunWizard::FirstRunWizard(QWidget* parent): QWizard(parent)
{
	setWindowTitle(i18n("simon Erstkonfiguration"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/firstrun.png"));
	addPage(createIntroPage());
	addPage(createCheckList());
	addPage(createExternalProgramsPage());
	addPage(createLanguageModelDescription());
	addPage(createLanguageModelSelectSource());
	addPage(createNewModelDescription());
	addPage(createModelSettings());

	addPage(createImportDictSelectTypePage());
	addPage(createImportDictBOMPPage());
	addPage(createImportDictWiktionaryPage());
	addPage(createImportDictLexiconPage());
	addPage(createImportDictWorkingPage());

	addPage(createGrammarDescriptionPage());
	addPage(createGrammarSettings());

	addPage(createSoundSettingsPage());

	addPage(createPasswordDescriptionPage());
	addPage(createPasswordSettingsPage());

	addPage(createJuliusdDescriptionPage());
	addPage(createJuliusdSettingsPage());

	addPage(createConfigureCommandsPage());

	addPage(createFinishedPage());

	initDefaultValues();
}

/**
 * \brief Creates the wizardpage to configure the commands
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createConfigureCommandsPage()
{
	SystemWidgetPage *comSettings = new SystemWidgetPage(this);
	comSettings->setChild(new CommandSettings(comSettings));
	return comSettings;
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
	WordListManager::getInstance()->addWords(words, true, true);
	next();
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
	ImportDictWorkingPage *importDictWorkingPage = new ImportDictWorkingPage(this);
	connect(importDictWorkingPage, SIGNAL(wordListImported(WordList*)), this, SLOT(importDict(WordList*)));
	return (QWizardPage*) importDictWorkingPage;
}


/**
 * \brief Creates the wizardpage to display some information about the grammar-settings-page that is about to be shown
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createGrammarDescriptionPage()
{
	QWizardPage *description = new QWizardPage(this);
	QHBoxLayout *lay = new QHBoxLayout(description);
	QLabel *desc = new QLabel(description);
	lay->addWidget(desc);
	description->setLayout(lay);

	desc->setWordWrap(true);
	description->setTitle(i18n("Über die Grammatik"));
	desc->setText(i18n("<html><head /><body><p>simon kann eine Grammatik verwenden um die Erkennungsrate zu erhöhen. Mit ihr können Sie Satzkonstrukte (wie zum Beispiel \"Nomen Verb Nomen\") definieren.</p><p>simon kann dabei sogar aus vorhandenen Texten ihren persönlichen Schreibstil lernen.</p><p>Bitte definieren Sie jetzt ihre Grammatik oder lassen Sie simon diese mithilfe einiger, am besten von ihnen erfassten Texten erlernen (ein großes Schattenlexikon vorrausgesetzt).</p></body></html>"));

	return description;
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
	description->setTitle(i18n("Über den Passwortschutz"));
	desc->setText(i18n("<html><head /><body><p>simon unterscheidet generell zwischen normaler Benutzung (dies umfasst das Trainieren, Hinzufügen von Wörtern, durchsuchen der Wortliste, etc.) und einem \"Systemverwaltungsmodus\". Dieser umfasst zum Beispiel die Systemeinstellungen.</p><p>Der Systemverwaltungsmodus kann mit einem Passwort gegen unlauteren Zugriff gesperrt werden. </p></body></html>"));

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
	description->setTitle(i18n("Über Juliusd"));
	desc->setText(i18n("<html><head /><body><p>Das simon-System besteht eigentlich aus zwei Teilen:</p><ul><li>simon (client; Dieses Programm)<li>juliusd (server, verwaltet julius)</ul><p>Der juliusd kann beliebig viele simon-clients gleichzeitig bedienen.</p><p>Meist wird der Juliusd auf einem zentralen Server ausgeführt werden, zu dem alle Clients verbinden. Es ist jedoch eine Einzelinstallation möglich. Starten Sie dafür Juliusd lokal und geben auf der nächsten Seite als Adresse ihre Loopbackadresse (meist 127.0.0.1 / localhost) und den konfigurierten Port (standardmäsig 4444) an.</p><p>Nähere Informationen zu Juliusd finden Sie im Handbuch</p></body></html>"));

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
 * \brief Creates the wizardpage to configure the grammar
 * \author Peter Grasch
 * @return The page
 */
QWizardPage* FirstRunWizard::createGrammarSettings()
{
	SystemWidgetPage *grammarSettingsPage = new SystemWidgetPage(this);

	GrammarSettings *grammarSettings = new GrammarSettings(grammarSettingsPage);
	grammarSettingsPage->setChild(grammarSettings);

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
	description->setTitle(i18n("Das Sprachmodell"));
	desc->setText(i18n("<html><head /><body><p>Sie benötigen die folgenden Einzelteile:</p><ul><li>(Schatten-) Wörterbuch, (Schatten-) Vokabeln, Grammatik und Trainingsdaten (pfad, prompts, wav_config)<br />Personenbezogen<li>Sprachskripte (tree1.hed)<br />Sprachbezogen<li>Modellskripte (sil.hed, config, proto, mkphones0.led und -1.led, global.ded, mktri.led)<br />Generell</ul><p>Aus diesen Eingabedaten werden dann folgende Dateien erstellt:<ul><li>hmmdefs<br />Beinhaltet eine technische Repräsentation der Stimme<li>tiedlist<br />Eine Liste der verwendeten Lautkombinationen<li>dict / dfa<br />Eine von julian lesbare Form der Grammatik</body></html>"));

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
	intro->setTitle(i18n("Willkommen bei simon"));
	desc->setText(i18n("Dieser Assistent wird Ihnen helfen, simon zu konfigurieren.\n\nAlle Einstellungen die Sie in diesem Assistenten treffen können später über das Systemmenü wieder geändert werden. Bitte beachten Sie, das die Einstellungen hier essientiell für die Funktionstüchtigkeit von simon sind.\n\nSollten Sie sich bei einigen Punkten nicht sicher sein, sehen Sie bitte im Handbuch nach oder fragen Sie gegebenenfalls ihren Systemadministrator."));

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
	checklist->setTitle(i18n("Checkliste"));
	desc->setText(i18n("<html><head/><body><p>Um simon erfolgreich zu benutzen werden einige externe Programme und Dateien benötigt.</p><ul><li>HTK Toolkit<br />(Erhältlich von der <a href=\"http://htk.eng.cam.ac.uk/\">Universität Cambridge</a>)<li>Julius<br />(Erhältlich von der offiziellen <a href=\"http://julius.sourceforge.jp/en/\">Julius Homepage</a>)<li>BZip2<br />(Erhältlich von der <a href=\"http://www.bzip.org/\">Homepage</a>)<li>Diverse Sprachmodell Skripte und Konfigurationsdateien<br />Für die Englische Sprache erhalten Sie die Dateien zum Beispiel vom GPL Sprachmodell voxforge (<a href=\"http://www.voxforge.org\">Voxforge Homepage</a>). Wenn sie selbst ein Sprachmodell erstellen wollen, können Sie einfach die mit simon ausgelieferten Default-Dateien verwenden (sie sind als Standardwerte in den Modelleinstellungen ausgewählt). Im Zweifelsfall wenden Sie sich bitte an den simon support (<a href=\"mailto:support@simon-listens.org\">support@simon-listens.org</a>) oder besuchen Sie das <a href=\"http://www.simon-listens.org/wiki/\">Wiki</a> (im Aufbau)</ul><p>Bitte installieren Sie diese, bevor Sie hier fortfahren.</p></body></html>"));

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
	newModel->setTitle(i18n("Neues Sprachmodell"));
	desc->setText(i18n("<html><head /><body><p>Sie haben ausgewählt ein neues Sprachmodell zu erstellen.</p><p>Von den folgenden Dateien finden sie Standard-Dateien im model-Ordner.</p><p>Für den nächsten Schritt brauchen Sie gültige, teilweise Ihrer Sprache entsprechenden, Exemplare der Dateien:<br />wav_config (wenn Sie simon zum Training verwenden können Sie bedenkenlos die voxforge-Version der Datei verwenden), tree1.hed (diese Datei muss auf Ihre Sprache, in diesem Fall Deutsch, angepasst sein), config, proto, mkphones0.led, mkphones1.led, global.ded, mktri.led<br /><b>Sie können die meisten dieser Dateien aus einem aktuellen Voxforge-Snapshot entnehmen. (<a href=\"http://www.voxforge.org\">Voxforge Homepage</a>)</b></p><p>Außerdem benötigen Sie <i>leere</i> prompts, lexicon, shadow-lexicon, vocab, shadow-vocab und grammar Dateien, die Sie im nächsten Schritt angeben müssen.</p><p>Eine ausführliche Anleitung finden Sie im <a href=\"http://www.cyber-byte.at/wiki/index.php?title=Initial_Configuration\">Wiki</a></body></html>"));

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
	finished->setTitle(i18n("Konfiguration abgeschlossen"));
	desc->setText(i18n("Die Konfiguration von simon ist hiermit abgeschlossen.\n\nSie können alle Einstellungen im System-Menü bearbeiten.\n\nSollten Sie soeben ein neues Sprachmodell erstellt haben, müssen Sie diese vor der ersten Verwendung trainieren. Selektieren Sie hierzu den Menüpunkt \"Training\".\n\nViel Spaß mit simon!"));

	return finished;
}


void FirstRunWizard::initDefaultValues()
{
	QString applicationPath = QCoreApplication::applicationDirPath()+"/";
// 	QString applicationPath = "./"; //define relative configuration
	
	Settings::checkAndSet("AskBeforeExit", true);
	Settings::checkAndSet("PathToCommands", applicationPath+"conf/commands.xml");
	Settings::checkAndSet("PathToProgramCategories", applicationPath+"conf/categories.xml");
	Settings::checkAndSet("PathToShortcuts", applicationPath+"conf/shortcuts.xml");
	Settings::checkAndSet("PathToTexts", applicationPath+"texts/");
	Settings::checkAndSet("SimonAutostart", false);
	Settings::checkAndSet("TempDir", applicationPath+"tmp/");
	Settings::checkAndSet("Performance/MaxDisplayedWords", 500);
	Settings::checkAndSet("Model/Samplerate", 16000);
	Settings::checkAndSet("Model/Channels", 1);
	Settings::checkAndSet("Model/PathToLexicon", applicationPath+"model/lexicon");
	Settings::checkAndSet("Model/PathToGrammar", applicationPath+"model/model.grammar");
	Settings::checkAndSet("Model/PathToPrompts", applicationPath+"model/prompts");
	Settings::checkAndSet("Model/PathToVocab", applicationPath+"model/model.voca");
	Settings::checkAndSet("Model/PathToWavConfig", applicationPath+"model/wav_config");
	Settings::checkAndSet("Model/PathToConfig", applicationPath+"model/scripts/config");
	Settings::checkAndSet("Model/PathToProto", applicationPath+"model/scripts/proto");
	Settings::checkAndSet("Model/PathToTreeHed", applicationPath+"model/tree1.hed");
	Settings::checkAndSet("Model/PathToMkPhones0", applicationPath+"model/scripts/mkphones0.led");
	Settings::checkAndSet("Model/PathToMkPhones1", applicationPath+"model/scripts/mkphones1.led");
	Settings::checkAndSet("Model/PathToSamples", applicationPath+"model/training.data");
	Settings::checkAndSet("Model/PathToGlobalDed", applicationPath+"model/scripts/global.ded");
	Settings::checkAndSet("Model/PathToSilHed", applicationPath+"model/scripts/sil.hed");
	Settings::checkAndSet("Model/PathToMktriLed", applicationPath+"model/scripts/mktri.led");
	Settings::checkAndSet("Model/PathToHmm", applicationPath+"model/hmmdefs");
	Settings::checkAndSet("Model/PathToTiedlist", applicationPath+"model/tiedlist");
	Settings::checkAndSet("Model/PathToShadowLexicon", applicationPath+"model/shadowlexicon");
	Settings::checkAndSet("Model/PathToShadowVocab", applicationPath+"model/shadow.voca");
	Settings::checkAndSet("Model/PathToDict", applicationPath+"model/model.dict");
	Settings::checkAndSet("Model/PathToDfa", applicationPath+"model/model.dfa");
	Settings::checkAndSet("Model/PathToPhonemes", "model/phonemes");
	Settings::checkAndSet("Programs/HTK/HDMan", "HDMan");
	Settings::checkAndSet("Programs/HTK/HLEd", "HLEd");
	Settings::checkAndSet("Programs/HTK/HCopy", "HCopy");
	Settings::checkAndSet("Programs/HTK/HCompV", "HCompV");
	Settings::checkAndSet("Programs/HTK/HERest", "HERest");
	Settings::checkAndSet("Programs/HTK/HHEd", "HHEd");
	Settings::checkAndSet("Programs/HTK/HVite", "HVite");
	Settings::checkAndSet("Programs/Files/BZip2", "bzip2");
	Settings::checkAndSet("Programs/Julius/mkfa", "mkfa");
	Settings::checkAndSet("Programs/Julius/dfa_minimize", "dfa_minimize");
	Settings::checkAndSet("Juliusd/AutoConnect", false);
	Settings::checkAndSet("Network/Timeout", 3000);
	Settings::checkAndSet("Network/JuliusdServers", "localhost:4444;");
	Settings::checkAndSet("Internet/TextOnlineUpdate", "http://simon.pytalhost.org/texts/list.xml");
	Settings::checkAndSet("Internet/WikiDumpOverview", "http://download.wikimedia.org/backup-index.html");
	Settings::checkAndSet("Internet/WikiDumpPrefix", "http://download.wikimedia.org/");
	Settings::checkAndSet("Internet/WikiDumpPostfix", "-pages-articles.xml.bz2");


	Settings::checkAndSet("Commands/Dictation", true);
	Settings::checkAndSet("Commands/UseGlobalTrigger", true);
	Settings::checkAndSet("Commands/Trigger", "simon");
	Settings::checkAndSet("Commands/Executable/Enabled", true);
	Settings::checkAndSet("Commands/Executable/PathToConfig", applicationPath+"conf/executables.xml");

	Settings::checkAndSet("Commands/Place/Enabled", true);
	Settings::checkAndSet("Commands/Place/PathToConfig", applicationPath+"conf/places.xml");

	Settings::checkAndSet("Commands/TextMacro/Enabled", true);
	Settings::checkAndSet("Commands/TextMacro/PathToConfig", applicationPath+"conf/textmacros.xml");

	Settings::checkAndSet("Commands/Shortcut/Enabled", true);
	Settings::checkAndSet("Commands/Shortcut/PathToConfig", applicationPath+"conf/shortcuts.xml");

	Settings::checkAndSet("Commands/DesktopGrid/Enabled", true);
	Settings::checkAndSet("Commands/DesktopGrid/Trigger", i18n("Desktopgitter"));
	Settings::checkAndSet("Commands/DesktopGrid/RealTransparency", false);

	Settings::checkAndSet("Commands/ATIntegration/Enabled", false);
	Settings::checkAndSet("Commands/ATIntegration/QtWorkarounds", true);
	Settings::checkAndSet("Commands/ATIntegration/QtMenuSupport", true);

	if (Pa_Initialize() != paNoError)
	{
		Settings::checkAndSet("Sound/InputDevice", Pa_GetDefaultInputDevice());
		Settings::checkAndSet("Sound/OutputDevice", Pa_GetDefaultOutputDevice());
		Pa_Terminate();
	} else {
		Settings::checkAndSet("Sound/InputDevice", 0);
		Settings::checkAndSet("Sound/OutputDevice", 0);
	}
	Settings::checkAndSet("Sound/Channels", 1);
	Settings::checkAndSet("Sound/Samplerate", 16000);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
FirstRunWizard::~FirstRunWizard()
{
}
