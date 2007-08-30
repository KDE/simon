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
	coreEvents= (CoreEvents*) new XEvents();
#endif
#ifdef __WIN32
	coreEvents= (CoreEvents*) new WindowsEvents();
	//WindowsEvents *win = new WindowsEvents();
#endif
	this->capslock = false;
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
	//sleep(1);
	for (int i=0; i < word.size();i++)
	{
		sendKey(word.at(i));
	}
}

/**
 * @brief Sends a key to the XServer / DirectInput (?)
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
	char c = key.toLatin1();

	if (((c >= 'A') && (c <= 'Z'))  || ((capslock) && ((c >= 'a') && (c <= 'z'))))
	{
		coreEvents->setModifierKey(KeyShift,false);
	}
#ifdef __WIN32	
	if (((c >= 'A') && (c <= 'Z')))
	{
		c+=32;
	}
#endif
	coreEvents->sendChar(c);

	coreEvents->unsetModifier(KeyShift);
}

/**
 * @brief Sets a modifier key (Alt / Strg / etc.)
 *
 * @param int virtualKey
 * The keycode of the modifier
 * @param bool once
 * If set the modifier will be un-toggled after one "normal" key
 * 
 * @author Peter Grasch
 */
void EventHandler::setModifier(int virtualKey, bool once)
{
	
}

/**
 * @brief Un-Sets a modifier key (Alt / Ctrl / etc.)
 *
 * @param int virtualKey
 * The keycode of the modifier
 * 
 * @author Peter Grasch
 */
void EventHandler::unsetModifier(int virtualKey)
{
	
}

/**
 * @brief Sends a raw KeyCode
 *
 * @param int keycode
 * The keycode of the key to send
 * 
 * @author Peter Grasch
 */
void EventHandler::sendKeyCode(int keycode)
{
	
}

/**
 * @brief Runs a command key (e.g. Ctrl+Alt+Del)
 *
 * @param int keycombination
 * The keycombination - this is not yet defined
 * @todo Define the combination structure
 * 
 * @author Peter Grasch
 */
void EventHandler::runCommandKey(int keycombination)
{
	
}