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
#include "firstrunwizard.h"
#include "welcomepage.h"
#include "version.h"

#include <simonuicomponents/inlinewidgetview.h>

#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>

#include <simonactions/commandsettings.h>
#include <simonactionsui/runcommandview.h>
#include <simoncontextui/contextview.h>
#include <simonuicomponents/trayiconmanager.h>

#include <simonprogresstracking/statusmanager.h>
#include <simonprogresstracking/compositeprogresswidget.h>

#include <simonmodelmanagementui/vocabularyview.h>

#include <simonscenarios/scenariomanager.h>

#include <simonsampleshare/sampleshare.h>

#include <simonmodelmanagementui/trainingview.h>
#include <simonmodelmanagementui/grammarview.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/actioncollection.h>

#include <simonsound/soundserver.h>

#include <QTimer>
#include <QFile>
#include <QPixmap>
#include <QCryptographicHash>
#include <QCloseEvent>
#include <QMenu>
#include <QThread>
#include <KToolBar>
#include <QLabel>
#include <QHBoxLayout>
#include <KDE/KComboBox>
#include <QDesktopServices>
#include <QTimeLine>
#include <QWebView>

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
#include <KCMultiDialog>
#include <KCModuleProxy>
#include <KPageWidgetItem>
#include <KIconLoader>
#include <KCmdLineArgs>
#include <KColorScheme>

/**
 * @brief Constructor
 *
 * This is the constructor of the main-UI class - the Simon View class.
 * It displays a little splash screen and initializes the member-variables
 *
 * @author Peter Grasch
 * @param Qwidget *parent
 * The parent which is passed on to the QMainWindow initialization - Default: 0
 * @param Qt::WFlags flags
 * The flags which are also passed on to the QMainWindow constructor - as before: Default: 0
 *
 */
SimonView::SimonView(QWidget* parent, Qt::WFlags flags)
: KXmlGuiWindow(parent, flags), ScenarioDisplay(),
  backButtonAnimation(new QTimeLine(700, this))
{
  Logger::log ( i18n ( "Starting Simon..." ) );

  //showing splash
  bool showSplash = KCmdLineArgs::parsedArgs()->isSet("splash");
  SimonInfo *info = 0;

  if (showSplash) {
    info = new SimonInfo();
    Logger::log ( i18n ( "Displaying Splashscreen..." ) );
    info->showSplash();
    info->writeToSplash ( i18n ( "Loading core..." ) );
  }

  KGlobal::locale()->insertCatalog("simonlib");

  control = (new SimonControl(this));

  if (!control->firstRunWizardCompleted()) {
    QPointer<FirstRunWizard> firstRun = new FirstRunWizard(this);
    bool firstRunWizardCompleted = firstRun->exec();
    delete firstRun;

    if (firstRunWizardCompleted || KMessageBox::questionYesNo(this, i18n("You did not complete the initial configuration. Simon will continue with default values.\n\nDo you want Simon to display the wizard again on the next start?"))==KMessageBox::No)
      control->setFirstRunWizardCompleted(true);
  }

  trayManager = new TrayIconManager(this);

  this->trayManager->createIcon ( KIcon ( KIconLoader().loadIcon("simon", KIconLoader::Panel, KIconLoader::SizeMedium, KIconLoader::DisabledState) ), i18n ( "Simon - Deactivated" ) );

  QMainWindow ( parent,flags );
  qApp->setQuitOnLastWindowClosed(false);
  ui.setupUi ( this );
  
  statusBar()->insertItem(i18n("Not connected"),0);
  statusBar()->insertItem("",1,10);
  statusBar()->insertPermanentWidget(2,StatusManager::global(this)->createWidget(this));

  ScenarioManager::getInstance()->registerScenarioDisplay(this);
  
  //Preloads all Dialogs
  if (showSplash)
    info->writeToSplash ( i18n ( "Loading training..." ) );
  trainDialog = new TrainingView(this);
  ScenarioManager::getInstance()->registerScenarioDisplay(trainDialog);

  if (showSplash)
    info->writeToSplash ( i18n ( "Loading vocabulary..." ) );
  vocabularyView = new VocabularyView(this);
  ScenarioManager::getInstance()->registerScenarioDisplay(vocabularyView);

  if (showSplash)
    info->writeToSplash ( i18n ( "Loading grammar..." ) );
  grammarView = new GrammarView(this);
  ScenarioManager::getInstance()->registerScenarioDisplay(grammarView);

  if (showSplash)
    info->writeToSplash ( i18n ( "Loading context..." ) );
  contextDialog = new ContextView(this);
  connect(contextDialog, SIGNAL(manageScenariosTriggered()), this, SLOT(manageScenarios()));
  ScenarioManager::getInstance()->registerScenarioDisplay(contextDialog);

  if (showSplash)
    info->writeToSplash ( i18n ( "Loading run..." ) );
  runDialog = new RunCommandView(this);
  connect(runDialog, SIGNAL(actionsChanged()), this, SLOT(updateActionList()));
  ScenarioManager::getInstance()->registerScenarioDisplay(runDialog);
  kDebug() << "SoundServer: " << SoundServer::getInstance();

  if (showSplash)
    info->writeToSplash ( i18n ( "Loading interface..." ) );

  setupActions();

  setupGUI();
  displayScenarioPrivate(ScenarioManager::getInstance()->getCurrentScenario());
  
  welcomePage = new WelcomePage(actionCollection()->action("activate"));
  ScenarioManager::getInstance()->registerScenarioDisplay(welcomePage);
  connect(welcomePage, SIGNAL(editScenario()), this, SLOT(editScenario()));
  
  ui.swMain->insertWidget(0, welcomePage);
  ui.swMain->setCurrentIndex(0);
  
  ui.inlineView->registerPage(vocabularyView);
  ui.inlineView->registerPage(trainDialog);
  ui.inlineView->registerPage(grammarView);
  ui.inlineView->registerPage(contextDialog);
  ui.inlineView->registerPage(runDialog);
  
  ui.frmBackToOverview->setMaximumHeight(0);
  connect(backButtonAnimation, SIGNAL(frameChanged(int)), this, SLOT(backButtonAnimationStep(int)));
  {
    QPalette p = ui.frmBackToOverview->palette();
    p.setBrush(QPalette::Window, p.alternateBase());
    ui.frmBackToOverview->setPalette(p);
  }
  
  connect(ui.pbBackToOverview, SIGNAL(clicked()), this, SLOT(backToOverview()));
  
  setupSignalSlots();
  control->startup();

  //hiding splash again after loading
  if (showSplash) {
    info->hideSplash();
    delete info;
  }

  if (!control->startMinimized())
    show();
}

void SimonView::backButtonAnimationStep ( int step )
{
  ui.frmBackToOverview->setMaximumHeight(step);
}

void SimonView::updateActionList()
{
  unplugActionList("command_actionlist");
  plugActionList("command_actionlist", ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->getGuiActions());
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
  //must be set after addAction() because of the unique name set in addAction(name,...)
  // deactivated because of KDE bug #307225
  //activateAction->setGlobalShortcut(KShortcut(Qt::SHIFT + Qt::Key_Pause));

  disconnectAction = new KAction(this);
  disconnectAction->setText(i18n("Disconnect"));
  disconnectAction->setIcon(KIcon("network-disconnect"));
  connect(disconnectAction, SIGNAL(triggered(bool)),
    control, SLOT(disconnectFromServer()));

  KToolBarPopupAction* connectActivate = new KToolBarPopupAction(KIcon("network-disconnect"), i18n("Connect"), this);
  kDebug() << "Real action: " << connectActivate;
  connectActivate->setCheckable(true);
  connectActivate->setShortcut(Qt::CTRL + Qt::Key_C);

  actionCollection()->addAction("connectActivate", connectActivate);
  connect(connectActivate, SIGNAL(triggered(bool)),
    this, SLOT(toggleConnection()));

  
  KAction* recompile = new KAction(this);
  recompile->setEnabled(control->getStatus() != SimonControl::Disconnected);
  recompile->setText(i18n("Synchronize"));
  recompile->setIcon(KIcon("view-refresh"));
  recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
  actionCollection()->addAction("compileModel", recompile);
  connect(recompile, SIGNAL(triggered(bool)),
    control, SLOT(compileModel()));

  KAction* sendSampleShareAction = new KAction(this);
  sendSampleShareAction->setText(i18n("Contribute samples"));
  sendSampleShareAction->setIcon(KIcon("repository"));
  actionCollection()->addAction("sampleShare", sendSampleShareAction);
  connect(sendSampleShareAction, SIGNAL(triggered(bool)),this, SLOT(showSampleShare()));
  
  actionCollection()->addAction(KStandardAction::Preferences, "configuration",
    this, SLOT(showSystemDialog()));

  KStandardAction::quit(this, SLOT(closeSimon()),
    actionCollection());
}

void SimonView::editScenario()
{
  backButtonAnimation->setFrameRange(0, 35);
  backButtonAnimation->start();
  ui.swMain->setCurrentIndex(1);
}

void SimonView::backToOverview()
{
  backButtonAnimation->setFrameRange(35, 0);
  backButtonAnimation->start();
  ui.swMain->setCurrentIndex(0);
}

void SimonView::displayScenarioPrivate(Scenario *scenario)
{
  kDebug() << "displayScenario: " << scenario->id();
  
  updateActionList();
}


void SimonView::manageScenarios()
{
  //FIXME: exit scenario edit mode if we are currently in there, return to main screen and open scenario configuration dialog
}


void SimonView::setupSignalSlots()
{
  //Setting up Signal/Slots
  QObject::connect ( control,SIGNAL (guiAction(QString)), ui.inlineView,SIGNAL (guiAction(QString)) );
  connect ( control, SIGNAL(systemStatusChanged(SimonControl::SystemStatus)), this, SLOT(representState(SimonControl::SystemStatus)));
}

/**
 * \brief Sets up the dialog to send samples to Voxforge
 * \author Alessandro Buggin
 */
void SimonView::showSampleShare()
{
  SampleShare *sampleShareWidget = new SampleShare;
  sampleShareWidget->show();
  connect(sampleShareWidget, SIGNAL(finished(int)), sampleShareWidget, SLOT(deleteLater()));
}

void SimonView::displayConnectionStatus(const QString &status)
{
  statusBar()->changeItem(status, 0);
}

void SimonView::toggleConnection()
{
  SimonControl::SystemStatus status = control->getStatus();

  if (status==SimonControl::Disconnected) {
    this->control->connectToServer();
  } 
  else if (status==SimonControl::Connecting) {
    this->control->abortConnecting();
  } 
  else {
    control->disconnectFromServer();
  }
}

/**
 * \brief An error occurred
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
 * @brief Shows a dialog to configure simon
 *
 * @author Peter Grasch
 */
void SimonView::showSystemDialog ()
{
  QPointer<KCMultiDialog> configDialog(new KCMultiDialog(this));

  configDialog->addModule("simongeneralconfig", QStringList() << "");
  configDialog->addModule("simonsoundconfig", QStringList() << "");
  configDialog->addModule("simonspeechmodelmanagementconfig", QStringList() << "");
  configDialog->addModule("simonmodelextensionconfig", QStringList() << "");
  configDialog->addModule("simonrecognitionconfig", QStringList() << "");
  configDialog->addModule("simonactionsconfig", QStringList() << "");
  configDialog->addModule("simonttsconfig", QStringList() << "");
  configDialog->addModule("kcm_attica");
  configDialog->addModule("simonwebcamconfiguration");
  configDialog->exec();
  
  delete configDialog;
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
	  if (control->getStatus() == SimonControl::ConnectedDeactivatedNotReady) {
	    KMessageBox::error(this, i18n("Could not start recognition because the system reports that the recognition is not ready.\n\nPlease check if you have defined a vocabulary, an appropriate grammar and recorded a few training samples.\n\nThe system will then, upon synchronization, generate the model which will be used for the recognition."));
	    representState(control->getStatus());
	  } 
	  else {
	    this->control->toggleActivition();
	  }
	}

	void SimonView::setActivation(bool active)
	{
	  if (active)
	    control->activateSimon();
	  else
	    control->deactivateSimon();
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
	  switch (status) {
	    case SimonControl::Disconnected:
	    {
	      displayConnectionStatus(i18nc("Disconnected from the server", "Disconnected"));

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

	      SimonInfo::showMessage ( i18n ( "Connection to server lost" ), 4000 ); // krazy:exclude=qmethods
	      
	      activateAction->setEnabled(false);
	      activateAction->setText(i18n("Activate"));
	      activateAction->setIcon(KIcon("media-playback-start"));
	      activateAction->setChecked(false);

	      if (trayManager)
		trayManager->createIcon ( KIcon ( KIconLoader().loadIcon("simon", KIconLoader::Panel, KIconLoader::SizeMedium, KIconLoader::DisabledState) ), i18n ( "Simon - Deactivated" ) );
      connectAction->setText(i18n("Connect"));
      connectAction->setChecked(false);
      connectAction->setIcon(KIcon("network-disconnect"));
      break;
    }

    case SimonControl::Connecting:
    {
      QString connectionStr = i18nc("Connecting to the server", "Connecting...");

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

      break;
    }

    case SimonControl::ConnectedDeactivating:
    {
      displayConnectionStatus(i18n("Connected: Deactivating..."));
      if (connectActivate) {
        connectActivate->setText(i18n("Deactivating..."));
        connectActivate->setChecked(false);
      }
      activateAction->setEnabled(true);
      activateAction->setText("Deactivating...");
      activateAction->setChecked(false);
      break;
    }

    case SimonControl::ConnectedDeactivatedNotReady:
    case SimonControl::ConnectedPaused:
    case SimonControl::ConnectedDeactivatedReady:
    {
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
      connectAction->setText(i18n("Connected"));
      connectAction->setIcon(KIcon("network-connect"));
      connectAction->setChecked(true);

      SimonInfo::showMessage ( i18n ( "Simon has been deactivated" ), 2000 );

      this->trayManager->createIcon ( KIcon ( KIconLoader().loadIcon("simon", KIconLoader::Panel, KIconLoader::SizeMedium, KIconLoader::DisabledState) ), i18n ( "simon - Deactivated" ) );
      //repaint();
      break;
    }

    case SimonControl::ConnectedResuming:
    case SimonControl::ConnectedActivating:
    {
      displayConnectionStatus(i18n("Connected: Activating..."));
      if (connectActivate) {
        connectActivate->setText(i18n("Activating..."));
        connectActivate->setChecked(false);
      }
      activateAction->setText(i18n("Activating..."));
      activateAction->setChecked(false);
    }

    case SimonControl::ConnectedActivated:
    {
      displayConnectionStatus(i18n("Connected and Activated"));

      if (connectActivate) {
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

      SimonInfo::showMessage ( i18n ( "Simon has been activated" ), 2000 );

      break;
    }
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
  if (KMessageBox::questionYesNo(this,
        i18n ("If you quit the application "
        "you will no longer be able to dictate "
        "texts or use commands.\n\nDo you want to quit?"),
        QString(), KStandardGuiItem::yes(), KStandardGuiItem::cancel(),
        "AskForQuitSimonMainWindow")
      == KMessageBox::Yes) {
    close();
    qApp->quit();
  }
}


/**
 * \param *event
 * just to comply with the original definition in QObject
 *
 *	@author Phillip Goriup, Peter Grasch
 */
void SimonView::closeEvent ( QCloseEvent * event )
{
  if (KApplication::kApplication()->sessionSaving()) {
    event->accept();
    return;
  }
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
  Logger::log( i18n ( "Quitting..." ) );
  trayManager->deleteLater();
  delete control;
  Logger::close();
}
