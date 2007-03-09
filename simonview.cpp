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
#include "simonview.h"

/**
 * @brief Constructor
 *
 * This is the constructor of the main-UI class - the Simon View class.
 * It displays a little splash screen and initializes the member-variables
 *
 * @author Peter Grasch
 * @param Qwidget *parent
 * The parent which is passed on to the QMainWindow initialization - Default: NULL
 * @param Qt::WFlags flags
 * The flags which are also passed on to the QMainWindow constructor - as before: Default: NULL
 * 
*/

SimonView::SimonView(QWidget *parent, Qt::WFlags flags)
{
	this->info = new SimonInfo();
	
	//showing splash
	this->info->showSplash();
	
	this->info->writeToSplash("Loading...");
	
	this->control = new SimonControl();
	this->control->activateSimon();
	this->trayManager = new TrayIconManager();
	
	this->trayManager->createIcon( QIcon( ":/images/tray.png" ), "Simon" );
	
	//Preloads all Dialogs
	this->addWordView = new AddWordView(this);
	this->wordList = new WordListView(this);
	this->runDialog = new RunApplicationView(this);
	
	this->vuMeter = new VuMeter();
	if (vuMeter->prepare())
		vuMeter->start();
	
	QMainWindow(parent,flags);
	ui.setupUi(this);
	

	//Setting up Signal/Slots
	QObject::connect(vuMeter, SIGNAL(level(int)), this, SLOT(setLevel(int)));
	
	QObject::connect(ui.pbAddWord, SIGNAL(clicked()), this, SLOT(showAddWordDialog()));
	QObject::connect(ui.pbEditWordList, SIGNAL(clicked()), this, SLOT(showWordListDialog()));
	QObject::connect(ui.pbRunProgram, SIGNAL(clicked()), this, SLOT(showRunDialog()));
	QObject::connect(ui.pbHide, SIGNAL(clicked()), this, SLOT(hideSimon()));
	QObject::connect(ui.pbClose, SIGNAL(clicked()), this, SLOT(closeSimon()));
	QObject::connect(this->trayManager, SIGNAL(clicked()), this, SLOT(toggleVisibility()));
	QObject::connect(ui.pbActivision, SIGNAL(clicked()), this, SLOT(toggleActivation()));
	QObject::connect(this->trayManager, SIGNAL(middleClicked()), this, SLOT(toggleActivation()));
	
	//setting Background
	ui.lbBg->lower();
	ui.lbBg->setPixmap(QPixmap(":/images/bg.png"));

	//hiding splash again after loading
	this->info->hideSplash();
}


void SimonView::setLevel(int level)
{
	level = abs(level);
	ui.pbLevel1->setValue(level);
	ui.pbLevel2->setValue(level);
}



/**
 * @brief Shows the Run Dialog
 *
 * @author Peter Grasch
 */
void SimonView::showRunDialog()
{
	this->runDialog->exec();
}


/**
 * @brief Shows a dialog to add a new Word to the model
 *
 * @author Peter Grasch
 */
void SimonView::showAddWordDialog()
{
	this->addWordView->exec();
}

/**
 * @brief Shows a dialog to Control the Laguage Model
 *
 * @author Peter Grasch
 */
void SimonView::showWordListDialog()
{
	this->wordList->exec();
}


/**
 * @brief Hide the main window
 *
 * Hides the main window and sends it to the system tray.
 *
 *	@author Peter Grasch
 * @todo For now this function only handles the main program - the dialogs should be hidden too
 *
*/
void SimonView::hideSimon()
{
	hide();
	//hide the program
}


/**
 * @brief Shows the main Window
 *
 * Shows the Main Window after it was hidden at first
 *
 *	@author Peter Grasch
 * @todo For now this function only handles the main program - the dialogs should be shown again too
 *
 */
void SimonView::showSimon()
{
	show();
}


/**
 * @brief Toggles the activation state
 *
 * Tells the Control-Layer to toggle the activation state - according to his
 * response the following (re)actions are taken:
 * 	desaturate the Tray Image as needed
 * 	desaturate the simon logo in the bg
 * 	replaces the text on the label to "Deaktivieren"/"Aktivieren"
 *
 *	@author Peter Grasch
 *
 */
void SimonView::toggleActivation()
{
	if (this->control->toggleActivition())
	{
		ui.lbBg->setPixmap(QPixmap(":/images/bg.png"));
		ui.pbActivision->setText("&Deaktivieren");
		this->trayManager->createIcon( QIcon( ":/images/tray.png" ), "Simon" );
		
		if (isHidden()) SimonInfo::showMessage(QString("Simon wurde aktiviert"), 2000);

		repaint();
	} else 
	{
		ui.lbBg->setPixmap(QPixmap(":/images/bg_d.png"));
		ui.pbActivision->setText("&Aktivieren");

		if (isHidden()) SimonInfo::showMessage(QString("simon wurde deaktiviert"), 2000);

		this->trayManager->createIcon( QIcon( ":/images/tray_d.png" ), "Simon - Deaktiviert" );
		repaint();
	}
}

/**
 * @brief Trigger Visibility
 *
 * Asks of the current status (hidden/visible) and inverts it
 *
 * @author Peter Grasch
 * @see showSimon(), hideSimon()
 *
 */
void SimonView::toggleVisibility()
{
	if (!(this->isHidden()))
		this->hide();
	else this->show();
}


/**
 * @brief Close the main window
 *
 * Closes the whole program and kills all dialogs associated with it
 *
 * @author Peter Grasch
 * @todo For now this function only handles the main program and the tray icon
 *
*/
void SimonView::closeSimon()
{
	close();
	this->~ SimonView();
}



/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
SimonView::~SimonView()
{
	this->trayManager->~ TrayIconManager();
	this->control->~ SimonControl();
	this->info->~ SimonInfo();
}


