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
#include <QDir>
#include <QPixmap>
#include <QMessageBox>
#include <QCryptographicHash>

#include "simonview.h"
#include "inlinewidgetview.h"
#include "Configuration/systemview.h"
#include "SimonLib/Logging/logger.h"
#include "simoncontrol.h"

#include "SimonLib/SimonInfo/simoninfo.h"
#include "Actions/runcommandview.h"
#include "SimonLib/TrayIcon/trayiconmanager.h"
#include "ModelManagement/modelmanager.h"
#include "ModelManagement/Training/trainingview.h"
#include "SimonLib/Settings/settings.h"
#include "SimonLib/PasswordDialog/passworddlg.h"
#include "Configuration/FirstRun/firstrunwizard.h"
#include "ModelManagement/WordList/wordlistview.h"
#include "ModelManagement/WordList/AddWord/addwordview.h"



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
	QDir::setCurrent ( QCoreApplication::applicationDirPath() );
	if ( !Logger::init() )
	{
		QMessageBox::critical ( this, tr ( "Fehler" ), tr ( "Konnte die Log-Datei nicht öffnen. Bitte überprüfen Sie die Berechtigungen.." ) );
		exit ( 1 );
	}
	
	Logger::log ( tr ( "[INF] Starte simon..." ) );
	
	this->info = new SimonInfo();

	//showing splash
	Logger::log ( tr ( "[INF] Zeige Splashscreen..." ) );
	this->info->showSplash();

	Logger::log ( tr ( "[INF] Lade Einstellungen..." ) );
	Settings::initSettings();

// 	Settings::set("ConfigDone", false);
	if (!Settings::getB("ConfigDone"))
	{
		FirstRunWizard *firstRunWizard = new FirstRunWizard(this);
		if (firstRunWizard->exec() || (QMessageBox::question(this, tr("Konfiguration abbrechen"), tr("Sie haben die Konfiguration nicht abgeschlossen. Einige Teile des Programmes funktionieren vielleicht nicht richtig.\n\nWollen Sie den Assistenten beim nächsten Start wieder anzeigen?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::No))
			Settings::set("ConfigDone", true);
		firstRunWizard->deleteLater();
	}
	
	
	
	Logger::log ( tr ( "[INF] Einstellungen geladen..." ) );

	this->info->writeToSplash ( tr ( "Lade Programmlogik..." ) );
	
	this->control = new SimonControl ();
	this->trayManager = new TrayIconManager();
	this->trayManager->createIcon ( QIcon ( ":/images/tray_d.png" ), "Simon" );


	this->trayManager->createIcon ( QIcon ( ":/images/tray.png" ), "Simon" );

	shownDialogs = 0;
	QMainWindow ( parent,flags );
	qApp->setQuitOnLastWindowClosed(false);
	ui.setupUi ( this );


	//Preloads all Dialogs
	guessChildTriggers ( ( QObject* ) this );

	this->info->writeToSplash ( tr ( "Lade \"Trainieren\"..." ) );
	this->trainDialog = TrainingView::getInstance ();

	this->info->writeToSplash ( tr ( "Lade \"Wortliste\"..." ) );
	this->wordList = new WordListView ( this );

	this->info->writeToSplash ( tr ( "Lade \"Wort hinzufügen\"..." ) );
	this->addWordView = AddWordView::getInstance();

	this->info->writeToSplash ( tr ( "Lade \"Ausführen\"..." ) );
	this->runDialog = new RunCommandView ( this );

	this->info->writeToSplash ( tr ( "Lade \"System\"..." ) );
	this->systemDialog = new SystemView ( this );

	this->info->writeToSplash ( tr ( "Lade Oberfläche..." ) );

	ui.tbStatus->addWidget ( ui.pbActivision );
	ui.tbStatus->addWidget ( ui.pbHide );
	ui.tbStatus->addWidget ( ui.pbCompileModel );
	ui.tbStatus->addWidget ( ui.pbKeyed );
	ui.tbStatus->addWidget ( ui.pbClose );
	ui.tbLogo->addWidget ( ui.lbLogo );
	ui.tbLogo->addWidget ( ui.frmConnectionStatus );
	ui.tbModules->addWidget ( ui.pbAddWord );
	ui.tbModules->addWidget ( ui.pbTrain );
	ui.tbModules->addWidget ( ui.pbWordList );
	ui.tbModules->addWidget ( ui.pbRunProgram );
	ui.pbSystem->setDisabled ( true );
	settingsToolButton = ui.tbModules->addWidget ( ui.pbSystem );
	ui.tbModules->removeAction ( settingsToolButton );

	setupSignalSlots();


	if ( Settings::getB ( "Juliusd/AutoConnect" ) )
	{
		this->info->writeToSplash ( tr ( "Verbinde zu juliusd..." ) );
		connectToServer();
	}

	//hiding splash again after loading
	this->info->hideSplash();

	show();
	QCoreApplication::processEvents();
	connect ( systemDialog, SIGNAL ( commandsChanged() ), runDialog, SLOT ( loadCommands() ) );

	//switches, if the settings are shown or not
	hideSettings();

	connect ( ui.pbKeyed, SIGNAL ( clicked ( bool ) ), this, SLOT ( checkSettingState() ) );
}

void SimonView::showAbout()
{
	QMessageBox::about(this, tr("Über simon"), tr("<html><head /><body><h2>simon 0.1-alpha-2</h2>\
	<h2>Entwicklung und Vertrieb</h2>\
	<p>Entwickelt und Vertrieben vom gemmeinnützigen Verein <a href=\"http://simon-listens.org\">simon listens e.V.</a> in Kooperation mit <a href=\"http://cyber-byte.at\">Cyber-Byte EDV Services</a>.</p>\
	<h3>Entwickler</h3>\
	<ul><li>Peter Grasch<li>Phillip Goriup<li>Susanne Tschernegg<li>Bettina Sturmann<li>Gigerl Martin</ul>\
	<h3>Dank an</h3> \
	<ul><li>Franz Stieger<li>Mathias Stieger<li>Phillip Theussl<li>Moacyr Prado<li>Stieger Michael</ul> \
	</body></html>"));
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void SimonView::setupSignalSlots()
{
	//Setting up Signal/Slots
	QObject::connect(ui.pbAbout, SIGNAL(clicked()), this, SLOT(showAbout()));
	QObject::connect(ui.pbAboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
	
	
	QObject::connect ( control,SIGNAL ( guiAction ( QString ) ), ui.inlineView,SIGNAL ( guiAction ( QString ) ) );
	connect ( control, SIGNAL ( guiAction ( QString ) ), this, SLOT ( doAction ( QString ) ) );
	QObject::connect ( this->trainDialog, SIGNAL ( displayMe() ), this, SLOT ( showTrainDialog() ) );

	QObject::connect ( ui.pbAddWord, SIGNAL ( clicked() ), this, SLOT ( showAddWordDialog()) );
	QObject::connect ( ui.pbWordList, SIGNAL ( clicked()), this, SLOT ( showWordListDialog()) );
	QObject::connect ( ui.pbRunProgram, SIGNAL ( clicked()), this, SLOT ( showRunDialog()) );
	QObject::connect ( ui.pbTrain, SIGNAL ( clicked()), this, SLOT ( showTrainDialog()) );
	QObject::connect ( ui.pbSystem, SIGNAL ( clicked( ) ), this, SLOT ( showSystemDialog () ) );
	QObject::connect ( addWordView, SIGNAL ( hidden() ), this, SLOT ( setButtonNotChecked() ) );

// 	QObject::connect ( ui.pbQuickAddWord, SIGNAL ( clicked() ), ui.pbAddWord, SLOT ( animateClick() ) );
// 	QObject::connect ( ui.pbQuickWordlist, SIGNAL ( clicked() ), ui.pbWordList, SLOT ( animateClick() ) );
// 	QObject::connect ( ui.pbQuickRunProgram, SIGNAL ( clicked() ), ui.pbRunProgram, SLOT ( animateClick() ) );
// 	QObject::connect ( ui.pbQuickTrain, SIGNAL ( clicked() ), ui.pbTrain, SLOT ( animateClick() ) );

	QObject::connect ( ui.pbHide, SIGNAL ( clicked() ), this, SLOT ( hideSimon() ) );
	QObject::connect ( ui.pbClose, SIGNAL ( clicked() ), this, SLOT ( closeSimon() ) );
	QObject::connect ( this->trayManager, SIGNAL ( clicked() ), this, SLOT ( toggleVisibility() ) );
	QObject::connect ( ui.pbActivision, SIGNAL ( clicked() ), this, SLOT ( toggleActivation() ) );
	QObject::connect ( this->trayManager, SIGNAL ( middleClicked() ), this, SLOT ( toggleActivation() ) );
	
	//TextSync
//	QObject::connect(ui.pbSyncTest, SIGNAL(clicked()),control, SLOT(sendFileToSyncer()));
	//______

	connect ( addWordView, SIGNAL ( addedWord() ), wordList,
	          SLOT ( reloadList() ) );
	connect ( trainDialog, SIGNAL ( trainingCompleted() ), wordList,
	          SLOT ( reloadList() ) );

	connect ( ui.pbConnect, SIGNAL ( clicked() ), this, SLOT ( connectToServer() ) );

	connect ( ui.inlineView, SIGNAL ( registeredPage ( InlineWidget* ) ), this,
	          SLOT ( inlineWidgetRegistered ( InlineWidget* ) ) );
	connect ( ui.inlineView, SIGNAL ( unRegisteredPage ( InlineWidget* ) ), this,
	          SLOT ( inlineWidgetUnRegistered ( InlineWidget* ) ) );



	QObject::connect ( control, SIGNAL ( connected() ), this, SLOT ( connected() ) );
	QObject::connect ( control, SIGNAL ( disconnected() ), this, SLOT ( disconnected() ) );
	connect ( control, SIGNAL ( connectionError ( QString ) ), this, SLOT ( errorConnecting ( QString ) ) );


	connect ( ui.pbCompileModel, SIGNAL ( clicked() ), this, SLOT ( compileModel() ) );
}

void SimonView::compileModel()
{
	ModelManager::compileModel();
}

void SimonView::setButtonNotChecked()
{
	if ( dynamic_cast<AddWordView*> ( sender() ) )
	{
		ui.pbAddWord->setChecked ( false );
	}
	else if ( dynamic_cast<RunCommandView*> ( sender() ) )
	{
		ui.pbRunProgram->setChecked ( false );
	}
	else if ( dynamic_cast<WordListView*> ( sender() ) )
	{
		ui.pbWordList->setChecked ( false );
	}
	else if ( dynamic_cast<SystemView*> ( sender() ) )
	{
		ui.pbSystem->setChecked ( false );
	}
	else if ( dynamic_cast<TrainingView*> ( sender() ) )
	{
		ui.pbTrain->setChecked ( false );
	}
}


/**
 * \brief Connects to juliusd and gives appropriate status information about it
 *
 * @author Peter Grasch
 */
void SimonView::connectToServer()
{
	ui.pbConnect->setText ( "Verbinde..." );
	ui.pbConnect->setChecked ( true );
	disconnect ( ui.pbConnect, 0,0,0 );
	connect ( ui.pbConnect, SIGNAL ( clicked() ), this, SLOT ( abortConnecting() ) );
	this->control->connectToJulius();
}

/**
 * \brief Informs the user that we connected to the server
 *
 * @author Peter Grasch
 */
void SimonView::connected()
{
	ui.pbConnect->setText ( tr ( "Trennen" ) );
	ui.lbTextConnectionStatus->setText(tr("Verbunden"));
	ui.pbConnect->setEnabled ( true );
	ui.pbActivision->setEnabled ( true );
	disconnect ( ui.pbConnect, 0,0,0 );
	connect ( ui.pbConnect, SIGNAL ( clicked() ), control, SLOT ( disconnectFromJulius() ) );

	SimonInfo::showMessage ( tr ( "Verbunden zu Julius" ), 3000 );

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
	ui.pbConnect->setText ( tr ( "Verbinden" ) );
	ui.lbTextConnectionStatus->setText(tr("Nicht Verbunden"));
	ui.pbActivision->setEnabled ( false );
	ui.pbConnect->setChecked ( false );
	disconnect ( ui.pbConnect, 0,0,0 );
	connect ( ui.pbConnect, SIGNAL ( clicked() ), this, SLOT ( connectToServer() ) );

	SimonInfo::showMessage ( tr ( "Verbindung zu Julius verloren" ), 4000 );
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
	ui.pbConnect->setText ( tr ( "Verbinden" ) );
	ui.pbConnect->setChecked ( false );
	disconnect ( ui.pbConnect, 0,0,0 );
	connect ( ui.pbConnect, SIGNAL ( clicked() ), this, SLOT ( connectToServer() ) );

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
void SimonView::errorConnecting ( QString error )
{
	ui.pbConnect->setText ( tr ( "Verbinden" ) );
	ui.pbConnect->setChecked ( false );
	disconnect ( ui.pbConnect, 0,0,0 );
	connect ( ui.pbConnect, SIGNAL ( clicked() ), this, SLOT ( connectToServer() ) );

	this->control->deactivateSimon();
	this->representState();

	QMessageBox::critical ( this, tr ( "Kritischer Verbindungsfehler" ), tr ( "Die Verbindung zum juliusd Erkennungsdämon konnte nicht aufgenommen werden.\n\nBitte überprüfen Sie Ihre Einstellungen, ihre Netzwerkverbindung und ggf. Ihre Firewall.\n\nDie exakte(n) Fehlermeldung(en) lautete(n):\n" ) +error );
	Logger::log ( tr ( "[ERR] Verbindung zu juliusd fehlgeschlagen..." ) );
}



/**
 * @brief Shows the Run Dialog
 *
 * @author Peter Grasch
 */
void SimonView::showRunDialog ()
{
	alignButtonToInlineWidget(ui.pbRunProgram, runDialog);
}


/**
 * @brief Shows a dialog to add a new Word to the model
 *
 * @author Peter Grasch
 */
void SimonView::showAddWordDialog ( )
{
	if ( !addWordView->isVisible() )
	{
		this->addWordView->show();
		ui.pbAddWord->setChecked ( true );
	}
	else
	{
		this->addWordView->hide();
		ui.pbAddWord->setChecked ( false );
	}
}


/**
 * \brief Presses the corresponding button / Gives visual feedback
 * \author Peter Grasch
 */
void SimonView::inlineWidgetRegistered ( InlineWidget *widget )
{
	if ( widget==wordList )
		if ( !ui.pbWordList->isChecked() ) ui.pbWordList->setChecked(true);
	if ( widget==runDialog )
		if ( !ui.pbRunProgram->isChecked() ) ui.pbRunProgram->setChecked(true);
	if ( widget==trainDialog )
		if ( !ui.pbTrain->isChecked() ) ui.pbTrain->setChecked(true);
	if ( widget==systemDialog )
		if ( !ui.pbSystem->isChecked() ) ui.pbSystem->setChecked(true);
}

/**
 * \brief Releases the corresponding button / Gives visual feedback
 * \author Peter Grasch
 */
void SimonView::inlineWidgetUnRegistered ( InlineWidget *widget )
{
	if ( widget==wordList )
		if ( ui.pbWordList->isChecked() ) ui.pbWordList->setChecked(false);
	if ( widget==runDialog )
		if ( ui.pbRunProgram->isChecked() ) ui.pbRunProgram->setChecked(false);
	if ( widget==trainDialog )
		if ( ui.pbTrain->isChecked() ) ui.pbTrain->setChecked(false);
	if ( widget==systemDialog )
		if ( ui.pbSystem->isChecked() ) ui.pbSystem->setChecked(false);
}

void SimonView::alignButtonToInlineWidget(QAbstractButton *btn, InlineWidget *widget)
{
	if (!btn || !widget) return;
	if (!widget->isShown())
	{
		if (ui.inlineView->indexOf(widget) == -1)
			ui.inlineView->registerPage ( widget );
		else
			ui.inlineView->focusPage(widget);

		btn->setChecked(true);
	} else
	{
		ui.inlineView->unRegisterPage ( widget );
		btn->setChecked(false);
	}
}

/**
 * @brief Shows a dialog to configure simon
 *
 * @author Peter Grasch
 */
void SimonView::showSystemDialog ()
{
	alignButtonToInlineWidget(ui.pbSystem, systemDialog);
}

/**
 * @brief Shows a dialog to train the language model
 *
 * @author Peter Grasch
 */
void SimonView::showTrainDialog ()
{
	alignButtonToInlineWidget(ui.pbTrain, trainDialog);
}

/**
 * @brief Shows a dialog to Control the Laguage Model
 *
 * @author Peter Grasch
 */
void SimonView::showWordListDialog ()
{
	alignButtonToInlineWidget(ui.pbWordList, wordList);
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
	this->representState();
}

/**
 * \brief Make the widgets represent the current state (active/inactive)
 *
 * \author Peter Grasch
 */
void SimonView::representState()
{
	if ( this->control->getActivitionState() )
	{
		ui.lbLogo->setPixmap ( QPixmap ( ":/images/simon.png" ) );
		ui.pbActivision->setText ( tr ( "Deaktivieren" ) );
		ui.pbActivision->setIcon ( QIcon ( ":/images/icons/media-playback-pause.svg" ) );
		this->trayManager->createIcon ( QIcon ( ":/images/tray.png" ), "Simon" );

		if ( isHidden() ) SimonInfo::showMessage ( QString ( "Simon wurde aktiviert" ), 2000 );

		repaint();
	}
	else
	{
		ui.lbLogo->setPixmap ( QPixmap ( ":/images/simon_d.png" ) );
		ui.pbActivision->setText ( tr ( "Aktivieren" ) );
		ui.pbActivision->setIcon ( QIcon ( ":/images/icons/media-playback-start.svg" ) );

		if ( isHidden() ) SimonInfo::showMessage ( QString ( tr ( "simon wurde deaktiviert" ) ), 2000 );

		this->trayManager->createIcon ( QIcon ( ":/images/tray_d.png" ), tr ( "Simon - Deaktiviert" ) );
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
	if ( ( !Settings::getB ( "AskBeforeExit" ) ) || ( QMessageBox::question ( this, tr ( "Wirklich beenden?" ), tr ( "Ein beenden der Applikation wird die Verbindung zur Erkennung beenden und weder Diktatfunktionen noch andere Kommandos können mehr benutzt werden.\n\nWollen Sie wirklich beenden?" ),QMessageBox::Yes|QMessageBox::No,QMessageBox::No ) == QMessageBox::Yes ) )
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
	if ( sender() != ui.pbClose )
		event->ignore();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
SimonView::~SimonView()
{
	Logger::log ( tr ( "[INF] Beenden..." ) );
	this->trayManager->~ TrayIconManager();
	this->control->~ SimonControl();
	this->info->~ SimonInfo();
	Logger::close();
}

/**
 * @brief If the user wants to set any settings, he/she has to give a password to show all settings.
 *
 *	@author Susanne Tschernegg
*/
void SimonView::checkSettingState()
{
	if ( ui.pbKeyed->isChecked() )
	{
		if ( !Settings::getB ( "Passwordprotected" ) || checkPassword() )
		{
			showSettings();
		}
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
 *      returns wheter the password is correct or not
*/
bool SimonView::checkPassword()
{
	PasswordDlg* dialog = new PasswordDlg ( this );

	int success = dialog->exec();
	if ( !success )
	{
		ui.pbKeyed->setChecked ( false );
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * @brief the user couldn't see the setting-widgets resp. they are disabled
 *
 *	@author Susanne Tschernegg
*/
void SimonView::hideSettings()
{
	//disables the button settings in the mainwindow
	if ( ui.pbSystem->isChecked() )
	{
		ui.pbSystem->setChecked(false);
		ui.inlineView->unRegisterPage(this->systemDialog);
	}

	ui.tbModules->removeAction ( this->settingsToolButton );
	ui.pbSystem->setDisabled ( true );



	wordList->setSettingsHidden();
	trainDialog->setSettingsHidden();
	runDialog->setSettingsHidden();
}

/**
 * @brief to set some settings, the widgets must be shown resp. not disabled
 *
 *	@author Susanne Tschernegg
*/
void SimonView::showSettings()
{
	//to set the button settings in the main not disabled
	ui.tbModules->addAction ( this->settingsToolButton );
	ui.pbSystem->setDisabled ( false );


	//sets the setting buttons visible
	trainDialog->setSettingsVisible();
	wordList->setSettingsVisible();
	runDialog->setSettingsVisible();
}

