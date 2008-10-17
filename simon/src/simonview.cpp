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


#include "simonview.h"


#include "inlinewidgetview.h"


#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>

#include <simonactionsui/runcommandview.h>
#include <trayiconmanager.h>

#include <speechmodelmanagement/wordlistmanager.h>
#include <simonmodelmanagementui/trainingview.h>
#include <simonmodelmanagementui/wordlistview.h>
#include <simonmodelmanagementui/AddWord/addwordview.h>
#include <simonactions/actionmanager.h>


#include <QPixmap>
#include <QCryptographicHash>
#include <QDebug>


#include <KMessageBox>
#include <KApplication>
#include <KAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>
#include <KStandardDirs>
#include <KStatusBar>
#include <KPasswordDialog>
#include <KConfigDialog>
#include <KCMultiDialog>
#include <KCModuleProxy>
#include <KPageWidgetItem>

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
SimonView::SimonView ( QWidget *parent, Qt::WFlags flags )
		:SimonMainWindow ( parent, flags )
{
	Logger::log ( i18n ( "[INF] Starte simon..." ) );

	SimonInfo *info = new SimonInfo();

	//showing splash
	Logger::log ( i18n ( "[INF] Zeige Splashscreen..." ) );
	info->showSplash();

	Logger::log ( i18n ( "[INF] Lade Konfigurationsmodule..." ) );

	configDialog = new KCMultiDialog(this);
	configDialog->addModule("simongeneralconfig", QStringList() << "");
	configDialog->addModule("simonsoundconfig", QStringList() << "");
	configDialog->addModule("simonspeechmodelmanagementconfig", QStringList() << "");
	configDialog->addModule("simonmodelinternetextensionconfig", QStringList() << "");
	configDialog->addModule("simonrecognitionconfig", QStringList() << "");

	KPageWidgetItem *commandSettingsItem = configDialog->addModule("simonactionsconfig", QStringList() << "");
	if (commandSettingsItem)
	{
		KCModuleProxy *proxy = static_cast<KCModuleProxy*>(commandSettingsItem->widget());
		ActionManager::getInstance()->setConfigurationDialog(proxy->realModule());
	}


	info->writeToSplash ( i18n ( "Lade Programmlogik..." ) );
	
	this->control = new SimonControl (this);
	this->trayManager = new TrayIconManager( this);
	this->trayManager->createIcon ( KIcon("simon"), i18n("simon"));


	shownDialogs = 0;
	QMainWindow ( parent,flags );
	qApp->setQuitOnLastWindowClosed(false);
	ui.setupUi ( this );
	
	ui.tbWelcome->setWindowIcon(KIcon("simon"));
	
	ui.inlineView->addTab(ui.tbWelcome, KIcon("simon"), i18n("Willkommen"));

	statusBar()->insertItem(i18n("Nicht Verbunden"),0);
	statusBar()->insertItem("",1,10);
	statusBar()->insertPermanentWidget(2,ui.pbProgress);
	
	connect(WordListManager::getInstance(), SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
	connect(WordListManager::getInstance(), SIGNAL(progress(int,int)), this, SLOT(displayProgress(int, int)));

	//Preloads all Dialogs
	guessChildTriggers ( ( QObject* ) this );

	info->writeToSplash ( i18n ( "Lade \"Trainieren\"..." ) );
	this->trainDialog = TrainingView::getInstance ();

	info->writeToSplash ( i18n ( "Lade \"Wortliste\"..." ) );
	this->wordList = new WordListView ( this );

	info->writeToSplash ( i18n ( "Lade \"Wort hinzufügen\"..." ) );
	this->addWordView = AddWordView::getInstance();

	info->writeToSplash ( i18n ( "Lade \"Ausführen\"..." ) );
	this->runDialog = new RunCommandView ( this );


	info->writeToSplash ( i18n ( "Lade Oberfläche..." ) );
	
	settingsShown=false;
	
	setupActions();

	setupSignalSlots();

	//hiding splash again after loading
	info->hideSplash();

	show();
	QCoreApplication::processEvents();

	//switches, if the settings are shown or not
	hideSettings();

	ui.lbWelcomeDesc->setPixmap(QPixmap(KStandardDirs::locate("appdata", "themes/default/welcomebanner.png")));
	ui.lbWarning->setStyleSheet("background-image: url(\""+KStandardDirs::locate("appdata", "themes/default/alphawarning.png")+"\"); padding-left:120px; padding-top:10px");

	ui.label_5->setPixmap(KIcon("mail-message-new").pixmap(QSize(24,24)));
	ui.label_7->setPixmap(KIcon("applications-internet").pixmap(QSize(24,24)));
	ui.label_9->setPixmap(KIcon("applications-internet").pixmap(QSize(24,24)));
	ui.label_13->setPixmap(KIcon("applications-internet").pixmap(QSize(24,24)));
}

void SimonView::setupActions()
{
	KAction* connectActivate = new KAction(this);
	connectActivate->setCheckable(true);
	connectActivate->setText(i18n("Verbinden"));
	connectActivate->setIcon(KIcon("network-disconnect"));
	connectActivate->setShortcut(Qt::CTRL + Qt::Key_C);
	actionCollection()->addAction("connectActivate", connectActivate);
	connect(connectActivate, SIGNAL(triggered(bool)),
		this, SLOT(toggleConnection()));
	this->trayManager->addAction("connectActivate", connectActivate);
	
	KAction* addWord = new KAction(this);
	addWord->setText(i18n("Wort Hinzufügen"));
	addWord->setIcon(KIcon("list-add"));
	addWord->setShortcut(Qt::CTRL + Qt::Key_N);
	actionCollection()->addAction("addword", addWord);
	connect(addWord, SIGNAL(triggered(bool)),
		this, SLOT(showAddWordDialog()));
	
	KAction* train = new KAction(this);
// 	train->setCheckable(true);
	train->setText(i18n("Trainieren"));
	train->setIcon(KIcon("view-pim-news"));
	train->setShortcut(Qt::CTRL + Qt::Key_T);
	actionCollection()->addAction("train", train);
	connect(train, SIGNAL(triggered(bool)),
		this, SLOT(showTrainDialog()));
	
	KAction* commands = new KAction(this);
// 	commands->setCheckable(true);
	commands->setText(i18n("Kommandos"));
	commands->setIcon(KIcon("system-run"));
	commands->setShortcut(Qt::CTRL + Qt::Key_K);
	actionCollection()->addAction("commands", commands);
	connect(commands, SIGNAL(triggered(bool)),
		this, SLOT(showRunDialog()));
	
	KAction* wordlist = new KAction(this);
// 	wordlist->setCheckable(true);
	wordlist->setText(i18n("Wortliste"));
	wordlist->setIcon(KIcon("format-justify-fill"));
	wordlist->setShortcut(Qt::CTRL + Qt::Key_L);
	actionCollection()->addAction("wordlist", wordlist);
	connect(wordlist, SIGNAL(triggered(bool)),
		this, SLOT(showWordListDialog()));
	
	KAction* recompile = new KAction(this);
	recompile->setText(i18n("Modell erstellen"));
	recompile->setIcon(KIcon("view-refresh"));
	recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
	actionCollection()->addAction("compileModel", recompile);
	connect(recompile, SIGNAL(triggered(bool)),
		control, SLOT(compileModel()));
	
	KAction* systemMode = new KAction(this);
	systemMode->setCheckable(true);
	systemMode->setText(i18n("Verwaltungsmodus"));
	systemMode->setIcon(KIcon("system-lock-screen"));
	systemMode->setShortcut(Qt::CTRL + Qt::Key_A);
	actionCollection()->addAction("systemMode", systemMode);
	connect(systemMode, SIGNAL(triggered(bool)),
		this, SLOT(checkSettingState()));
	

	actionCollection()->addAction(KStandardAction::Preferences, "configuration",
                               this, SLOT(showSystemDialog()));

	KStandardAction::quit(this, SLOT(closeSimon()),
			      actionCollection());
	
	setupGUI();
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void SimonView::setupSignalSlots()
{
	//Setting up Signal/Slots
	
	QObject::connect ( control,SIGNAL ( guiAction ( QString ) ), ui.inlineView,SIGNAL ( guiAction ( QString ) ) );
	connect ( control, SIGNAL(guiAction(QString)), this, SLOT(doAction(QString)));
	connect ( control, SIGNAL(systemStatusChanged(SimonControl::SystemStatus)), this, SLOT(representState(SimonControl::SystemStatus)));
	connect ( control, SIGNAL(statusInfo(const QString&)), this, SLOT(displayStatus(const QString&)));
	connect ( control, SIGNAL(statusError(const QString&)), this, SLOT(displayError(const QString&)));
	connect ( control, SIGNAL(progressInfo(int, int)), this, SLOT(displayProgress(int, int)));

	connect ( addWordView, SIGNAL ( addedWord() ), wordList,
	          SLOT ( reloadList() ) );
	connect ( trainDialog, SIGNAL ( trainingCompleted() ), wordList,
	          SLOT ( reloadList() ) );
}

void SimonView::displayStatus(const QString &status)
{
	statusBar()->changeItem(status, 1);
}

void SimonView::displayConnectionStatus(const QString &status)
{
	statusBar()->changeItem(status, 0);
}

void SimonView::displayProgress(int cur, int max)
{
	ui.pbProgress->setMaximum(max);
	if (max != -1)
		ui.pbProgress->setValue(cur);
}


void SimonView::toggleConnection()
{
	SimonControl::SystemStatus status = control->getStatus();
	
	if (status==SimonControl::Disconnected)
	{
		this->control->connectToServer();
	} else if (status==SimonControl::Connecting)
	{
		this->control->abortConnecting();
	} else control->disconnectFromServer();
}



/**
 * \brief An error occured
 *
 * @author Peter Grasch
 *
 * \param  error
 * The error that occurred
 */
void SimonView::displayError ( const QString& error )
{
	KMessageBox::error ( this, error );
}


/**
 * @brief Shows the Run Dialog
 *
 * @author Peter Grasch
 */
void SimonView::showRunDialog ()
{
	ui.inlineView->toggleDisplay(runDialog);
}


/**
 * @brief Shows a dialog to add a new Word to the model
 *
 * @author Peter Grasch
 */
void SimonView::showAddWordDialog ( )
{
	if ( !addWordView->isVisible() )
		this->addWordView->show();
	else
		this->addWordView->hide();
}


/**
 * @brief Shows a dialog to configure simon
 *
 * @author Peter Grasch
 */
void SimonView::showSystemDialog ()
{
	configDialog->show();
}

/**
 * @brief Shows a dialog to train the language model
 *
 * @author Peter Grasch
 */
void SimonView::showTrainDialog ()
{
	ui.inlineView->toggleDisplay(trainDialog);
}

/**
 * @brief Shows a dialog to Control the Laguage Model
 *
 * @author Peter Grasch
 */
void SimonView::showWordListDialog ()
{
	ui.inlineView->toggleDisplay(wordList);
}


/**
 * @brief Hide the main window
 *
 * Hides the main window and sends it to the system tray.
 *
 *	@author Peter Grasch
 *
*/
void SimonView::hideSimon()
{
	shownDialogs=0;
	currentPos = pos();

	if ( this->addWordView->isVisible() )
	{
		this->shownDialogs = shownDialogs | sAddWordView;
		addWordDlgPos = addWordView->pos();
		this->addWordView->hide();
	}
	hide();
}


/**
 * @brief Shows the main Window
 *
 * Shows the Main Window after it was hidden at first
 *
 *	@author Peter Grasch
 *
 */
void SimonView::showSimon()
{
	move ( currentPos );
	show();
	if ( shownDialogs & sAddWordView )
	{
		addWordView->show();
		addWordView->move ( addWordDlgPos );
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
}

/**
 * \brief Make the widgets represent the current state (connected / disconnected, active/inactive)
 *
 * \author Peter Grasch
 */
void SimonView::representState(SimonControl::SystemStatus status)
{
	QAction *connectActivate = actionCollection()->action("connectActivate");
	switch (status)
	{
		case SimonControl::Disconnected: {
			displayConnectionStatus(i18n("Getrennt"));
			
			if (connectActivate) {
				connectActivate->setText(i18n ( "Verbinden" ));
				connectActivate->setChecked(false);
				connectActivate->setIcon(KIcon("network-disconnect"));
			}

			SimonInfo::showMessage ( i18n ( "Verbindung zu Server getrennt" ), 4000 );
			//TODO: we should probably (configurably) try to reconnect at this point
			
			break; }
			
		case SimonControl::Connecting: {
			QString connectionStr = i18n("Verbinde...");
			
			if (connectActivate) {
				connectActivate->setText(connectionStr);
				connectActivate->setChecked(true);
				connectActivate->setIcon(KIcon("network-disconnect"));
			}
			displayConnectionStatus(connectionStr);
			
			break; }
			
		case SimonControl::ConnectedDeactivated: {
			displayConnectionStatus(i18n("Verbunden aber Deaktiviert"));
			
			if (connectActivate) {
				connectActivate->setText(i18n ( "Aktivieren" ));
				connectActivate->setChecked(false);
				connectActivate->setIcon(KIcon("media-playback-pause"));
			}
			
				
			SimonInfo::showMessage ( i18n ( "simon wurde deaktiviert" ), 2000 );
				
			this->trayManager->createIcon ( KIcon ( KIconLoader().loadIcon("simon", KIconLoader::Panel, KIconLoader::SizeMedium, KIconLoader::DisabledState) ), i18n ( "Simon - Deaktiviert" ) );
			repaint();
			break; }
			
		case SimonControl::ConnectedActivated: {
			displayConnectionStatus(i18n("Verbunden und Aktiviert"));
			
			if (connectActivate)
			{
				connectActivate->setText(i18n ( "Aktiviert" ));
				connectActivate->setChecked(true);
				connectActivate->setIcon(KIcon("network-connect"));
			}
			
			this->trayManager->createIcon ( KIcon ( "simon" ), "Simon" );
				
			SimonInfo::showMessage ( i18n ( "simon wurde aktiviert" ), 2000 );
			break; }
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
	if ( ! ( this->isHidden() ) )
		this->hideSimon();
	else this->showSimon();
}


/**
 * @brief Close the main window
 *
 * Closes the whole program and kills all dialogs associated with it
 *
 * @author Peter Grasch
 *
*/
void SimonView::closeSimon()
{
	if ( ( !control->askBeforeQuit() ) || ( KMessageBox::questionYesNo ( this, i18n ( "Wirklich beenden?" ), i18n ( "Ein beenden der Applikation wird die Verbindung zur Erkennung beenden und weder Diktatfunktionen noch andere Kommandos können mehr benutzt werden.\n\nWollen Sie wirklich beenden?" )) == KMessageBox::Yes ) )
	{
		close();
		qApp->quit();
	}
}


/**
 * @brief sender request
 * it differs if the sender is pbClose, or the CloseButton in the title bar
 *
 * \param *event
 * just to comply with the original definition in QObject
 *
 *	@author Phillip Goriup, Peter Grasch
*/
void SimonView::closeEvent ( QCloseEvent * event )
{
	this->hideSimon();
	event->ignore();
}

/**
 * @brief If the user wants to set any settings, he/she has to give a password to show all settings.
 *
 *	@author Susanne Tschernegg
*/
void SimonView::checkSettingState()
{
	if ( !settingsShown )
	{
		if ( !control->passwordProtected() || checkPassword() )
		{
			showSettings();
		} else 
			actionCollection()->action("systemMode")->setChecked(false);
	}
	else
	{
		hideSettings();
	}
}

/**
 * @brief checks the password, if it returns true, the user gaves the right password
 *
 *	@author Susanne Tschernegg
 *  @return bool
 *      returns whether the password is correct or not
*/
bool SimonView::checkPassword()
{
	KPasswordDialog dlg( this , KPasswordDialog::NoFlags );
	dlg.setPrompt( i18n( "Dieser Bereich wurde passwortgeschützt." ));
	if( !dlg.exec() )
		return false; //the user canceled
		
	return (dlg.password() == control->adminPassword());
}

/**
 * @brief the user couldn't see the setting-widgets resp. they are disabled
 *
 *	@author Susanne Tschernegg
*/
void SimonView::hideSettings()
{
	configDialog->hide();

	actionCollection()->action("configuration")->setVisible(false);

	wordList->setSettingsHidden();
	trainDialog->setSettingsHidden();
	runDialog->setSettingsHidden();
	
	settingsShown=false;
}

/**
 * @brief to set some settings, the widgets must be shown resp. not disabled
 *
 *	@author Susanne Tschernegg
*/
void SimonView::showSettings()
{
	actionCollection()->action("configuration")->setVisible(true);

	//sets the setting buttons visible
	trainDialog->setSettingsVisible();
	wordList->setSettingsVisible();
	runDialog->setSettingsVisible();
	
	settingsShown=true;
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonView::~SimonView()
{
	Logger::log ( i18n ( "[INF] Beenden..." ) );
	trayManager->deleteLater();
	control->deleteLater();
	Logger::close();
}
