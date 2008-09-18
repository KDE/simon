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
#include <KActionCollection>
#include <QMenu>
#include <KSystemTrayIcon>
#include "../Logging/logger.h"
#include <KLocalizedString>

/**
 * @brief Constructor
 *
 * Initializes the parent QObject class
 * Sets up the connection from the triggered signal to the triggered slot
 *
 * @author Peter Grasch
 * 
 */
TrayIconManager::TrayIconManager(QWidget *parent)
 : QObject()
{
	this->icon = new KSystemTrayIcon(parent);
}

/**
 * @brief Creates the Icon and displays it in the systray
 *
 * Utilizes the KSystemTrayIcon class
 *
 * @author Peter Grasch
 * @param QPixmap icon
 * The icon that will be displayed
 * 
 */
void TrayIconManager::createIcon(const KIcon& icon, const QString& tooltip)
{
	Logger::log(i18n("[INF] Erstelle systray icon (\"%1\")").arg(tooltip));
	this->icon->setIcon( icon );
	this->icon->setToolTip( tooltip );
	this->icon->show();
}


void TrayIconManager::addAction(const QString& name, KAction* action)
{
	this->icon->contextMenu()->addAction(action);
	this->icon->actionCollection()->addAction(name, action);
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
	if (icon)
		delete this->icon;
}


