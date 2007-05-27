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
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>


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
	QSystemTrayIcon *icon; //!< The QSystemTrayIcon to display the icon itself

signals:
	/**
	* @brief Signal: clicked
	*
	* Signal that is emitted when the icon is clicked
	*
	* @author Peter Grasch
	* 
	 */
	void clicked();
	
	/**
	* @brief Signal: middleClicked
	*
	* Signal that is emitted when the icon is clicked with the middle (3rd) Mouse Button
	*
	* @author Peter Grasch
	* 
	 */
	void middleClicked();

public slots:
	void triggered( QSystemTrayIcon::ActivationReason reason);
	
public:
	void createIcon(QIcon icon, QString tooltip);
	void hideIcon();
    TrayIconManager();

    ~TrayIconManager();

};

#endif
