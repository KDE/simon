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
#include "settings.h"
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
 * \brief Checks if the value is already set (it is not null) and sets it if it isn't
 *
 * @param option The setting to change
 * @param value The new value
 * \return true, if we changed anything
 */
bool DefineDefaultValuesPage::checkAndSet(QString option, QVariant value)
{
	bool changed=false;
	if (Settings::get(option).isNull())
	{
		Settings::set(option, value);
		changed = true;
	}
	ui.pbProgress->setValue(++currentProgress);
	QCoreApplication::processEvents();
	return changed;
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
	
	ui.pbProgress->setValue(0);
	
	checkAndSet("AskBeforeExit", true);
	checkAndSet("PathToCommands", applicationPath+"conf/commands.xml");
	checkAndSet("PathToProgramCategories", applicationPath+"conf/categories.xml");
	checkAndSet("PathToShortcuts", applicationPath+"conf/shortcuts.xml");
	checkAndSet("PathToTexts", applicationPath+"texts/");
	checkAndSet("SimonAutostart", false);
	checkAndSet("TempDir", applicationPath+"tmp/");
	checkAndSet("Desktopgrid/RealTransparency", false);
	checkAndSet("Performance/MaxDisplayedWords", 500);
	checkAndSet("Model/Samplerate", 16000);
	checkAndSet("Model/Channels", 1);
	checkAndSet("Model/PathToLexicon", applicationPath+"model/lexicon");
	checkAndSet("Model/PathToGrammar", applicationPath+"model/model.grammar");
	checkAndSet("Model/PathToPrompts", applicationPath+"model/prompts");
	checkAndSet("Model/PathToVocab", applicationPath+"model/model.voca");
	checkAndSet("Model/PathToWavConfig", applicationPath+"model/wav_config");
	checkAndSet("Model/PathToConfig", applicationPath+"model/scripts/config");
	checkAndSet("Model/PathToProto", applicationPath+"model/scripts/proto");
	checkAndSet("Model/PathToTreeHed", applicationPath+"model/tree1.hed");
	checkAndSet("Model/PathToMkPhones0", applicationPath+"model/scripts/mkphones0.led");
	checkAndSet("Model/PathToMkPhones1", applicationPath+"model/scripts/mkphones1.led");
	checkAndSet("Model/PathToSamples", applicationPath+"model/training.data");
	checkAndSet("Model/PathToGlobalDed", applicationPath+"model/scripts/global.ded");
	checkAndSet("Model/PathToSilHed", applicationPath+"model/scripts/sil.hed");
	checkAndSet("Model/PathToMktriLed", applicationPath+"model/scripts/mktri.led");
	checkAndSet("Model/PathToHmm", applicationPath+"model/hmmdefs");
	checkAndSet("Model/PathToTiedlist", applicationPath+"model/tiedlist");
	checkAndSet("Model/PathToShadowLexicon", applicationPath+"model/shadowlexicon");
	checkAndSet("Model/PathToShadowVocab", applicationPath+"model/shadow.voca");
	checkAndSet("Model/PathToDict", applicationPath+"model/model.dict");
	checkAndSet("Model/PathToDfa", applicationPath+"model/model.dfa");
	checkAndSet("Model/PathToPhonemes", "model/phonemes");
	checkAndSet("Programs/HTK/HDMan", "HDMan");
	checkAndSet("Programs/HTK/HLEd", "HLEd");
	checkAndSet("Programs/HTK/HCopy", "HCopy");
	checkAndSet("Programs/HTK/HCompV", "HCompV");
	checkAndSet("Programs/HTK/HERest", "HERest");
	checkAndSet("Programs/HTK/HHEd", "HHEd");
	checkAndSet("Programs/HTK/HVite", "HVite");
	checkAndSet("Programs/Files/BZip2", "bzip2");
	checkAndSet("Programs/Julius/mkfa", "mkfa");
	checkAndSet("Programs/Julius/dfa_minimize", "dfa_minimize");
	checkAndSet("Juliusd/AutoConnect", false);
	checkAndSet("Network/Timeout", 3000);
	checkAndSet("Internet/TextOnlineUpdate", "http://simon.pytalhost.org/texts/list.xml");
	checkAndSet("Internet/WikiDumpOverview", "http://download.wikimedia.org/backup-index.html");
	checkAndSet("Internet/WikiDumpPrefix", "http://download.wikimedia.org/");
	checkAndSet("Internet/WikiDumpPostfix", "-pages-articles.xml.bz2");

	complete = true;
	QTimer::singleShot(100, this, SIGNAL(done()));
}

