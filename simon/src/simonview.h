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
#ifndef SIMONVIEW_H
#define SIMONVIEW_H


/**
 *	@class SimonView
 *	@brief The Main UI Class
 *
 *	This class is not only the "main" UI-Class but also the interface to the
 *	underlying main concept class.
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
*/

#define sAddWordView 		1
#include "ui_main.h"
#include "simonmainwindow.h"
#include "simoncontrol.h"

class QPoint;
class QWidget;
class InlineWidgetView;
class QString;
class QTimer;
class SimonControl;
class SimonInfo;
class TrainingView;
class VuMeter;
class AddWordView;
class TrayIconManager;
class RunCommandView;
class SystemView;
class QAction;
class WordListView;
class ConfigurationDialog;


typedef QHash<QObject*,  const char*> ActionIdent;

class SimonView : public SimonMainWindow    {
	
	Q_OBJECT
	
private slots:
	void compileModel();

private:
	bool settingsShown;
	int shownDialogs;
	QPoint currentPos;
	QPoint addWordDlgPos;

	Ui::MainWindow ui;	//!< Mainwindow UI definition - made by uic from the QTDesigner .ui
	SimonControl *control; //!< Pointer to the main concept class
	TrayIconManager *trayManager; //!< Handles the TrayIcon
	AddWordView *addWordView; //!< Pointer on the Dialog "Add Word to Language model"
	WordListView *wordList; //!< Pointer on the Dialog "WordList"
	RunCommandView *runDialog; //!< Pointer on the Dialog "RunCommand"
	TrainingView *trainDialog; //!< Pointer on the Dialog "Training"
	SystemView *systemDialog; //!< Pointer on the Dialog "System"
	ConfigurationDialog *configurationDialog;


	void setupSignalSlots();
	void setupActions();


public slots:
	void displayStatus(const QString &status);
	void displayConnectionStatus(const QString &status);
	void displayProgress(int cur, int max);
	
	void toggleVisibility();
	void hideSimon();
	void showSimon();
	void closeSimon();
	
	void toggleConnection();
	void errorConnecting(QString error);
	
	void toggleActivation();
	void representState(SimonControl::SystemStatus status);
	

	void showAddWordDialog();
	void showRunDialog();
	void showTrainDialog();
	void showWordListDialog();
	void showSystemDialog();
    
    void checkSettingState();

public:
	explicit SimonView(QWidget *parent = 0, Qt::WFlags flags = 0);

	~SimonView();

	void closeEvent ( QCloseEvent * event );

    bool checkPassword();
    void hideSettings();
    void showSettings();

};

#endif
