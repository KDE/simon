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

#define COMMANDIDENT "simon"

/**
 * @brief Constructor
 * 
 * Sets the activitionstate to true
 *
 *	@author Peter Grasch
*/
SimonControl::SimonControl() : QObject ()
{
	this->active=true;
	this->julius = new JuliusControl();
	this->run = new RunCommand();
	eventHandler = new EventHandler();
	
	QObject::connect(julius, SIGNAL(connected()), this, SLOT(connectedToJulius()));
	QObject::connect(julius, SIGNAL(disconnected()), this, SLOT(disconnectedFromJulius()));
	QObject::connect(julius, SIGNAL(error(QString)), this, SLOT(errorConnecting(QString)));
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
SimonControl::~SimonControl()
{
	
}

/**
 * @brief Connects to julius
 *
 *	@author Peter Grasch
 * @todo We should make the host and the port configurable
 */
void SimonControl::connect(QString host)
{
	QStringList hostport;
	hostport=host.split(":",QString::KeepEmptyParts,Qt::CaseSensitive);
	
    julius->connectTo(hostport[0],hostport[1].toInt());
}

/**
 * @brief disconnects from julius
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnect()
{
	julius->disconnect();
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
void SimonControl::wordRecognised(QString word)
{
	if (word.indexOf(COMMANDIDENT,0,Qt::CaseInsensitive) == 0)
	{
		word = word.replace(0, QString(COMMANDIDENT).length()+1,"");
		SimonInfo::showMessage(word,2000);
		run->run(word);
	}
	else eventHandler->sendWord(word);
}

/**
 * @brief Julius connected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the julius socket was established
 *
 *	@author Peter Grasch
 */
void SimonControl::connectedToJulius()
{
	emit connected();
}

/**
 * @brief Julius disconnected
 *
 * This is just a feedback function provided to react to the fact that the
 * connection to the julius socket was lost
 *
 *	@author Peter Grasch
 */
void SimonControl::disconnectedFromJulius()
{
	emit disconnected();
}

/**
 * @brief We want to abort connecting to julius
 * 
 *	@author Peter Grasch
 */
void SimonControl::abortConnecting()
{
	this->julius->disconnect();
}


/**
 * @brief Emits the signal connetionError(QString)
 * 
 *	@author Peter Grasch
 */
void SimonControl::errorConnecting(QString error)
{
	emit connectionError(error);
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
	//mic->initializeMic( (short) 2, 44100 );
	//mic->capture(10,10);
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
	QObject::disconnect( this->julius, SIGNAL(wordRecognised(QString)), this, SLOT(wordRecognised(QString)));
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
	QObject::connect(julius, SIGNAL(wordRecognised(QString)), this, SLOT(wordRecognised(QString)));
	return this->active;
}

