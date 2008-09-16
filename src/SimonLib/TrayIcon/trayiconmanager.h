//
// C++ Interface: trayiconmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRAYICONMANAGER_H
#define TRAYICONMANAGER_H

#include <QObject>
#include <KAction>
#include <KIcon>
#include <KSystemTrayIcon>

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
class TrayIconManager : public QObject
{

	Q_OBJECT
private:
	KSystemTrayIcon *icon; //!< The QSystemTrayIcon to display the icon itself


	
public:
	void createIcon(const KIcon& icon, const QString& tooltip);
	void addAction(const QString& name, KAction* action);
    TrayIconManager(QWidget *parent);

    ~TrayIconManager();

};

#endif
