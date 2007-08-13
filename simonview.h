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


//#define ANIMATIONS

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
#define sTrainMain 		2
#define sWordListView 		4
#define sRunApplicationView 	8
#define sSystemView	 	16


#include "wordlistview.h"

#include <QMainWindow>


#include <QSettings>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include "ui_main.h"

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
class RunApplicationView;
class SystemView;

class SimonView : public QMainWindow {
	
	Q_OBJECT

private slots:
	
	#ifdef ANIMATIONS
	void resizeButtons();
	void startTransformToBusy();
	void startTransformToIdle();
	#endif
	void inlineButtonClicked();
	void setMainButtonsIconSize(QSize newSize);
	void setMainButtonsFontSize(float fontSize);
	void resizeMainButtonContentsToWindow();
	void setButtonsBusy();
	void setButtonsIdle();
	void setButtonNotChecked();

private:
	int shownDialogs;
	QPoint currentPos;
	QPoint runDlgPos;
	QPoint addWordDlgPos;
	QPoint wordlistDlgPos;
	QPoint trainDlgPos;
	QPoint settingsDlgPos;
	QWidget *buttonMover;
	InlineWidgetView *inlineView;

	Ui::MainWindow ui;	//!< Mainwindow UI definition - made by uic from the QTDesigner .ui
	SimonControl *control; //!< Pointer to the main concept class
	SimonInfo *info;  //!< Pointer to the Info class
	TrayIconManager *trayManager; //!< Handles the TrayIcon
	AddWordView *addWordView; //!< Pointer on the Dialog "Add Word to Language model"
	WordListView *wordList; //!< Pointer on the Dialog "WordList"
	RunApplicationView *runDialog; //!< Pointer on the Dialog "RunCommand"
	TrainingView *trainDialog; //!< Pointer on the Dialog "Training"
	SystemView *systemDialog; //!< Pointer on the Dialog "System"
	VuMeter *vuMeter; //!< Does the calculation of the current input "loudness" for the 2 bars in the mainwindow
	

	#ifdef ANIMATIONS
	
	bool xMoveDone, yMoveDone, wScaleDone, hScaleDone, logoScaleDone;
	
	QVector<QPushButton*> animatedButtons;
	short moveYDirection, moveXDirection, logoScaleDirection;
	float currentSizeH, currentSizeW, currentMoveX, currentMoveY, logoScaleFactor,
		scaleXFactor, scaleYFactor;
	QTimer *tresizeButtons;
	QTimer *fadeIn;

	void setupAnimations();
	#endif
	
	bool viewShouldBeBusy();
	bool viewBusy;
	void resizeEvent(QResizeEvent *event);
	void setupSignalSlots();


public slots:
	void setLevel(int level);
	
	
	void toggleVisibility();
	void hideSimon();
	void showSimon();
	
	void closeSimon();
	void connectToServer();
	void abortConnecting();
	void errorConnecting(QString error);
	void connected();
	void disconnected();
	
	void toggleActivation();
	void representState();
	
	void showAddWordDialog(bool show=true);
	void showRunDialog(bool show=true);
	void showTrainDialog(bool show=true);
	void showWordListDialog(bool show=true);
	void showSystemDialog(bool show=true);

public:
	SimonView(QWidget *parent = 0, Qt::WFlags flags = 0);

	~SimonView();

	void closeEvent ( QCloseEvent * event );
	


};

#endif
