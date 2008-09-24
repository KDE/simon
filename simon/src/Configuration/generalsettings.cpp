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
#include <KFile>
#include "../SimonLib/Settings/settings.h"

/**
 * \brief Constructs a new GeneralSettings object
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
GeneralSettings::GeneralSettings(QWidget* parent): QWidget(parent) //i18n("Allgemeine Einstellungen"), KIcon("computer"), i18n("Grundlegende Einstellungen rund um Simon"), parent)
{
	ui.setupUi(this);
// 	guessChildTriggers(this);

	//set help
	//help = i18n("Hier können Sie grundlegende Einstellungen rund um Simon verändern.\n\nDazu zählen: Pfade und Sicherheitsabfragen.\n\nEinstellungen in diesem Modul können kritisch für die Verwendung von simon sein.\n\nWenn das Häckchen \"Starte juliusd wenn nötig\" aktiviert ist, wird versucht juliusd lokal zu starten, sollte zu keinem laufenden Dämon verbunden werden können.");
}

/**
 * \brief Applys the changed settings
 * \author Peter Grasch
 * @return Success
 */
// bool GeneralSettings::apply()
// {

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

// 	return true;
// }




/**
 * \brief Destructor
 * \author Peter Grasch
 */
GeneralSettings::~GeneralSettings()
{
}


