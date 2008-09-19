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
#include "projectview.h"
#include "settings.h"
#include "settingsview.h"
#include "userdialog.h"
#include <QVariant>
#include <QTextEdit>

/**
 * @brief Constructor
 * 
 * 	Sets up the trayIcon and sets it to the deactivated state;
 * 	Connects the signal/slots and instances the julius handle and
 * 	the NetworkControl
 *
 *	@author Peter Grasch
 *	@param QWidget *parent
 *	The widgets parent
 *	@param Qt::WFlags flags
 *	Sets the window-flags
 */
ProjectView::ProjectView(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent,flags)
{
	Settings::initSettings();

	ui.setupUi(this);
	
	//Signals/Slots
	connect(ui.pbStart, SIGNAL(clicked()), this, SLOT(startServer()));
	connect (ui.pbStop, SIGNAL(clicked()), this, SLOT(stopServer()));
	connect (ui.pbReload, SIGNAL(clicked()), this, SLOT(reloadServer()));
	connect (ui.actionReload, SIGNAL(triggered()), this, SLOT(reloadServer()));
	connect (ui.actionStop, SIGNAL(triggered()), this, SLOT(stopServer()));
	connect(ui.actionStart, SIGNAL(triggered()), this, SLOT(startServer()));
	connect (ui.pbSendWord, SIGNAL(clicked()), this, SLOT(sendWord()));
	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(closeJuliusd()));
	connect(ui.pbClose, SIGNAL(clicked()), this, SLOT(closeJuliusd()));
	connect(ui.actionQt, SIGNAL(triggered()), this, SLOT(showAboutQT()));
	connect(ui.actionSimon, SIGNAL(triggered()), this, SLOT(showAboutSimon()));
	connect(ui.actionUsers, SIGNAL(triggered()), this, SLOT(configureUsers()));
	
	//init Members
	running = false;
	net = new NetworkControl();
	dlgSettings = new SettingsView(this);
	trayIcon = new TrayIconManager();
	trayIcon->createIcon( QIcon(":/images/juliustray_d.png"), "simon: Julius Integration deaktiviert" );
	connect(trayIcon, SIGNAL(clicked()), this, SLOT(toggleVisibility()));
	connect(trayIcon, SIGNAL(middleClicked()), this, SLOT(toggleStartStop()));

	
	connect (net, SIGNAL(connectionReceived(QString,int)), this, SLOT(clientConnected(QString,int)));
	connect (net, SIGNAL(connectionDropped(QString,int)), this, SLOT(clientConnectionDropped(QString,int)));
	connect (net, SIGNAL(errorOccured(QString,int)), this, SLOT(printError(QString,int)));
	connect (net, SIGNAL(info(QString,int)), this, SLOT(printInfo(QString,int)));
	connect (net, SIGNAL(wordSent(QString,int)), this, SLOT(displaySentWord(QString,int)));
	
	if (Settings::get("StartArgs").toInt() & CONNECTONSTART)
		this->startServer();
	if (Settings::get("StartArgs").toInt() & STARTMINIMIZED)
		QTimer::singleShot(0, this, SLOT(hide()));
}

 /**                                                                          
  * @brief Sends the word trough the NetworkControl net (member) and appends it to the log
  *                                                                                       
  *     @author Peter Grasch                                                              
  *     @param QString word                                                               
  *     Contains the word                                                                 
  *
  */                                                                                      
void ProjectView::sendWord(QString word)                                                 
{                                                                                        
       if (word == "__INTERNAL__EMPTY__")                                                
               word = QInputDialog::getText(this, "Testwort", "Geben Sie hier ein Wort ein das Sie an\nsimon schicken wollen.\nEs wird gleich behandelt werden als wäre\nes von Julius erkannt worden.\n\nDas Wort wird an alle verbundenen Clients geschickt.");                  
       net->sendWord(word);                                                                
} 



/**
 * \brief Tells the user that a client has connected to the server
 * 
 * \param QString host
 * The host that connected to the server
 */
void ProjectView::clientConnected(QString host, int id)
{
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("<p>Der Computer \""+host+"\" hat sich zum Server verbunden</p>");
	QTextEdit *newLog = new QTextEdit(this);
	newLog->setReadOnly(true);
	this->logs.insert(id, newLog);
	ui.twLogs->addTab(newLog, host);
}

/**
 * \brief Tells the user that a client has disconnected from the server
 * 
 * \param QString host
 * The host that disconnected from the server
 */
void ProjectView::clientConnectionDropped(QString host, int id)
{
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("<p>Der Computer \""+host+"\" hat die Verbindung beendet</p>");
	ui.twLogs->removeTab(ui.twLogs->indexOf(logs.value(id)));
}


/**
 * @brief Shows an about Dialog about Simon
 *
 *	@author Peter Grasch
 */
void ProjectView::showAboutSimon()
{
	QMessageBox::about(this, "About simon", "Simon ist eine Spracherkennungs-suite die OpenSource\nSpracherkennungssoftware (Julius/Julian) mit\nselbsterstellten Sprachmodellen ausstattet und das\nganze mit einer einfachen, intuitiven aber auch\nfunktionsreichen grafischen Oberfläche abrundet");
}

/**
 * @brief Shows an about Dialog about QT
 *
 *	@author Peter Grasch
 */
void ProjectView::showAboutQT()
{
	QMessageBox::aboutQt(this);
}

/**
 * @brief Toggles the Visibility of the main window
 *
 *	@author Peter Grasch
 */
void ProjectView::toggleVisibility()
{
	if (isHidden())
		show();
	else hide();
}

bool ProjectView::initDb()
{
	if (!net->initDb())
	{
		QMessageBox::critical(this, i18n("Fehlerhafte Datenbankverbindung"), i18n("Konnte Verbindung mit der Datenbank nicht herstellen.\n\nDie Benutzerkonten können nicht ausgelesen werden. Bitte überprüfen Sie die Einstellungen."));
		return false;
	}
	return true;
}

/**
 * @brief Starts the server
 * 
 *	Enables/Disables the ui controls and appends "Starte System" to the
 *	Log;
 *	It also tells the NetworkControl net (member) to connect to the host
 *	Moreover it exchanges the trayIcon to an inactive one
 *
 *	@author Peter Grasch
 */
void ProjectView::startServer()
{
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("<b>Starte System...</b>");
	

	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("\tVerbinde zur Datenbank...");
	
	if (!initDb()) return;

	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("\tStarte Server...");

	if (Settings::get("RestrictAccess").toBool())
		net->startServer( QHostAddress(Settings::getS("AllowedHost")), Settings::get("Port").toInt() );
	else 
		net->startServer( QHostAddress::Any, Settings::get("Port").toInt() );

	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("\tServer läuft.");
	
	
	running = true;
	
	trayIcon->createIcon( QIcon(":/images/juliustray.png"), "simon: Julius Integration" );
	
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("<p style=\"color:green; font-weight:bold;\">Fertig</p>\n");
	ui.pbStart->setEnabled(false);
	ui.pbStop->setEnabled(true);
	ui.pbSendWord->setEnabled(true);
	ui.pbReload->setEnabled(true);
}

/**
 * @brief Stops the server
 * 
 *	Enables/Disables the ui controls and appends "Stoppe Erkennung" to the
 *	Log;
 *	It also tells the NetworkControl net (member) to drop the connection to the host
 *	Moreover it exchanges the trayIcon to an active one
 *
 *	@author Peter Grasch
 */
void ProjectView::stopServer()
{
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("\n<b>Stoppe System</b>...");
	
	ui.pbStart->setEnabled(true);
	ui.pbStop->setEnabled(false);
	ui.pbSendWord->setEnabled(false);
	ui.pbReload->setEnabled(false);
	trayIcon->createIcon( QIcon(":/images/juliustray_d.png"), "simon: Julius Integration deaktiviert" );
	
	
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("\tServer stoppen...");
	net->stopServer();
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("\tServer gestoppt...");
	
	
	if (Settings::get("OutArgs").toInt() & INFO)
		ui.tbLog->append("<p style=\"color:green; font-weight:bold;\">Fertig</p>\n");
	
	running = false;
}


void ProjectView::configureUsers()
{
	if (!net->getDbManager())
	{
		if (QMessageBox::question(this, i18n("Datenbankverbindung"), i18n("Es besteht derzeit keine Verbindung mit der Datenbank (dazu muss der Server gestartet werden).\n\nWollen Sie jetzt verbinden?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
			if (!initDb()) return;
	}

	if (!net->getDbManager()) return;

	UserDialog *userDlg = new UserDialog(this->net->getDbManager(), this);
	userDlg->exec();
}

/**
 * @brief Shows the Settings Dialog
 * 
 *	Creates a new Settings Dialog and shows it
 *
 *	@author Peter Grasch
 */
void ProjectView::showSettingsDialog()
{
	if (dlgSettings->exec())
	{
		if (Settings::get("OutArgs").toInt() & INFO)
			ui.tbLog->append("Konfiguration gespeichert");
	} else
		if (Settings::get("OutArgs").toInt() & INFO)
			ui.tbLog->append("Konfiguration abgebrochen");
}

/**
 * @brief Reloads the Server
 * 
 *	Calls the stopServer and startServer methods
 *
 *	@author Peter Grasch
 *	@see stopServer()
 *	@see startServer()
 */
void ProjectView::reloadServer()
{
	stopServer();
	startServer();
}

/**
 * @brief Appends an bold Errorstring to the Log
 * 
 *	Uses the <b> and </b> hmtl tags to make the error text bold
 *
 *	@author Peter Grasch
 *	@param QString error
 *	Contains the error message
 */
void ProjectView::printError(QString error, int id)
{
	if (Settings::get("OutArgs").toInt() & ERROR)
	{
		QString str = "<p style=\"color:red;\">"+error+"</p>\n";
		if (id != -1)
		{
			QTextEdit *log = this->logs.value(id);
			if (!log) return;
			log->append(str);
		} else ui.tbLog->append(str);
	}
}

void ProjectView::displaySentWord(QString word, int id)
{
	if (Settings::get("OutArgs").toInt() & WORD)
	{
		QTextEdit *log = this->logs.value(id);
		if (!log) return;
		log->append("<p style=\"color:blue\">"+word.trimmed()+"</p>");
	}
}

/**
 * @brief Appends an info to the Log
 *
 *	@author Peter Grasch
 *	@param QString error
 *	Contains the message
 */
void ProjectView::printInfo(QString info, int id)
{
	if (Settings::get("OutArgs").toInt() & INFO)
	{
		QString str = "<p style=\"color:green;\">"+info+"</p>\n";
		if (id != -1)
		{
			QTextEdit *log = this->logs.value(id);
			if (!log) return;
			log->append(str);
		} else ui.tbLog->append(str);
	}
}


/**
 * @brief If the server is running it is stopped and vice versa
 * 
 *	@author Peter Grasch
 */
void ProjectView::toggleStartStop()
{
	if (running)
		stopServer();
	else startServer();
}


/**
 * @brief Closes Juliusd
 * 
 *	@author Peter Grasch
 */
void ProjectView::closeJuliusd()
{
	close();
	this->trayIcon->deleteLater();
	this->trayIcon =0;
}

/**
 * @brief Destructor
 * 
 * 	Destroys the trayIcon (member)
 * 
 *	@author Peter Grasch
 */
ProjectView::~ProjectView()
{
	if (this->trayIcon)
		this->trayIcon->deleteLater();
}


