//
// C++ Implementation: generalsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "generalsettings.h"
#include <QInputDialog>
#include <QVariant>
#include "settings.h"

/**
 * \brief Constructs a new GeneralSettings object
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
GeneralSettings::GeneralSettings(QWidget* parent): SystemWidget(tr("Allgemeine Einstellungen"), QIcon(":/images/icons/computer.svg"), tr("Grundlegende Einstellungen rund um Simon"), parent)
{
	ui.setupUi(this);
	hide();
	
	//set help
	help = tr("Hier können Sie grundlegende Einstellungen rund um Simon verändern.\n\nDazu zählen: Pfade und Sicherheitsabfragen.\n\nEinstellungen in diesem Modul können kritisch für die Verwendung von simon sein.\n\nWenn das Häckchen \"Starte juliusd wenn nötig\" aktiviert ist, wird versucht juliusd lokal zu starten, sollte zu keinem laufenden Dämon verbunden werden können.");
}


/**
 * \brief Applys the changed settings
 * \author Peter Grasch
 * @return Success
 */
bool GeneralSettings::apply()
{
	//general
	Settings::set("SimonAutostart", ui.cbStartSimonOnBoot->isChecked());
	Settings::set("StartJuliusdWhenRequired", ui.cbStartJuliusAsNeeded->isChecked());
	Settings::set("AskBeforeExit", ui.cbAskBeforeExit->isChecked());
	Settings::set("AutoConnect", ui.cbAutoConnect->isChecked());

	//password
	Settings::set("Password", ui.lePassword->text());

	//paths
	Settings::set("PathToCommands", ui.leCommands->text());
	Settings::set("PathToShortcuts", ui.leShortcuts->text());
	Settings::set("PathToProgramCategories", ui.leProgramCategories->text());
	Settings::set("PathToCommands", ui.leCommands->text());
	Settings::set("PathToLexicon", ui.leLexicon->text());
	Settings::set("PathToGrammar", ui.leGrammar->text());
	Settings::set("PathToPrompts", ui.lePrompts->text());
	Settings::set("PathToVocab", ui.leVocab->text());

	return true;
}


/**
 * \brief Resets the settings
 * \author Peter Grasch
 * @return Success
 */
bool GeneralSettings::reset()
{
	return init();
}


/**
 * \brief Initialy loads the settings
 * \author Peter Grasch
 * @return Success
 */
bool GeneralSettings::init()
{
	//general
	ui.cbStartSimonOnBoot->setChecked(Settings::get("SimonAutostart").toBool());
	ui.cbStartJuliusAsNeeded->setChecked(Settings::get("StartJuliusdWhenRequired").toBool());
	ui.cbAskBeforeExit->setChecked(Settings::get("AskBeforeExit").toBool());
	ui.cbAutoConnect->setChecked(Settings::get("AutoConnect").toBool());

	//password
	ui.lePassword->setText(Settings::get("Password").toString());

	//paths
	ui.leCommands->setText(Settings::get("PathToCommands").toString());
	ui.leShortcuts->setText(Settings::get("PathToShortcuts").toString());
	ui.leProgramCategories->setText(Settings::get("PathToProgramCategories").toString());
	ui.leLexicon->setText(Settings::get("PathToLexicon").toString());
	ui.leGrammar->setText(Settings::get("PathToGrammar").toString());
	ui.lePrompts->setText(Settings::get("PathToPrompts").toString());
	ui.leVocab->setText(Settings::get("PathToVocab").toString());

	return true;
}





/**
 * \brief Destructor
 * \author Peter Grasch
 */
GeneralSettings::~GeneralSettings()
{
}


