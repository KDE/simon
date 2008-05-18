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
#include "../SimonLib/Settings/settings.h"

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


	connect(ui.cbStartSimonOnBoot, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
	connect(ui.cbAskBeforeExit, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));

	connect(ui.leCommands, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leShortcuts, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leProgramCategories, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leTempDir, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.lePathToTexts, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));

	//set help
	help = tr("Hier können Sie grundlegende Einstellungen rund um Simon verändern.\n\nDazu zählen: Pfade und Sicherheitsabfragen.\n\nEinstellungen in diesem Modul können kritisch für die Verwendung von simon sein.\n\nWenn das Häckchen \"Starte juliusd wenn nötig\" aktiviert ist, wird versucht juliusd lokal zu starten, sollte zu keinem laufenden Dämon verbunden werden können.");
}

/**
 * \brief Determines if the page is completely configured
 * 
 * \author Peter Grasch
 * @return if we are done here
 */
bool GeneralSettings::isComplete()
{
	return ((!ui.leCommands->text().isEmpty()) && (!ui.leShortcuts->text().isEmpty()) && 
			(!ui.leProgramCategories->text().isEmpty()) &&  (!ui.leTempDir->text().isEmpty()) && 
			(!ui.lePathToTexts->text().isEmpty()));
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
	Settings::set("AskBeforeExit", ui.cbAskBeforeExit->isChecked());

	//paths
	Settings::set("PathToCommands", ui.leCommands->text());
	Settings::set("PathToShortcuts", ui.leShortcuts->text());
	Settings::set("PathToProgramCategories", ui.leProgramCategories->text());
	Settings::set("TempDir", ui.leTempDir->text());
	Settings::set("PathToTexts", ui.lePathToTexts->text());

	Settings::set("ConfigDone", !ui.pbShowFirstRunWizard->isChecked());


// #ifdef __WIN32
// 	QSettings settings;
// 	settings.setPath("Microsoft", "Windows", QSettings::UserScope);
// 	if (ui.cbStartSimonOnBoot->isChecked()) {
// 		// Want to start on boot up
// 		QString appPath = qApp->applicationFilePath();
// 		settings.writeEntry("/CurrentVersion/Run/simon.exe", appPath);
// 	} else {
// 		// Do not want to start on boot up
// 		settings.removeEntry("/CurrentVersion/Run/simon.exe");
// 	}
// #endif

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
	ui.pbShowFirstRunWizard->setChecked(!Settings::getB("ConfigDone"));

	//performance
	ui.sbMaxSimultaniouslyShownWords->setValue(Settings::getI("Performance/MaxDisplayedWords"));

	//general
	ui.cbStartSimonOnBoot->setChecked(Settings::getB("SimonAutostart"));
	ui.cbAskBeforeExit->setChecked(Settings::getB("AskBeforeExit"));


	//paths
	ui.leCommands->setText(Settings::getS("PathToCommands"));
	ui.leShortcuts->setText(Settings::getS("PathToShortcuts"));
	ui.leProgramCategories->setText(Settings::getS("PathToProgramCategories"));
	ui.leTempDir->setText(Settings::getS("TempDir"));
	ui.lePathToTexts->setText(Settings::getS("PathToTexts"));

	return true;
}





/**
 * \brief Destructor
 * \author Peter Grasch
 */
GeneralSettings::~GeneralSettings()
{
}


