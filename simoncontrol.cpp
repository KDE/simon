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

#include "simoncontrol.h"
#include "logger.h"
#include "settings.h"
#include <QMessageBox>
#include "shortcutcontrol.h"
#include "screengrid.h"
#include "soundcontrol.h"
#include "juliuscontrol.h"
#include "runcommand.h"
#include "atwatcher.h"
#include "eventhandler.h"
#include "shortcutcontrol.h"

/**
 * @brief Constructor
 * 
 * Sets the activitionstate to true
 *
 *	@author Peter Grasch
*/
SimonControl::SimonControl() : QObject ()
{
	this->active=false;
	this->julius = new JuliusControl();
	this->run = RunCommand::getInstance();
	eventHandler = EventHandler::getInstance();

	this->shortcutControl = ShortcutControl::getInstance();

	this->atWatcher = ATWatcher::getInstance();
	
	QObject::connect(julius, SIGNAL(connected()), this, SLOT(connectedToJulius()));
	QObject::connect(julius, SIGNAL(disconnected()), this, SLOT(disconnectedFromJulius()));
	QObject::connect(julius, SIGNAL(connectionError(QString)), this, SLOT(errorConnecting(QString)));

	QObject::connect(julius, SIGNAL(error(QString,bool)), this, SLOT(juliusError(QString,bool)));
	QObject::connect(julius, SIGNAL(warning(QString)), this, SLOT(juliusWarning(QString)));
	QObject::connect(julius, SIGNAL(loggedIn()), this, SLOT(loggedIn()));
}

void SimonControl::loggedIn()
{
	SimonInfo::showMessage(tr("Benutzer authentifiziert"), 1500);
}

void SimonControl::juliusError(QString error, bool skippable)
{
	if (skippable)
		error += "\n\n"+tr("Sie können dies ignorieren, indem Sie die Option \"Warnungen ignorieren\" In den Netzwerkeinstellungen aktivieren");

	QMessageBox::critical(0, tr("Julius Fehler"), error);

	Logger::log(tr("[ERR] Julius Fehler: %1, Überspringbar: %2").arg(error).arg(skippable ? tr("Ja") : tr("Nein")));
}

void SimonControl::juliusWarning(QString warning)
{
	Logger::log(tr("[INF] Julius Warning: %1").arg(warning));

	SimonInfo::showMessage(tr("Julius: %1").arg(warning), 5000);
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonControl::~SimonControl()
{
    delete mic;
    julius->deleteLater();
    delete eventHandler;
}

/**
 * @brief Connects to julius
 *
 *	@author Peter Grasch
 */
void SimonControl::connectToJulius()
{
	juliusdConnectionsToTry.clear();
	juliusdConnectionErrors.clear();
	QString juliusServers = Settings::getS("Network/JuliusdServers");
	if (juliusServers.isEmpty()) return;
	QStringList addresses = juliusServers.split(";", QString::SkipEmptyParts);
	Logger::log(tr("[INF] %1 juliusd Adressen gefunden").arg(addresses.count()));
	
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
	Logger::log(tr("[INF] Verbinden zu Julius auf ")+host);
	QStringList address = host.split(":");
	julius->connectTo(address[0],address[1].toInt());
}

/**
 * @brief disconnects from julius
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnectFromJulius()
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
	QString keyword = Settings::getS("Commands/Keyword");
	
	if (word.startsWith(keyword))
	{
		word = word.replace(0, QString(keyword).length()+1,"");
		
		if (word.startsWith(Settings::getS("Desktopgrid/Trigger")))
		{
			ScreenGrid *sg = new ScreenGrid();
			connect(sg, SIGNAL(click(int, int)), this, SLOT(click(int, int)));
			sg->show();
		}
		if (shortcutControl && (shortcutControl->nameExists(word)))
		{
			eventHandler->sendShortcut(shortcutControl->getShortcut(word));
		} else if (!atWatcher->trigger(word))
			if (!run->run(word))
				emit guiAction(word);
	} else {
		eventHandler->sendWord(word);
	}
}

void SimonControl::click(int x, int y)
{
	if (dynamic_cast<ScreenGrid*>(sender()))
		delete sender();
	eventHandler->click(x,y);
}

/**
 * @brief Julius connected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the julius socket was established
 *
 * @author Peter Grasch
 */
void SimonControl::connectedToJulius()
{
	Logger::log(tr("[INF]")+" "+tr("Verbunden zu julius"));
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
void SimonControl::disconnectedFromJulius()
{
	Logger::log(tr("[INF] Verbindung von Julius getrennt"));
	emit disconnected();
}

/**
 * @brief We want to abort connecting to julius
 * 
 * @author Peter Grasch
 */
void SimonControl::abortConnecting()
{
	Logger::log(tr("[INF] Verbinden abgebrochen"));
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
	Logger::log(tr("[ERR] Verbinden zu Julius (%1) fehlgeschlagen: %2").arg(currentHost).arg(error));
	
	juliusdConnectionErrors << QString("%1: %2").arg(currentHost).arg(error);
	
	if (juliusdConnectionsToTry.count() > 0)
		connectTo(juliusdConnectionsToTry.at(0));
	else {
		emit connectionError(juliusdConnectionErrors.join("\n"));
	}
}

/**
 * @brief Sets basic Parameters for the Sound-System
 *
 * Uses the SoundControl control handle to initialize the primary sound card
 *
 *	@author Peter Grasch
 */
void SimonControl::initializeMic()
{
	
}


/**
 * @brief getActivitionState()
 *
 * Returns the current Activition State
 *
 *	@author Peter Grasch
 */
bool SimonControl::getActivitionState()
{
	return this->active;
}


/**
 * @brief Deactivates Simon
 *
 *	@author Peter Grasch
 */
bool SimonControl::deactivateSimon()
{
	this->active=false;
	disconnect( this->julius, SIGNAL(recognised(QString,QString,QString)), this, SLOT(wordRecognised(QString,QString,QString)));
	Logger::log(tr("[INF] Simon deaktiviert"));
	return this->active;
}

/**
 * @brief Toggles the activition
 *
 *	@author Peter Grasch
 */
bool SimonControl::toggleActivition()
{
	if (!julius->isConnected()) return false;
	
	if (this->active)
	{
		deactivateSimon();
	} else activateSimon();
	
	return this->active;
}

/**
 * @brief Activates Simon
 *
 *	@author Peter Grasch
 */
bool SimonControl::activateSimon()
{
	this->active=true;

	QObject::connect(julius, SIGNAL(recognised(QString,QString,QString)), this, SLOT(wordRecognised(QString,QString,QString)));
		
	Logger::log(tr("[INF] Simon aktiviert"));
	return this->active;
}

void SimonControl::sendFileToSyncer()
{
	julius->sendSyncFile("fileone.txt");
}

