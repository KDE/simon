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
GeneralSettings::GeneralSettings(QWidget* parent): SystemWidget(i18n("Allgemeine Einstellungen"), KIcon("computer"), i18n("Grundlegende Einstellungen rund um Simon"), parent)
{
	ui.setupUi(this);
	guessChildTriggers(this);
	hide();

	ui.urTempDir->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
	ui.urPathToTexts->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);

	connect(ui.cbStartSimonOnBoot, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
	connect(ui.cbAskBeforeExit, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));

	connect(ui.urProgramCategories, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urTempDir, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urPathToTexts, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	//set help
	help = i18n("Hier können Sie grundlegende Einstellungen rund um Simon verändern.\n\nDazu zählen: Pfade und Sicherheitsabfragen.\n\nEinstellungen in diesem Modul können kritisch für die Verwendung von simon sein.\n\nWenn das Häckchen \"Starte juliusd wenn nötig\" aktiviert ist, wird versucht juliusd lokal zu starten, sollte zu keinem laufenden Dämon verbunden werden können.");
}

/**
 * \brief Determines if the page is completely configured
 * 
 * \author Peter Grasch
 * @return if we are done here
 */
bool GeneralSettings::isComplete()
{
	return ((!ui.urProgramCategories->url().isEmpty()) &&  (!ui.urTempDir->url().isEmpty()) && 
			(!ui.urPathToTexts->url().isEmpty()));
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
	Settings::set("PathToProgramCategories", ui.urProgramCategories->url().path());
	Settings::set("TempDir", ui.urTempDir->url().path());
	Settings::set("PathToTexts", ui.urPathToTexts->url().path());

	Settings::set("ConfigDone", !ui.pbShowFirstRunWizard->isChecked());


// #ifdef Q_OS_WIN
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
	ui.urProgramCategories->setPath(Settings::getS("PathToProgramCategories"));
	ui.urTempDir->setPath(Settings::getS("TempDir"));
	ui.urPathToTexts->setPath(Settings::getS("PathToTexts"));

	return true;
}





/**
 * \brief Destructor
 * \author Peter Grasch
 */
GeneralSettings::~GeneralSettings()
{
}


