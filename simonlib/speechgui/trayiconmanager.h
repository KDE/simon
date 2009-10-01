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


#ifndef SIMON_TRAYICONMANAGER_H_6FF7C67B484A402598AAED62DF36E4B4
#define SIMON_TRAYICONMANAGER_H_6FF7C67B484A402598AAED62DF36E4B4

#include <QObject>
#include <KAction>
#include <KIcon>
#include <KSystemTrayIcon>
#include "speechgui_export.h"

/**
 *	@class TrayIconManager
 *	@brief Manages the System Tray Icon
 *
 *	Uses the QSystemTrayIcon Class to display the system tray icon
 *
 *	@version 0.1
 *	@date 25.01.2006
 *	@author Peter Grasch
 */
class SPEECHGUI_EXPORT TrayIconManager : public QObject
{

	Q_OBJECT
private:
	KSystemTrayIcon *icon; //!< The QSystemTrayIcon to display the icon itself

	
public:
	void parentWidgetTrayClose();
	void createIcon(const KIcon& icon, const QString& tooltip);
	void addAction(const QString& name, KAction* action);
    TrayIconManager(QWidget *parent=0);

    ~TrayIconManager();

};

#endif
