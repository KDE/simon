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
#include "scenariomanagementdialog.h"


#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>

#include <simonactionsui/runcommandview.h>
#include <trayiconmanager.h>

#include <simonprogresstracking/statusmanager.h>
#include <simonprogresstracking/compositeprogresswidget.h>

#include <simonmodelmanagementui/vocabularyview.h>

#include <speechmodelmanagement/scenariomanager.h>

#include <simonmodelmanagementui/trainingview.h>
#include <simonmodelmanagementui/grammarview.h>
#include <simonmodelmanagementui/AddWord/addwordview.h>
#include <simonactions/actionmanager.h>
#include <speechmodelmanagement/scenario.h>

#include <QTimer>
#include <QPixmap>
#include <QCryptographicHash>
#include <QCloseEvent>
#include <QMenu>
#include <QThread>
#include <KToolBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>


#include <KMessageBox>
#include <KApplication>
#include <KAction>
#include <KToolBarPopupAction>
#include <KMenu>
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
SimonView::SimonView(QWidget* parent, Qt::WFlags flags)
		: SimonMainWindow(parent, flags),
	shownDialogs(0)
{
	Logger::log ( i18n ( "[INF] Starting simon..." ) );

	SimonInfo *info = new SimonInfo();

	//showing splash
	Logger::log ( i18n ( "[INF] Displaying Splashscreen..." ) );
	info->showSplash();
	info->writeToSplash ( i18n ( "Loading core..." ) );

	control = (new SimonControl(this));
	trayManager = (new TrayIconManager(this));
	configDialog = (new KCMultiDialog(this));

	Logger::log ( i18n ( "[INF] Loading configuration modules..." ) );

	configDialog->addModule("simongeneralconfig", QStringList() << "");
	configDialog->addModule("simonsoundconfig", QStringList() << "");
	configDialog->addModule("simonspeechmodelmanagementconfig", QStringList() << "");
	configDialog->addModule("simonmodelinternetextensionconfig", QStringList() << "");
	configDialog->addModule("simonrecognitionconfig", QStringList() << "");
	configDialog->addModule("simonsynchronisationconfig", QStringList() << "");

	KPageWidgetItem *commandSettingsItem = configDialog->addModule("simonactionsconfig", QStringList() << "");
	if (commandSettingsItem)
	{
		KCModuleProxy *proxy = static_cast<KCModuleProxy*>(commandSettingsItem->widget());
		ActionManager::getInstance()->setConfigurationDialog(proxy->realModule());
		ActionManager::getInstance()->setMainWindow(this);
		ActionManager::getInstance()->init();
	}


	this->trayManager->createIcon ( KIcon ( KIconLoader().loadIcon("simon", KIconLoader::Panel, KIconLoader::SizeMedium, KIconLoader::DisabledState) ), i18n ( "simon - Deactivated" ) );


	QMainWindow ( parent,flags );
	qApp->setQuitOnLastWindowClosed(false);
	ui.setupUi ( this );
#if KDE_IS_VERSION(4,0,80)
	ui.inlineView->setCloseButtonEnabled(true);
#endif
	
	ui.tbWelcome->setWindowIcon(KIcon("simon"));
	
	ui.inlineView->addTab(ui.tbWelcome, KIcon("simon"), i18n("Welcome"));

	statusBar()->insertItem(i18n("Not connected"),0);
	statusBar()->insertItem("",1,10);
	statusBar()->insertPermanentWidget(2,StatusManager::global(this)->createWidget(this));
	

	//Preloads all Dialogs
	guessChildTriggers ( ( QObject* ) this );

	info->writeToSplash ( i18n ( "Loading \"Training\"..." ) );
	this->trainDialog = new TrainingView(this);
	ScenarioManager::getInstance()->registerScenarioDisplay(trainDialog);

	info->writeToSplash ( i18n ( "Loading \"Wordlist\"..." ) );
	vocabularyView = new VocabularyView(this);
	ScenarioManager::getInstance()->registerScenarioDisplay(vocabularyView);

	info->writeToSplash ( i18n ( "Loading \"Grammar\"..." ) );
	this->grammarView = new GrammarView(this);
	ScenarioManager::getInstance()->registerScenarioDisplay(grammarView);

	info->writeToSplash ( i18n ( "Loading \"Run\"..." ) );
	this->runDialog = new RunCommandView ( this );
	ScenarioManager::getInstance()->registerScenarioDisplay(runDialog);


	info->writeToSplash ( i18n ( "Loading Interface..." ) );
	
	settingsShown=false;

	cbCurrentScenario = new QComboBox(this);

	displayScenarios();
	updateScenarioDisplays();
	
	setupActions();

	setupSignalSlots();

	control->actOnAutoConnect();

	//hiding splash again after loading
	info->hideSplash();
	delete info;

	ui.lbWelcomeDesc->setPixmap(QPixmap(KStandardDirs::locate("appdata", "themes/default/welcomebanner.png")));
	ui.lbWarning->setStyleSheet("background-image: url(\""+KStandardDirs::locate("appdata", "themes/default/alphawarning.png")+"\"); padding-left:120px; padding-top:10px");

	ui.label_5->setPixmap(KIcon("mail-message-new").pixmap(QSize(24,24)));
	ui.label_7->setPixmap(KIcon("applications-internet").pixmap(QSize(24,24)));
	ui.label_9->setPixmap(KIcon("applications-internet").pixmap(QSize(24,24)));
	ui.label_13->setPixmap(KIcon("applications-internet").pixmap(QSize(24,24)));

	if (!control->startMinimized())
		show();
}

void SimonView::displayScenarios()
{
	cbCurrentScenario->clear();

	QList<Scenario*> scenarioList = ScenarioManager::getInstance()->getScenarios();
	foreach (Scenario* s, scenarioList) {
		cbCurrentScenario->addItem(s->icon(), s->name(), s->id());
	}
}



void SimonView::updateScenarioDisplays()
{
	//a scenario has been selected from the list of loaded scenarios
	int currentIndex = cbCurrentScenario->currentIndex();
	kDebug() << currentIndex;
	if (currentIndex == -1) return;

	QString currentId = cbCurrentScenario->itemData(currentIndex).toString();
	Scenario *scenario = ScenarioManager::getInstance()->getScenario(currentId);
	kDebug() << "Scenario " << scenario;
	if (!scenario) {
		KMessageBox::error(this, i18n("Couldn't retrieve Scenario \"%1\"", currentId));
		return;
	}
	ScenarioManager::getInstance()->updateDisplays(scenario, true);
}

void SimonView::setupActions()
{
	connectAction = new KAction(this);
	connectAction->setText(i18n("Connect"));
	connectAction->setCheckable(true);
	connectAction->setIcon(KIcon("network-disconnect"));
	connect(connectAction, SIGNAL(triggered(bool)),
		this, SLOT(toggleConnection()));
	actionCollection()->addAction("connect", connectAction);
	this->trayManager->addAction("connect", connectAction);

	activateAction = new KAction(this);
	activateAction->setText(i18n("Activate"));
	activateAction->setIcon(KIcon("media-playback-start"));
	activateAction->setCheckable(true);
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(toggleActivation()));
	this->trayManager->addAction("activate", activateAction);
	activateAction->setEnabled(false);
	actionCollection()->addAction("activate", activateAction);

	disconnectAction = new KAction(this);
	disconnectAction->setText(i18n("Disconnect"));
	disconnectAction->setIcon(KIcon("network-disconnect"));
	connect(disconnectAction, SIGNAL(triggered(bool)),
		control, SLOT(disconnectFromServer()));

	KToolBarPopupAction* connectActivate = new KToolBarPopupAction(KIcon("network-disconnect"), i18n("Connect"), this);
	connectActivate->setCheckable(true);
	connectActivate->setShortcut(Qt::CTRL + Qt::Key_C);

	actionCollection()->addAction("connectActivate", connectActivate);
	connect(connectActivate, SIGNAL(triggered(bool)),
		this, SLOT(toggleConnection()));
	
	KAction* addWord = new KAction(this);
	addWord->setText(i18n("Add Word"));
	addWord->setIcon(KIcon("list-add"));
	addWord->setShortcut(Qt::CTRL + Qt::Key_N);
	actionCollection()->addAction("addword", addWord);
	connect(addWord, SIGNAL(triggered(bool)),
		this, SLOT(showAddWordDialog()));
	
	KAction* train = new KAction(this);
	train->setText(i18n("Training"));
	train->setIcon(KIcon("view-pim-news"));
	train->setShortcut(Qt::CTRL + Qt::Key_T);
	actionCollection()->addAction("train", train);
	connect(train, SIGNAL(triggered(bool)),
		this, SLOT(showTrainDialog()));
	
	KAction* commands = new KAction(this);
	commands->setText(i18n("Commands"));
	commands->setIcon(KIcon("system-run"));
	commands->setShortcut(Qt::CTRL + Qt::Key_K);
	actionCollection()->addAction("commands", commands);
	connect(commands, SIGNAL(triggered(bool)),
		this, SLOT(showRunDialog()));
	
	KAction* wordlist = new KAction(this);
	wordlist->setText(i18n("Wordlist"));
	wordlist->setIcon(KIcon("format-justify-fill"));
	wordlist->setShortcut(Qt::CTRL + Qt::Key_L);
	actionCollection()->addAction("wordlist", wordlist);
	connect(wordlist, SIGNAL(triggered(bool)),
		this, SLOT(showWordListDialog()));
	
	KAction* grammar = new KAction(this);
	grammar->setText(i18n("Grammar"));
	grammar->setIcon(KIcon("applications-education-language"));
	grammar->setShortcut(Qt::CTRL + Qt::Key_G);
	actionCollection()->addAction("grammar", grammar);
	connect(grammar, SIGNAL(triggered(bool)),
		this, SLOT(showGrammarDialog()));
	
	KAction* recompile = new KAction(this);
	recompile->setEnabled(control->getStatus() != SimonControl::Disconnected);
	recompile->setText(i18n("Synchronize"));
	recompile->setIcon(KIcon("view-refresh"));
	recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
	actionCollection()->addAction("compileModel", recompile);
	connect(recompile, SIGNAL(triggered(bool)),
		control, SLOT(compileModel()));
	
	//QLabel *lbDesc = new QLabel(i18n("Scenario: "), this);
	KAction* scenarioLabel = new KAction(this);
	scenarioLabel->setText(i18n("Scenario:"));
	actionCollection()->addAction("scenarioLabel", scenarioLabel);


	KAction* currentScenarioAction = new KAction(this);
	currentScenarioAction->setText(i18n("Synchronize"));
	currentScenarioAction->setDefaultWidget(cbCurrentScenario);
	actionCollection()->addAction("selectScenario", currentScenarioAction);


	KAction* manageScenariosAction = new KAction(this);
	manageScenariosAction->setText(i18n("Manage scenarios"));
	manageScenariosAction->setIcon(KIcon("view-choose"));
	actionCollection()->addAction("manageScenarios", manageScenariosAction);
	connect(manageScenariosAction, SIGNAL(triggered(bool)),
		this, SLOT(manageScenarios()));
	
	actionCollection()->addAction(KStandardAction::Preferences, "configuration",
                               this, SLOT(showSystemDialog()));

	KStandardAction::quit(this, SLOT(closeSimon()),
			      actionCollection());

	setupGUI();

	ActionManager::getInstance()->publishGuiActions();
}

void SimonView::manageScenarios()
{
//	KMessageBox::information(this, i18n("This is not yet implemented, sorry!"));
	ScenarioManagementDialog *dlg = new ScenarioManagementDialog(this);
	if (dlg->exec()) {
		//reload scenario information
		if (!ScenarioManager::getInstance()->setupScenarios())
			KMessageBox::sorry(this, i18n("Couldn't re-initialize scenarios. Please restart simon!"));

		displayScenarios();
		updateScenarioDisplays();
	}
	dlg->deleteLater();
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

	connect(trainDialog, SIGNAL(execd()), this, SLOT(showTrainDialog()));
	connect(cbCurrentScenario, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScenarioDisplays()));
}

void SimonView::displayConnectionStatus(const QString &status)
{
	statusBar()->changeItem(status, 0);
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
	AddWordView *addWordView = new AddWordView();
	addWordView->show();
	connect(addWordView, SIGNAL(finished(int)), addWordView, SLOT(deleteLater()));
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
	ui.inlineView->toggleDisplay(vocabularyView);
}

/**
 * @brief Shows a dialog to Control the Laguage Model
 *
 * @author Peter Grasch
 */
void SimonView::showGrammarDialog ()
{
	ui.inlineView->toggleDisplay(grammarView);
}


/**
 * @brief Toggles the activation state
 *
 * Tells the Control-Layer to toggle the activation state - according to his
 * response the following (re)actions are taken:
 * 	desaturate the Tray Image as needed
 * 	desaturate the simon logo in the bg
 * 	replaces the text on the label to "Deaktivieren"/"Activate"
 *
 *	@author Peter Grasch
 *
 */
void SimonView::toggleActivation()
{
	if (control->getStatus() == SimonControl::ConnectedDeactivatedNotReady)
	{
		KMessageBox::error(this, i18n("Couldn't start recognition because the system reports that the recognition is not ready.\n\nPlease check if you have defined a wordlist, an appropriate grammar and recorded a few trainings samples.\n\nThe system will then, upon synchronization, generate the model which will be used for the recognition."));
		representState(control->getStatus());
	} else
		this->control->toggleActivition();
}

/**
 * \brief Make the widgets represent the current state (connected / disconnected, active/inactive)
 *
 * \author Peter Grasch
 */
void SimonView::representState(SimonControl::SystemStatus status)
{
	guiUpdateMutex.lock();
	KToolBarPopupAction *connectActivate = dynamic_cast<KToolBarPopupAction*>(actionCollection()->action("connectActivate"));
	QAction *compileAction = actionCollection()->action("compileModel");
	if (compileAction) 
		compileAction->setEnabled(status != SimonControl::Disconnected);
	switch (status)
	{
		case SimonControl::Disconnected: {
			displayConnectionStatus(i18n("Disconnected"));
			
			if (connectActivate) {
				connectActivate->setText(i18n ( "Connect" ));
				connectActivate->setChecked(false);
				connectActivate->setIcon(KIcon("network-disconnect"));
				if (connectActivate->menu()->actions().contains(disconnectAction))
					connectActivate->menu()->removeAction(disconnectAction);

				disconnect(connectActivate,0,0,0);
				connect(connectActivate, SIGNAL(triggered(bool)),
					this, SLOT(toggleConnection()));
			}

			SimonInfo::showMessage ( i18n ( "Connection to server lost" ), 4000 );
			//TODO: we should probably (configurably) try to reconnect at this point
			activateAction->setEnabled(false);
			activateAction->setText(i18n("Activate"));
			activateAction->setIcon(KIcon("media-playback-start"));
			activateAction->setChecked(false);

			connectAction->setText(i18n("Connect"));
			connectAction->setChecked(false);
			connectAction->setIcon(KIcon("network-disconnect"));
			break; }
			
		case SimonControl::Connecting: {
			QString connectionStr = i18n("Connecting...");
			
			if (connectActivate) {
				connectActivate->setText(connectionStr);
				connectActivate->setChecked(true);
				connectActivate->setIcon(KIcon("network-disconnect"));

				disconnect(connectActivate,0,0,0);
				connect(connectActivate, SIGNAL(triggered(bool)),
					this, SLOT(toggleConnection()));
			}
			displayConnectionStatus(connectionStr);
			if (connectActivate->menu()->actions().contains(disconnectAction))
				connectActivate->menu()->removeAction(disconnectAction);

			connectAction->setText(connectionStr);
			connectAction->setChecked(true);
			connectAction->setIcon(KIcon("network-disconnect"));

			break; }
		
		case SimonControl::ConnectedDeactivating: {
			displayConnectionStatus(i18n("Connected: Deactivating..."));
			if (connectActivate) {
				connectActivate->setText(i18n ( "Deactivating..." ));
				connectActivate->setChecked(false);
			}
			activateAction->setEnabled(true);
			activateAction->setText("Deactivating...");
			activateAction->setChecked(false);
			break;
		}
		
		case SimonControl::ConnectedDeactivatedNotReady: 
		case SimonControl::ConnectedPaused: 
		case SimonControl::ConnectedDeactivatedReady: {
			displayConnectionStatus(i18n("Connected but Deactivated"));
			
			if (connectActivate) {
				connectActivate->setText(i18n ( "Activate" ));
				connectActivate->setChecked(false);
				connectActivate->setIcon(KIcon("media-playback-start"));

				disconnect(connectActivate,0,0,0);
				connect(connectActivate, SIGNAL(triggered(bool)),
					this, SLOT(toggleActivation()));
					
				//add disconnect action with icon network-disconnect
				if (!connectActivate->menu()->actions().contains(disconnectAction))
					connectActivate->menu()->addAction(disconnectAction);
			}
			
			activateAction->setEnabled(true);
			activateAction->setText(i18n("Activate"));
			activateAction->setChecked(false);
			activateAction->setIcon(KIcon("media-playback-start"));
			connectAction->setText("Connected");
			connectAction->setIcon(KIcon("network-connect"));
			connectAction->setChecked(true);

			
			SimonInfo::showMessage ( i18n ( "simon has been deactivated" ), 2000 );
				
			this->trayManager->createIcon ( KIcon ( KIconLoader().loadIcon("simon", KIconLoader::Panel, KIconLoader::SizeMedium, KIconLoader::DisabledState) ), i18n ( "simon - Deactivated" ) );
			//repaint();
			break; }
		
		case SimonControl::ConnectedResuming: 
		case SimonControl::ConnectedActivating: {
			displayConnectionStatus(i18n("Connected; Activating..."));
			if (connectActivate) {
				connectActivate->setText(i18n ( "Activating..." ));
				connectActivate->setChecked(false);
			}
			activateAction->setText(i18n("Activating..."));
			activateAction->setChecked(false);
		}
			
		case SimonControl::ConnectedActivated: {
			displayConnectionStatus(i18n("Connected and Activated"));
			
			if (connectActivate)
			{
				connectActivate->setText(i18n ( "Activated" ));
				connectActivate->setChecked(true);
				connectActivate->setIcon(KIcon("media-playback-start"));

				disconnect(connectActivate,0,0,0);
				connect(connectActivate, SIGNAL(triggered(bool)),
					this, SLOT(toggleActivation()));

				if (!connectActivate->menu()->actions().contains(disconnectAction))
					connectActivate->menu()->addAction(disconnectAction);
			}
			
			activateAction->setText(i18n("Activated"));
			activateAction->setChecked(true);
			activateAction->setIcon(KIcon("media-playback-start"));

			this->trayManager->createIcon ( KIcon ( "simon" ), "Simon" );
				
			SimonInfo::showMessage ( i18n ( "simon has been activated" ), 2000 );
			break; }
	}
	guiUpdateMutex.unlock();
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
//	kWarning() << KMessageBox::questionYesNoCancel ( this, i18n ( "If you quit the application the connection to the server will be closed and "
//"you will no longer be able to dictate texts or use command.\n\nDo you want to "
//"quit?"), QString(), KStandardGuiItem::yes(), KStandardGuiItem::no(), KStandardGuiItem::cancel(), "askForQuitSimonMainWindow" );

	if ( KMessageBox::questionYesNoCancel ( this, i18n ( "If you quit the application the connection to the server will be closed and "
"you will no longer be able to dictate texts or use command.\n\nDo you want to "
"quit?"), QString(), KStandardGuiItem::yes(), KStandardGuiItem::no(), KStandardGuiItem::cancel(), "askForQuitSimonMainWindow" ) == KMessageBox::Yes )
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
	hide();
	event->ignore();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonView::~SimonView()
{
	Logger::log ( i18n ( "[INF] Quitting..." ) );
	delete trayManager;
	delete control;
	Logger::close();
}
