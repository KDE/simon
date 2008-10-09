/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H


/**
	@author Peter Grasch <bedahr@gmx.net>
*/

#include <QMainWindow>
#include <QTcpSocket>
#include <QHash>
#include <QByteArray>
#include <QProcess>
#include "networkcontrol.h"
#include "trayiconmanager.h"
#include "settingsview.h"
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>
#include <QHostAddress>
#include "ui_main.h"
class QTextEdit;

/**
 *	@class ProjectView
 *	@brief Views the Main Window
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */

class ProjectView : public QMainWindow {
	
	Q_OBJECT
public slots:
	void startServer();
	void sendWord(QString word="__INTERNAL__EMPTY__");
	void stopServer();
	void reloadServer();
	void toggleVisibility();
	void closeJuliusd();
	void showSettingsDialog();
	
	void clientConnected(QString host, int id);
	void clientConnectionDropped(QString host, int id);
	
	void toggleStartStop();
	void printError(QString error, int id);
	void displaySentWord(QString word, int id);
	void configureUsers();
	void printInfo(QString info, int id);
	void showAboutSimon();
	void showAboutQT();
private:
	bool running; //!< The current state of the system
	NetworkControl *net; //!< Network handle - used to send the words
	TrayIconManager *trayIcon; //!< The tray-icon
	Ui::MainWindow ui;  //!< UI Definition made from uic from the .ui files
	SettingsView *dlgSettings; //!< Settings Dialog
	QHash <int, QTextEdit*> logs;
	bool initDb();
	
public:
	ProjectView(QWidget *parent = 0, Qt::WFlags flags = 0);

    ~ProjectView();

};

#endif
