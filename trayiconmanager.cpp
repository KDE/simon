//
// C++ Implementation: trayiconmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "trayiconmanager.h"

/**
 * @brief Constructor
 *
 * Initializes the parent QObject class
 * Sets up the connection from the triggered signal to the triggered slot
 *
 * @author Peter Grasch
 * 
 */
TrayIconManager::TrayIconManager()
 : QObject()
{
	this->icon = new QSystemTrayIcon(this);
	connect(this->icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(triggered(QSystemTrayIcon::ActivationReason)));
}

/**
 * @brief Creates the Icon and displays it in the systray
 *
 * Utilizes the QSystemTrayIcon class from qt 4.2
 *
 * @author Peter Grasch
 * @param QPixmap icon
 * The icon that will be displayed
 * 
 */
void TrayIconManager::createIcon(QIcon icon, QString tooltip)
{
	this->icon->setIcon( icon );
	this->icon->setToolTip( tooltip );
	this->icon->show();
}

/**
 * @brief Hides the Systrayicon
 *
 * @author Peter Grasch
 * 
 */
void TrayIconManager::hideIcon()
{
	this->icon->hide();
}

/**
 * @brief Slot to react on the triggered action of the icon
 *
 * Emits the clicked() signal if the icon was clicked
 * 
 * @author Peter Grasch
 * @param QSystemTrayIcon::ActivationReason reason
 *  Contains the reason and is used to determine which button was pressed
 */
void TrayIconManager::triggered( QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
		emit clicked();
	else if (reason == QSystemTrayIcon::MiddleClick)
		emit middleClicked();
}

/**
 * @brief Destructor
 *
 * Deletes the icon
 *
 * @author Peter Grasch
 */
TrayIconManager::~TrayIconManager()
{
	this->icon->~ QSystemTrayIcon();
}


