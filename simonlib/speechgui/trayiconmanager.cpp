/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "trayiconmanager.h"
#include <KActionCollection>
#include <QMenu>
#include <KSystemTrayIcon>
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
 * @param icon
 * The icon that will be displayed
 * @param tooltip
 * The tooltip of the icon
 * 
 */
void TrayIconManager::createIcon(const KIcon& icon, const QString& tooltip)
{
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
	icon->hide();
	if (icon)
		delete this->icon;
}


