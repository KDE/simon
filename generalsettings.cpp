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
	guessChildTriggers(this);
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
	//performance:
	Settings::set("Performance/MaxDisplayedWords", ui.sbMaxSimultaniouslyShownWords->value());
	//general
	Settings::set("SimonAutostart", ui.cbStartSimonOnBoot->isChecked());
	Settings::set("StartJuliusdWhenRequired", ui.cbStartJuliusAsNeeded->isChecked());
	Settings::set("AskBeforeExit", ui.cbAskBeforeExit->isChecked());
	Settings::set("AutoConnect", ui.cbAutoConnect->isChecked());

	//paths
	Settings::set("PathToCommands", ui.leCommands->text());
	Settings::set("PathToShortcuts", ui.leShortcuts->text());
	Settings::set("PathToProgramCategories", ui.leProgramCategories->text());
	Settings::set("PathToCommands", ui.leCommands->text());
	Settings::set("TempDir", ui.leTempDir->text());
	Settings::set("PathToTexts", ui.lePathToTexts->text());

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
	//performance
	ui.sbMaxSimultaniouslyShownWords->setValue(Settings::get("Performance/MaxDisplayedWords").toInt());

	//general
	ui.cbStartSimonOnBoot->setChecked(Settings::get("SimonAutostart").toBool());
	ui.cbStartJuliusAsNeeded->setChecked(Settings::get("StartJuliusdWhenRequired").toBool());
	ui.cbAskBeforeExit->setChecked(Settings::get("AskBeforeExit").toBool());
	ui.cbAutoConnect->setChecked(Settings::get("AutoConnect").toBool());


	//paths
	ui.leCommands->setText(Settings::get("PathToCommands").toString());
	ui.leShortcuts->setText(Settings::get("PathToShortcuts").toString());
	ui.leProgramCategories->setText(Settings::get("PathToProgramCategories").toString());
	ui.leTempDir->setText(Settings::get("TempDir").toString());
	ui.lePathToTexts->setText(Settings::get("PathToTexts").toString());

	return true;
}





/**
 * \brief Destructor
 * \author Peter Grasch
 */
GeneralSettings::~GeneralSettings()
{
}


