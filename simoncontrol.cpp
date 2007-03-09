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
	sleep (2);
	//run->run(word);
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
	SimonInfo::showMessage("Verbunden zu Julius", 4000);
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
	SimonInfo::showMessage("Verbindung zu Julius verloren", 4000);
}

/**
 * @brief Sets basic Parameters for the Sound-System
 *
 * Uses the MicControl control handle to initialize the primary sound card
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
	disconnect( this->julius, 0,0,0);
	return this->active;
}

/**
 * @brief Toggles the activition
 *
 *	@author Peter Grasch
 */
bool SimonControl::toggleActivition()
{
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
	connect(julius, SIGNAL(wordRecognised(QString)), this, SLOT(wordRecognised(QString)));
	connect(julius, SIGNAL(connected()), this, SLOT(connectedToJulius()));
	connect(julius, SIGNAL(disconnected()), this, SLOT(disconnectedFromJulius()));
	return this->active;
}

