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
#include "ui_simonview.h"
#include "simonmainwindow.h"
#include "simoncontrol.h"

class QPoint;
class QWidget;
class InlineWidgetView;
class QString;
class SimonControl;
class SimonInfo;
class TrainingView;
class AddWordView;
class TrayIconManager;
class RunCommandView;
class QAction;
class WordListView;
class KCMultiDialog;


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
	KCMultiDialog *configDialog;


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