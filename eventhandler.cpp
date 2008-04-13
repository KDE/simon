//
// C++ Implementation: eventhandler
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "eventhandler.h"
#include "shortcutcontrol.h"


EventHandler* EventHandler::instance;
/**
 * @brief Constructor
 * 
 * Creates a new Event Handler
 * 
 * @author Peter Grasch 
 */
EventHandler::EventHandler()
{
#ifdef linux
	this->coreEvents= (CoreEvents*) new XEvents();
#endif
#ifdef __WIN32
	coreEvents= (CoreEvents*) new WindowsEvents();
#endif
	
	this->capslock = false;
}

/**
 * \brief Tells coreEvents to click the coordinates with a simple LMB-Click and release
 * \author Peter Grasch
 * @param x The x coordinate
 * @param y The y coordinate
 */
void EventHandler::click(int x, int y)
{
	if (!coreEvents) return;
		coreEvents->click(x,y);
}

/**
 * @brief Sends a word to the underlying Core Eventhandlers
 *
 * Splits up the word in characters and sends every single one 
 * seperate by using sendKey()
 *
 * @param QString word
 * The word to send
 * 
 * @author Peter Grasch
 */
void EventHandler::sendWord(QString word)
{
	/*#ifdef linux
	sleep(1);
	#endif
	#ifdef __WIN32
	Sleep(1000);
	#endif*/

	for (int i=0; i < word.size();i++)
	{
		sendKey(word.at(i));
	}
}

/**
 * \brief Sends the shortcut to the Backends
 * \author Peter Grasch
 * @param shortcut The shortcut to send
 */
void EventHandler::sendShortcut(Shortcut *shortcut)
{
	if (shortcut)
		coreEvents->sendShortcut(*shortcut);
}

/**
 * @brief Sends a key to the XServer / WinAPI
 * 
 * Sends every key trough the underlying CoreEvent classes.
 * Distincts between Linux and Windows
 * 
 * @param QChar key
 * The key to send
 * 
 * @author Peter Grasch
 * \todo Shift gets unset ALOT
 */
void EventHandler::sendKey(QChar key)
{
	unsigned short c = (unsigned short) key.unicode();

	if (((c >= 'A') && (c <= 'Z'))  || ((capslock) && ((c >= 'a') && (c <= 'z'))))
	{
		coreEvents->setModifierKey(KeyShift,false);
	}
#ifdef __WIN32	
	if (((c >= 'A') && (c <= 'Z')))
	{
		c+=32;
		coreEvents->setModifierKey(VK_LSHIFT,false);
	}
#endif
	coreEvents->sendKey(c);

	coreEvents->unsetModifier(KeyShift);
}

EventHandler::~EventHandler()
{
    delete coreEvents;
}
