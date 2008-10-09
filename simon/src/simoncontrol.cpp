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

#include "RecognitionControl/juliuscontrol.h"
#include "Actions/actionmanager.h"
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
	this->julius = new JuliusControl(parent);
	ActionManager *actionManager = ActionManager::getInstance();
	actionManager->init();
	QObject::connect(actionManager, SIGNAL(guiAction(QString)), this, SIGNAL(guiAction(QString)));
	
	QObject::connect(julius, SIGNAL(connected()), this, SLOT(connectedToServer()));
	QObject::connect(julius, SIGNAL(disconnected()), this, SLOT(disconnectedFromServer()));
	QObject::connect(julius, SIGNAL(connectionError(QString)), this, SLOT(errorConnecting(QString)));

	QObject::connect(julius, SIGNAL(error(QString,bool)), this, SLOT(juliusError(QString,bool)));
	QObject::connect(julius, SIGNAL(warning(QString)), this, SLOT(juliusWarning(QString)));
	QObject::connect(julius, SIGNAL(loggedIn()), this, SLOT(loggedIn()));
	
	QObject::connect(julius, SIGNAL(recognised(QString,QString,QString)), this, SLOT(wordRecognised(QString,QString,QString)));
}

void SimonControl::loggedIn()
{
	SimonInfo::showMessage(i18n("Benutzer authentifiziert"), 1500);
}

void SimonControl::juliusError(QString error, bool skippable)
{
	if (skippable)
		error += "\n\n"+i18n("Sie können dies ignorieren, indem Sie die Option \"Warnungen ignorieren\" In den Netzwerkeinstellungen aktivieren");

	KMessageBox::error(0, error, i18n("Julius Fehler"));

	Logger::log(i18n("[ERR] Julius Fehler: %1, Überspringbar: %2", error, skippable ? i18n("Ja") : i18n("Nein")));
}

void SimonControl::juliusWarning(QString warning)
{
	Logger::log(i18n("[INF] Julius Warning: %1", warning));

	SimonInfo::showMessage(i18n("Julius: %1", warning), 5000);
}

/**
 * @brief Connects to julius
 *
 *	@author Peter Grasch
 */
void SimonControl::connectToServer()
{
	setStatus(SimonControl::Connecting);
	
	juliusdConnectionsToTry.clear();
	juliusdConnectionErrors.clear();
	QStringList addresses = CoreConfiguration::juliusdServers();
	Logger::log(i18n("[INF] %1 juliusd Adressen gefunden", addresses.count()));
	
	if (addresses.count() == 0) return;
	
	for (int i=0; i < addresses.count(); i++)
		juliusdConnectionsToTry.append(addresses.at(i));
		
	connectTo(juliusdConnectionsToTry.at(0));
}


/**
 * \brief Connects to the given address/port (e.g.: localhost:4444)
 * \author Peter Grasch
 * @param host The host/port combination to connect to
 */
void SimonControl::connectTo(QString host)
{
	Logger::log(i18n("[INF] Verbinden zu Julius auf ")+host);
	QStringList address = host.split(":");
	julius->connectTo(address[0],address[1].toInt());
}

/**
 * @brief disconnects from julius
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnectFromServer()
{
	julius->disconnectFromServer();
}


/**
 * @brief Word recognised
 * 
 * usually called when Julius recognised a word
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

void SimonControl::setStatus(SimonControl::SystemStatus status)
{
	this->status = status;
	emit systemStatusChanged(status);
}

/**
 * @brief Julius connected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the julius socket was established
 *
 * @author Peter Grasch
 */
void SimonControl::connectedToServer()
{
	setStatus(SimonControl::ConnectedDeactivated);
	Logger::log(i18n("[INF]")+" "+i18n("Verbunden zu julius"));
	this->activateSimon();
	emit connected();
}

/**
 * @brief Julius disconnected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the julius socket was lost
 *
 * @author Peter Grasch
 */
void SimonControl::disconnectedFromServer()
{
	setStatus(SimonControl::Disconnected);
	Logger::log(i18n("[INF] Verbindung von Julius getrennt"));
	emit disconnected();
}

/**
 * @brief We want to abort connecting to julius
 * 
 * @author Peter Grasch
 */
void SimonControl::abortConnecting()
{
	Logger::log(i18n("[INF] Verbinden abgebrochen"));
	juliusdConnectionsToTry.clear();
	this->julius->disconnectFromServer();
}


/**
 * @brief Emits the signal connetionError(QString)
 * 
 * The function will try to connect to the next Juliusd Address in the list if
 * juliusdConnectionsToTry isn't empty;
 * If it isn't it will take the first entry and try to connect to that (see the 
 * connectTo() function); If it is, it will the error with the connectionError(error) Signal
 * 
 * @author Peter Grasch
 */
void SimonControl::errorConnecting(QString error)
{
	QString currentHost = juliusdConnectionsToTry.takeAt(0);
	Logger::log(i18n("[ERR] Verbinden zu Julius (%1) fehlgeschlagen: %2", currentHost, error));
	
	juliusdConnectionErrors << QString("%1: %2").arg(currentHost).arg(error);
	
	if (juliusdConnectionsToTry.count() > 0)
		connectTo(juliusdConnectionsToTry.at(0));
	else {
		setStatus(Disconnected);
		emit connectionError(juliusdConnectionErrors.join("\n"));
	}
}



/**
 * @brief Toggles the activition
 *
 *	@author Peter Grasch
 */
SimonControl::SystemStatus SimonControl::toggleActivition()
{
// 	if (!julius->isConnected()) return false;
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

// void SimonControl::sendFileToSyncer()
// {
// 	julius->sendSyncFile("fileone.txt");
// }


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonControl::~SimonControl()
{
	julius->deleteLater();
//     delete eventHandler;
}
