//
// C++ Implementation: definedefaultvaluespage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "definedefaultvaluespage.h"
#include "../../SimonLib/Settings/settings.h"
#include <QCoreApplication>
#include <QTimer>

/**
 * \brief Constructor - sets up the ui
 * \author Peter Grasch
 * @param parent Sets the parent to the given parent
 */
DefineDefaultValuesPage::DefineDefaultValuesPage ( QWidget* parent ) : QWizardPage ( parent )
{
	setTitle("Setze Standardwerte...");
	this->complete=false;
	ui.setupUi(this);
	this->currentProgress=0;
}


/**
 * \brief Retruns the member complete
 * \author Peter Grasch
 * @return this is set if all values are set
 */
bool DefineDefaultValuesPage::isComplete() const
{
	return this->complete;
}


/**
 * \brief Initializes the values with default settings (if they are not already set)
 * \author Peter Grasch
 */
void DefineDefaultValuesPage::initializePage()
{
	QWizardPage::initializePage();

	this->complete=false;

	QString applicationPath = QCoreApplication::applicationDirPath()+"/";
// 	QString applicationPath = "."; //define relative configuration
	
	ui.pbProgress->setValue(0);
	
	Settings::checkAndSet("AskBeforeExit", true);
	Settings::checkAndSet("PathToCommands", applicationPath+"conf/commands.xml");
	Settings::checkAndSet("PathToProgramCategories", applicationPath+"conf/categories.xml");
	Settings::checkAndSet("PathToShortcuts", applicationPath+"conf/shortcuts.xml");
	Settings::checkAndSet("PathToTexts", applicationPath+"texts/");
	Settings::checkAndSet("SimonAutostart", false);
	Settings::checkAndSet("TempDir", applicationPath+"tmp/");
	Settings::checkAndSet("Desktopgrid/RealTransparency", false);
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
	Settings::checkAndSet("Commands/Keyword", "simon");
	Settings::checkAndSet("Desktopgrid/Trigger", "Desktopgitter");
	Settings::checkAndSet("Internet/TextOnlineUpdate", "http://simon.pytalhost.org/texts/list.xml");
	Settings::checkAndSet("Internet/WikiDumpOverview", "http://download.wikimedia.org/backup-index.html");
	Settings::checkAndSet("Internet/WikiDumpPrefix", "http://download.wikimedia.org/");
	Settings::checkAndSet("Internet/WikiDumpPostfix", "-pages-articles.xml.bz2");

	ui.pbProgress->setValue(ui.pbProgress->maximum());
	QCoreApplication::processEvents();

	complete = true;
	QTimer::singleShot(100, this, SIGNAL(done()));
}

