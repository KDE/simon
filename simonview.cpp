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
#include "inlinewidgetview.h"
#include "logger.h"
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include "simoncontrol.h"
#include "simoninfo.h"
#include "runapplicationview.h"
#include "trayiconmanager.h"
#include "vumeter.h"
#include "soundsettings.h"
#include "trainingview.h"
#include "systemview.h"
#include "settings.h"

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
    ipwizard->show();
	if (!Logger::init())
	{
		QMessageBox::critical(this, tr("Fehler"), tr("Konnte die Log-Datei nicht öffnen. Bitte überprüfen Sie die Berechtigungen.."));
		exit(1);
	}
	
	Logger::log(tr("[INF] Starte simon..."));
	
	this->info = new SimonInfo();
	
	//showing splash
	Logger::log(tr("[INF] Zeige Splashscreen..."));
	this->info->showSplash();
	
	Logger::log(tr("[INF] Lade Einstellungen..."));
	Settings::initSettings();
	
	this->info->writeToSplash(tr("Lade Programmlogik..."));
	
	this->control = new SimonControl();
	this->trayManager = new TrayIconManager();
	
	this->trayManager->createIcon( QIcon( ":/images/tray.png" ), "Simon" );
	
	inlineView = new InlineWidgetView(this);
	inlineView->hide();
	QSizePolicy pol(QSizePolicy::Expanding,QSizePolicy::Expanding);
	pol.setVerticalStretch(1);
	inlineView->setSizePolicy(pol);
	

    //this->ipwizard = new ImportProgramWizard(this);  //todo delete!! just for testing
	//Preloads all Dialogs
	this->info->writeToSplash(tr("Lade \"Wort hinzufühgen\"..."));
	this->addWordView = new AddWordView(this);
	this->info->writeToSplash(tr("Lade \"Wortliste\"..."));
	this->wordList = new WordListView(inlineView);
	this->info->writeToSplash(tr("Lade \"Ausführen\"..."));
	this->runDialog = new RunApplicationView(this);
	this->info->writeToSplash(tr("Lade \"Trainieren\"..."));

	this->trainDialog = new TrainingView(wordList, this);
	this->wordList->setTrainingView(trainDialog);

	this->info->writeToSplash(tr("Lade \"System\"..."));
	this->systemDialog = new SystemView(this);
	
	this->info->writeToSplash(tr("Lade Oberfläche..."));


	this->vuMeter = new VuMeter();
	if (vuMeter->prepare())
		vuMeter->start();
	
	shownDialogs = 0;
	viewBusy = false;
	QMainWindow(parent,flags);
	ui.setupUi(this);
	
	
	buttonMover= ui.wButtonWidget;
	#ifdef ANIMATIONS
	setupAnimations();
	#endif
	
	setupSignalSlots();

	
	//setting Background
	QLinearGradient bg(QPointF(1, 1), QPointF(900, 550));
	bg.setColorAt(0, QColor(70, 120, 190));
	bg.setColorAt(1, QColor(25, 60, 130));
	QPalette p(palette());
	p.setBrush(QPalette::Background, bg);
	setPalette(p);

	if (Settings::get("AutoConnect").toBool())
	{
		this->info->writeToSplash(tr("Verbinde zu juliusd..."));
		connectToServer();
	}
	
	//hiding splash again after loading
	this->info->hideSplash();

	show();
	QCoreApplication::processEvents();
	resizeMainButtonContentsToWindow();
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void SimonView::setupSignalSlots()
{
	//Setting up Signal/Slots
	QObject::connect(vuMeter, SIGNAL(level(int)), this, SLOT(setLevel(int)));
	
	QObject::connect(ui.pbAddWord, SIGNAL(toggled(bool)), this, SLOT(showAddWordDialog(bool)));
	QObject::connect(ui.pbEditWordList, SIGNAL(toggled(bool)), this, SLOT(showWordListDialog(bool)));
	QObject::connect(ui.pbRunProgram, SIGNAL(toggled(bool)), this, SLOT(showRunDialog(bool)));
	QObject::connect(ui.pbTrain, SIGNAL(toggled(bool)), this, SLOT(showTrainDialog(bool)));
	QObject::connect(ui.pbSettings, SIGNAL(toggled(bool)), this, SLOT(showSystemDialog(bool)));
	QObject::connect(addWordView, SIGNAL(hidden()), this, SLOT(setButtonNotChecked()));
	


	QObject::connect(ui.pbHide, SIGNAL(clicked()), this, SLOT(hideSimon()));
	QObject::connect(ui.pbClose, SIGNAL(clicked()), this, SLOT(closeSimon()));
	QObject::connect(this->trayManager, SIGNAL(clicked()), this, SLOT(toggleVisibility()));
	QObject::connect(ui.pbActivision, SIGNAL(clicked()), this, SLOT(toggleActivation()));
	QObject::connect(this->trayManager, SIGNAL(middleClicked()), this, SLOT(toggleActivation()));

	QObject::connect(ui.pbEditWordList, SIGNAL(clicked()), this, SLOT(inlineButtonClicked()));
	QObject::connect(ui.pbTrain, SIGNAL(clicked()), this, SLOT(inlineButtonClicked()));
	QObject::connect(ui.pbRunProgram, SIGNAL(clicked()), this, SLOT(inlineButtonClicked()));
	QObject::connect(ui.pbSettings, SIGNAL(clicked()), this, SLOT(inlineButtonClicked()));

	connect(wordList, SIGNAL(showAddWordDialog()), this, 
		SLOT(showAddWordDialog()));
	connect(addWordView, SIGNAL(addedWord()), wordList, 
		SLOT(reloadList()));
	connect(trainDialog, SIGNAL(trainingCompleted()), wordList, 
		SLOT(reloadList()));
	
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
// 	connect(ui.pbCancelConnect, SIGNAL(clicked()), this, SLOT(abortConnecting()));
	
	
	QObject::connect(control, SIGNAL(connected()), this, SLOT(connected()));
	QObject::connect(control, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(control, SIGNAL(connectionError(QString)), this, SLOT(errorConnecting(QString)));
}

bool SimonView::viewShouldBeBusy()
{
	return (ui.pbSettings->isChecked() || 
		ui.pbTrain->isChecked() || ui.pbEditWordList->isChecked() || 
			ui.pbRunProgram->isChecked() );
}

void SimonView::setButtonNotChecked()
{
	if (dynamic_cast<AddWordView*>(sender())) {
		ui.pbAddWord->setChecked(false);
	} else if (dynamic_cast<RunApplicationView*>(sender())) {
		ui.pbRunProgram->setChecked(false);
	} else if (dynamic_cast<WordListView*>(sender())) {
		ui.pbEditWordList->setChecked(false);
	} else if (dynamic_cast<SystemView*>(sender())) {
		ui.pbSettings->setChecked(false);
	} else if (dynamic_cast<TrainingView*>(sender())) {
		ui.pbTrain->setChecked(false);
	}
}

void  SimonView::resizeEvent(QResizeEvent *event)
{
	if (viewBusy) 
		this->setButtonsBusy();
	else {
		resizeMainButtonContentsToWindow();
	}
	QWidget::resizeEvent(event);
}

void SimonView::resizeMainButtonContentsToWindow()
{
	float newFontSize =  (int) round( ((float) buttonMover->height())/35.3f )+7;
	int iconSize = (int) round((((float)buttonMover->height())/2.5f)/4+10);
	QSize newIconSize = QSize(iconSize, iconSize);
	setMainButtonsIconSize(newIconSize);
	setMainButtonsFontSize(newFontSize);
}

void SimonView::setMainButtonsIconSize(QSize newSize)
{
	ui.pbRunProgram->setIconSize(newSize);
	ui.pbTrain->setIconSize(newSize);
	ui.pbEditWordList->setIconSize(newSize);
	ui.pbAddWord->setIconSize(newSize);
}

void SimonView::setMainButtonsFontSize(float fontSize)
{
	QFont f = ui.pbAddWord->font();
	f.setPointSizeF(fontSize);
	ui.pbAddWord->setFont(f);
	ui.pbEditWordList->setFont(f);
	ui.pbTrain->setFont(f);
	ui.pbRunProgram->setFont(f);

}

/**
 * \brief Connects to juliusd and gives appropriate status information about it
 *
 * @author Peter Grasch
 */
void SimonView::connectToServer()
{
	ui.pbConnect->setText("Verbinde...");
	ui.pbConnect->setChecked(true);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(abortConnecting()));
	this->control->connectToJulius();
}

/**
 * \brief Informs the user that we connected to the server
 *
 * @author Peter Grasch
 */
void SimonView::connected()
{
	ui.pbConnect->setText(tr("Verbunden"));
	ui.pbConnect->setEnabled(true);
	ui.pbActivision->setEnabled(true);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), control, SLOT(disconnectFromJulius()));
	
	SimonInfo::showMessage(tr("Verbunden zu Julius"), 3000);
	
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
	ui.pbConnect->setText(tr("Verbinden"));
	ui.pbActivision->setEnabled(false);
	ui.pbConnect->setChecked(false);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	
	SimonInfo::showMessage(tr("Verbindung zu Julius verloren"), 4000);
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
	ui.pbConnect->setText(tr("Verbinden"));
	ui.pbConnect->setChecked(false);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	
	this->control->abortConnecting();
	this->representState();
	
	this->control->deactivateSimon();
	
}

/**
 * \brief Error connecting to the server
 *
 * @author Peter Grasch
 * 
 * \param  error
 * The error that occured
 */
void SimonView::errorConnecting(QString error)
{
	ui.pbConnect->setText(tr("Verbinden"));
	ui.pbConnect->setChecked(false);
	disconnect(ui.pbConnect, 0,0,0);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
	
	this->control->deactivateSimon();
	this->representState();
	
	QMessageBox::critical(this, tr("Kritischer Verbindungsfehler"), tr("Die Verbindung zum juliusd Erkennungsdämon konnte nicht aufgenommen werden.\n\nBitte überprüfen Sie Ihre Einstellungen, ihre Netzwerkverbindung und ggf. Ihre Firewall.\n\nDie exakte(n) Fehlermeldung(en) lautete(n):\n")+error);
	Logger::log(tr("[ERR] Verbindung zu juliusd fehlgeschlagen..."));
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
void SimonView::showRunDialog(bool show)
{
	if (show)
	{
		inlineView->registerPage(runDialog);
		ui.pbRunProgram->setChecked(true);
	} else {
		inlineView->unRegisterPage(runDialog);
		ui.pbRunProgram->setChecked(false);
	}
}


/**
 * @brief Shows a dialog to add a new Word to the model
 *
 * @author Peter Grasch
 */
void SimonView::showAddWordDialog(bool show)
{
	if (show)
	{
		this->addWordView->show();
        //this->ipwizard->show();
		ui.pbaddWord->setChecked(true);
	} else 
	{
		this->addWordView->hide();
		ui.pbAddWord->setChecked(false);
	}
}

/**
 * @brief Shows a dialog to configure simon
 *
 * @author Peter Grasch
 */
void SimonView::showSystemDialog(bool show)
{
	if (show) {
		inlineView->registerPage(systemDialog);
		ui.pbSettings->setChecked(true);
	} else {
		inlineView->unRegisterPage(systemDialog);
		ui.pbSettings->setChecked(false);
	}
}

/**
 * @brief Shows a dialog to train the language model
 *
 * @author Peter Grasch
 */
void SimonView::showTrainDialog(bool show)
{
	if (show) {
		ui.pbTrain->setChecked(true);
		inlineView->registerPage(trainDialog);
	} else {
		ui.pbTrain->setChecked(false);
		inlineView->unRegisterPage(trainDialog);
	}
}

/**
 * @brief Shows a dialog to Control the Laguage Model
 *
 * @author Peter Grasch
 */
void SimonView::showWordListDialog(bool show)
{
	if (show) {
// 		this->wordList->show();
		inlineView->registerPage(wordList);
		ui.pbEditWordList->setChecked(true);
	} else {
		inlineView->unRegisterPage(wordList);
// 		this->wordList->hide();
		ui.pbEditWordList->setChecked(false);
	}
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
	if (this->systemDialog->isVisible())
	{
		this->shownDialogs = shownDialogs | 
					sSystemView;
		settingsDlgPos = systemDialog->pos();
		this->systemDialog->hide();
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

	if (shownDialogs & sSystemView)
	{
		systemDialog->show();
		systemDialog->move(settingsDlgPos);
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
		ui.pbActivision->setText(tr("Deaktivieren"));
		ui.pbActivision->setIcon(QIcon(":/images/icons/media-playback-pause.svg"));
		this->trayManager->createIcon( QIcon( ":/images/tray.png" ), "Simon" );
		
		if (isHidden()) SimonInfo::showMessage(QString("Simon wurde aktiviert"), 2000);

		repaint();
	} else 
	{
		ui.lbLogo->setPixmap(QPixmap(":/images/simon_d.png"));
		ui.pbActivision->setText(tr("Aktivieren"));
		ui.pbActivision->setIcon(QIcon(":/images/icons/media-playback-start.svg"));

		if (isHidden()) SimonInfo::showMessage(QString(tr("simon wurde deaktiviert")), 2000);

		this->trayManager->createIcon( QIcon( ":/images/tray_d.png" ), tr("Simon - Deaktiviert" ));
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
	if ((!Settings::get("AskBeforeExit").toBool()) || (QMessageBox::question(this, tr("Wirklich beenden?"), tr("Ein beenden der Applikation wird die Verbindung zur Erkennung beenden und weder Diktatfunktionen noch andere Kommandos können mehr benutzt werden.\n\nWollen Sie wirklich beenden?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes))
	{
		close();
		this->~ SimonView();
	}
}


/**
 * @brief sender request
 * it differs if the sender is pbClose, or the CloseButton in the title bar
 * 
 * \param *event
 * just to complie with the original definition in QObject
 * 
 *	@author Phillip Goriup, Peter Grasch
*/
void SimonView::closeEvent ( QCloseEvent * event )
{
	this->hideSimon();
	if (sender() != ui.pbClose)
		event->ignore();
}





/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
SimonView::~SimonView()
{
	Logger::log(tr("[INF] Beenden..."));
	this->trayManager->~ TrayIconManager();
	this->control->~ SimonControl();
	this->info->~ SimonInfo();
	Logger::close();
}
