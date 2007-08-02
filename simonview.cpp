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
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include "simonview.h"
#include "logger.h"

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
	if (!Logger::init())
	{
		QMessageBox::critical(this, tr("Fehler"), tr("Konnte die Log-Datei nicht öffnen. Bitte Überprüfen Sie die Berechtigungen.."));
		exit(1);
	}
	
	Logger::log("Starting simon...");
	
	this->settings = new 
		QSettings(QSettings::IniFormat,QSettings::UserScope,"CyberByte","simon");
	this->info = new SimonInfo();
	
	//showing splash
	this->info->showSplash();
	
	this->info->writeToSplash("Loading...");
	
	this->control = new SimonControl();
	this->trayManager = new TrayIconManager();
	
	this->trayManager->createIcon( QIcon( ":/images/tray.png" ), "Simon" );
	
	//Preloads all Dialogs
	this->addWordView = new AddWordView(this);
	this->wordList = new WordListView(this);
	this->runDialog = new RunApplicationView(this);
	this->trainDialog = new TrainingView(wordList, this);
	this->wordList->setTrainingView(trainDialog);
	this->settingsDialog = new SettingsView(this);
	
	this->vuMeter = new VuMeter();
	//Disabled for now because it crashes the windows compile
	if (vuMeter->prepare())
		vuMeter->start();
	
	shownDialogs = 0;
	QMainWindow(parent,flags);
	ui.setupUi(this);
	
	ui.frmConnecting->setVisible(false);
	

	//Setting up Signal/Slots
	QObject::connect(vuMeter, SIGNAL(level(int)), this, SLOT(setLevel(int)));
	
	QObject::connect(ui.pbAddWord, SIGNAL(clicked()), this, SLOT(showAddWordDialog()));
	QObject::connect(ui.pbEditWordList, SIGNAL(clicked()), this, SLOT(showWordListDialog()));
	QObject::connect(ui.pbRunProgram, SIGNAL(clicked()), this, SLOT(showRunDialog()));
	QObject::connect(ui.pbTrain, SIGNAL(clicked()), this, SLOT(showTrainDialog()));
	QObject::connect(ui.pbSettings, SIGNAL(clicked()), this, SLOT(showSettingsDialog()));
	QObject::connect(ui.pbHide, SIGNAL(clicked()), this, SLOT(hideSimon()));
	QObject::connect(ui.pbClose, SIGNAL(clicked()), this, SLOT(closeSimon()));
	QObject::connect(this->trayManager, SIGNAL(clicked()), this, SLOT(toggleVisibility()));
	QObject::connect(ui.pbActivision, SIGNAL(clicked()), this, SLOT(toggleActivation()));
	QObject::connect(this->trayManager, SIGNAL(middleClicked()), this, SLOT(toggleActivation()));

	connect(wordList, SIGNAL(showAddWordDialog()), this, 
			SLOT(showAddWordDialog()));
	connect(addWordView, SIGNAL(addedWord()), wordList, 
			SLOT(reloadList()));
	connect(trainDialog, SIGNAL(trainingCompleted()), wordList, 
			SLOT(reloadList()));
	
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(ui.pbCancelConnect, SIGNAL(clicked()), this, SLOT(abortConnecting()));
	
	
	QObject::connect(control, SIGNAL(connected()), this, SLOT(connected()));
	QObject::connect(control, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(control, SIGNAL(connectionError(QString)), this, SLOT(errorConnecting(QString)));
	
	//setting Background
// 	QPixmap bg(":/images/bg_clear.png");

	QLinearGradient bg(QPointF(1, 1), QPointF(900, 550));
	bg.setColorAt(0, QColor(70, 120, 190));
	bg.setColorAt(1, QColor(25, 60, 130));

	QPalette p(palette());
	p.setBrush(QPalette::Background, bg);
	setPalette(p);
	
	
	
	ui.lbLogo->setPixmap(QPixmap(":/images/simon.png"));

	this->info->writeToSplash("Connecting to juliusd...");
	connectToServer();
	
	//hiding splash again after loading
	this->info->hideSplash();
	
}

/**
 * \brief Connects to juliusd and gives appropriate status information about it
 *
 * @author Peter Grasch
 */
void SimonView::connectToServer()
{
	if (!(settings->value("network/defaultjuliusdaddress").toString()).isEmpty())
	{
           ui.pbConnect->setText("Verbinde...");
	   ui.pbConnect->setEnabled(false);
	   ui.frmConnecting->setVisible(true);
	   this->control->activateSimon();
	   this->control->connect(settings->value("network/defaultjuliusdaddress").toString());
    }
}

/**
 * \brief Informs the user that we connected to the server
 *
 * @author Peter Grasch
 */
void SimonView::connected()
{
	ui.pbConnect->setText("Verbindung trennen");
	ui.pbConnect->setEnabled(true);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), control, SLOT(disconnect()));
	
	ui.frmConnecting->setVisible(false);
	SimonInfo::showMessage("Verbunden zu Julius", 3000);
	
	this->control->getActivitionState();
	this->representState();
}

/**
 * \brief Informs the user that we have been disconnected from the server
 *
 * @author Peter Grasch
 */
void SimonView::disconnected()
{
	ui.pbConnect->setText("Verbinden");
	ui.pbConnect->setEnabled(true);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	
	SimonInfo::showMessage("Verbindung zu Julius verloren", 4000);
	//we should probably try to reconnect at this point
	
	control->deactivateSimon();
	this->representState();
}


/**
 * \brief We canceled the connecting to the server
 *
 * @author Peter Grasch
 */
void SimonView::abortConnecting()
{
	ui.pbConnect->setText("Verbinden");
	ui.pbConnect->setEnabled(true);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	
	ui.frmConnecting->setVisible(false);
	
	this->control->abortConnecting();
	this->representState();
	
	this->control->deactivateSimon();
	
}

/**
 * \brief Error connecting to the server
 *
 * @author Peter Grasch
 * 
 * \param QString error
 * The error that occured
 */
void SimonView::errorConnecting(QString error)
{
	ui.pbConnect->setText("Verbinden");
	ui.pbConnect->setEnabled(true);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	
	ui.frmConnecting->setVisible(false);
	
	this->control->deactivateSimon();
	this->representState();
	
	QMessageBox::critical(this, tr("Kritischer Verbindungsfehler"), tr("Die Verbindung zum juliusd Erkennungsdämon konnte nicht aufgenommen werden.\n\nBitte Überprüfen Sie Ihre Einstellungen, ihre Netzwerkverbindung und ggf. Ihre Firewall.\n\nDie exakte Fehlermeldung lautete:\n")+error);
}



/**
 * \brief Sets the vumeter to the given level
 *
 * @author Peter Grasch
 */
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
	this->runDialog->show();
}


/**
 * @brief Shows a dialog to add a new Word to the model
 *
 * @author Peter Grasch
 */
void SimonView::showAddWordDialog()
{
	this->addWordView->show();
}

/**
 * @brief Shows a dialog to configure simon
 *
 * @author Peter Grasch
 */
void SimonView::showSettingsDialog()
{
	this->settingsDialog->show();
}

/**
 * @brief Shows a dialog to train the language model
 *
 * @author Peter Grasch
 */
void SimonView::showTrainDialog()
{
	this->trainDialog->show();
}

/**
 * @brief Shows a dialog to Control the Laguage Model
 *
 * @author Peter Grasch
 */
void SimonView::showWordListDialog()
{
	this->wordList->show();
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
	shownDialogs=0;
	currentPos = pos();

	if (this->addWordView->isVisible())
	{
		this->shownDialogs = shownDialogs | sAddWordView;
		addWordDlgPos = addWordView->pos();
		this->addWordView->hide();
	}
	if (this->trainDialog->isVisible())
	{
		this->shownDialogs = shownDialogs | sTrainMain;
		trainDlgPos = trainDialog->pos();
		this->trainDialog->hide();
	}
	if (this->wordList->isVisible())
	{
		this->shownDialogs = shownDialogs | sWordListView;
		wordlistDlgPos = wordList->pos();
		this->wordList->hide();
	}
	if (this->runDialog->isVisible())
	{
		this->shownDialogs = shownDialogs | 
					sRunApplicationView;
		runDlgPos = runDialog->pos();
		this->runDialog->hide();
	}
	if (this->settingsDialog->isVisible())
	{
		this->shownDialogs = shownDialogs | 
					sSettingsView;
		settingsDlgPos = settingsDialog->pos();
		this->settingsDialog->hide();
	}
	hide();
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
	move(currentPos);
	show();
	if (shownDialogs & sAddWordView) 
	{
		addWordView->show();
		addWordView->move(addWordDlgPos);
	}

	if (shownDialogs & sTrainMain)
	{
		trainDialog->show();
		trainDialog->move(trainDlgPos);
	}

	if (shownDialogs & sRunApplicationView)
	{
		runDialog->show();
		runDialog->move(runDlgPos);
	}

	if (shownDialogs & sWordListView)
	{
		wordList->show();
		wordList->move(wordlistDlgPos);
	}

	if (shownDialogs & sSettingsView)
	{
		settingsDialog->show();
		settingsDialog->move(settingsDlgPos);
	}
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
	this->control->toggleActivition();
	this->representState();
}

/**
 * \brief Make the widgets represent the current state (active/inactive)
 * 
 * \author Peter Grasch
 */
void SimonView::representState()
{
	if (this->control->getActivitionState())
	{
		ui.lbLogo->setPixmap(QPixmap(":/images/simon.png"));
		ui.pbActivision->setText("  &Deaktivieren");
		ui.pbActivision->setIcon(QIcon(":/images/icons/media-playback-pause.svg"));
		this->trayManager->createIcon( QIcon( ":/images/tray.png" ), "Simon" );
		
		if (isHidden()) SimonInfo::showMessage(QString("Simon wurde aktiviert"), 2000);

		repaint();
	} else 
	{
		ui.lbLogo->setPixmap(QPixmap(":/images/simon_d.png"));
		ui.pbActivision->setText("  A&ktivieren");
		ui.pbActivision->setIcon(QIcon(":/images/icons/media-playback-start.svg"));

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
		this->hideSimon();
	else this->showSimon();
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
	if ((!false /*Confirm shutdown*/) || (QMessageBox::question(this, "Wirklich beenden?", "Ein beenden der Applikation wird die Verbindung zur Erkennung beenden und weder Diktatfunktionen noch andere Kommandos können mehr benutzt werden.\n\nWollen Sie wirklich beenden?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes))
	{
		close();
		this->~ SimonView();
	}
}



/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
SimonView::~SimonView()
{
	Logger::log("Shutting down...");
	this->trayManager->~ TrayIconManager();
	this->control->~ SimonControl();
	this->info->~ SimonInfo();
	Logger::close();
}


