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


#include "simoncontrol.h"

#include <simonactions/actionmanager.h>

#include "coreconfiguration.h"

#include <KMessageBox>
#include <KLocalizedString>
#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>

/**
 * @brief Constructor
 * 
 * Sets the activitionstate to true
 *
 *	@author Peter Grasch
*/
SimonControl::SimonControl(QWidget *parent) : QObject (parent)
{
	setStatus(SimonControl::Disconnected);
	this->recognitionControl = new RecognitionControl(parent);
	ActionManager::getInstance()->init();
	QObject::connect(ActionManager::getInstance(), SIGNAL(guiAction(QString)), this, SIGNAL(guiAction(QString)));
	
	QObject::connect(recognitionControl, SIGNAL(connected()), this, SLOT(connectedToServer()));
	QObject::connect(recognitionControl, SIGNAL(disconnected()), this, SLOT(disconnectedFromServer()));
	QObject::connect(recognitionControl, SIGNAL(connectionError(QString)), this, SLOT(errorConnecting(QString)));

	QObject::connect(recognitionControl, SIGNAL(status(const QString&, int, int)), this, SLOT(slotRecognitionControlStatus(const QString&, int, int)));
	QObject::connect(recognitionControl, SIGNAL(progress(int, int)), this, SIGNAL(progressInfo(int,int)));
	QObject::connect(recognitionControl, SIGNAL(error(const QString&,bool)), this, SLOT(serverError(QString,bool)));
	QObject::connect(recognitionControl, SIGNAL(warning(const QString&)), this, SLOT(serverWarning(QString)));
	QObject::connect(recognitionControl, SIGNAL(loggedIn()), this, SLOT(loggedIn()));
	
	QObject::connect(recognitionControl, SIGNAL(recognised(const QString&,const QString&,const QString&)), this, SLOT(wordRecognised(QString,QString,QString)));
	QObject::connect(recognitionControl, SIGNAL(recognitionStatusChanged(RecognitionControl::RecognitionStatus)), this, SLOT(recognitionStatusChanged(RecognitionControl::RecognitionStatus)));
}

void SimonControl::slotRecognitionControlStatus(const QString& message, int progNow, int progMax)
{
	emit statusInfo(message);
	if (progNow != -1)
		emit progressInfo(progNow, progMax);
}

bool SimonControl::passwordProtected()
{ return CoreConfiguration::passwordProtected(); }

QString SimonControl::adminPassword()
{ return CoreConfiguration::adminPassword(); }

bool SimonControl::askBeforeQuit()
{ return CoreConfiguration::askBeforeQuit(); }

void SimonControl::loggedIn()
{
	SimonInfo::showMessage(i18n("Benutzer authentifiziert"), 1500);
}

void SimonControl::serverError(QString error, bool skippable)
{
	if (skippable)
		error += "\n\n"+i18n("Sie können dies ignorieren, indem Sie die Option \"Warnungen ignorieren\" In den Netzwerkeinstellungen aktivieren");

	KMessageBox::error(0, error, i18n("Server Fehler"));

	Logger::log(i18n("[ERR] Server Fehler: %1, Überspringbar: %2", error, skippable ? i18n("Ja") : i18n("Nein")));
}

void SimonControl::serverWarning(QString warning)
{
	Logger::log(i18n("[INF] Server Warning: %1", warning));

	SimonInfo::showMessage(i18n("simond: %1", warning), 5000);
}

/**
 * @brief Connects to recognitionControl
 *
 *	@author Peter Grasch
 */
void SimonControl::connectToServer()
{
	setStatus(SimonControl::Connecting);
	recognitionControl->startConnecting();
}


/**
 * @brief disconnects from recognitionControl
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnectFromServer()
{
	recognitionControl->disconnectFromServer();
}


/**
 * @brief Word recognised
 * 
 * usually called when the server recognised a word
 *
 *	@author Peter Grasch
 *	@param QString word
 *	the recognised word
 */
void SimonControl::wordRecognised(QString word,QString sampa, QString samparaw)
{
	Q_UNUSED(sampa);
	Q_UNUSED(samparaw);

	if (status != SimonControl::ConnectedActivated) return;
	
	ActionManager::getInstance()->process(word);
}


void SimonControl::recognitionStatusChanged(RecognitionControl::RecognitionStatus status)
{
	switch (status)
	{
		case RecognitionControl::Ready:
		{
			
			break;
		}
		
		case RecognitionControl::Started:
		{
			
			break;
		}
		
		case RecognitionControl::TemporarilyUnavailable:
		{
			
			break;
		}
		
		case RecognitionControl::Stopped:
		{
			
			break;
		}
		
	}
}

void SimonControl::setStatus(SimonControl::SystemStatus status)
{
	this->status = status;
	emit systemStatusChanged(status);
}

/**
 * @brief Server connected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the recognitionControl socket was established
 *
 * @author Peter Grasch
 */
void SimonControl::connectedToServer()
{
	setStatus(SimonControl::ConnectedDeactivated);
	Logger::log(i18n("[INF]")+" "+i18n("Verbunden zu Server"));
	this->activateSimon();
	emit connected();
}

/**
 * @brief Server disconnected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the recognitionControl socket was lost
 *
 * @author Peter Grasch
 */
void SimonControl::disconnectedFromServer()
{
	setStatus(SimonControl::Disconnected);
	Logger::log(i18n("[INF] Verbindung von Server getrennt"));
	emit disconnected();
}

/**
 * @brief We want to abort connecting to recognitionControl
 * 
 * @author Peter Grasch
 */
void SimonControl::abortConnecting()
{
	Logger::log(i18n("[INF] Verbinden abgebrochen"));
	this->recognitionControl->disconnectFromServer();
}


/**
 * @brief Emits the signal connetionError(QString)
 * 
 * @author Peter Grasch
 */
void SimonControl::errorConnecting(QString error)
{
	setStatus(Disconnected);
	emit statusError(i18n ( "Die Verbindung zum juliusd Erkennungsdämon konnte nicht aufgenommen werden.\n\nBitte überprüfen Sie Ihre Einstellungen, ihre Netzwerkverbindung und ggf. Ihre Firewall.\n\nDie exakte(n) Fehlermeldung(en) lautete(n):\n" ) +error );
}



/**
 * @brief Toggles the activition
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::toggleActivition()
{
	if ((status != SimonControl::Disconnected) && (status != SimonControl::Connecting))
	{
		if (status==SimonControl::ConnectedActivated)
		{
			deactivateSimon();
		} else activateSimon();
	}
	
	return status;
}

/**
 * @brief Activates Simon
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::activateSimon()
{
	Logger::log(i18n("[INF] Simon aktiviert"));
	setStatus(SimonControl::ConnectedActivated);
	return status;
}


/**
 * @brief Deactivates Simon
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::deactivateSimon()
{
	if ((status != SimonControl::Disconnected) && (status != SimonControl::Connecting))
	{
		setStatus(SimonControl::ConnectedDeactivated);
		Logger::log(i18n("[INF] Simon deaktiviert"));
	}
	return status;
}

void SimonControl::compileModel()
{
	emit statusInfo(i18n("Starte Synchronisation..."));
	
	recognitionControl->startSynchronisation();
	
	emit statusInfo(i18n("Synchronisation gestartet"));
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonControl::~SimonControl()
{
	recognitionControl->deleteLater();
//     delete eventHandler;
}
