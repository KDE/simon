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
 *	@class SimonView
 *	@brief The Main UI Class
 *
 *	This class is not only the "main" UI-Class but also the interface to the
 *	underlying main concept class.
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
 *	@todo Implementing functions
*/

#include <QMainWindow>
#include <QPlastiqueStyle>
#include <QPixmap>
#include <QSplashScreen>
#include <QMessageBox>
#include "simoncontrol.h"
#include "simoninfo.h"
#include "runapplicationview.h"
#include "trayiconmanager.h"
#include "wordlistview.h"
#include "vumeter.h"
#include "trainingview.h"
#include "settingsview.h"

#include "wav.h"
#include "miccontrol.h"

#include "ui_main.h"


class SimonView : public QMainWindow {
	
	Q_OBJECT
private:
	Ui::MainWindow ui;	//!< Mainwindow UI definition - made by uic from the QTDesigner .ui
	QSplashScreen *splash;  //!< Splashscreen-Pointer
	SimonControl *control; //!< Pointer to the main concept class
	SimonInfo *info;  //!< Pointer to the Info class
	TrayIconManager *trayManager; //!< Handles the TrayIcon
	AddWordView *addWordView; //!< Pointer on the Dialog "Add Word to Language model"
	WordListView *wordList; //!< Pointer on the Dialog "WordList"
	RunApplicationView *runDialog; //!< Pointer on the Dialog "RunCommand"
	TrainingView *trainDialog; //!< Pointer on the Dialog "Training"
	SettingsView *settingsDialog; //!< Pointer on the Dialog "Settings"
	
	VuMeter *vuMeter; //!< Does the calculation of the current input "loudness" for the 2 bars in the mainwindow

public slots:
	void setLevel(int level);
	
	
	void toggleVisibility();
	void hideSimon();
	void showSimon();
	
	void closeSimon();
	void connectToServer();
	void abortConnecting();
	void connected();
	void disconnected();
	
	void toggleActivation();
	
	void showAddWordDialog();
	void showRunDialog();
	void showTrainDialog();
	void showWordListDialog();
	void showSettingsDialog();

public:
	SimonView(QWidget *parent = 0, Qt::WFlags flags = 0);
	
	~SimonView();

};

#endif
