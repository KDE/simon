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
#include <QMessageBox>

/**
 * @brief Constructor
 * 
 * Creates a new Event Handler
 * 
 * @author Peter Grasch 
 */
EventHandler::EventHandler(ShortcutControl *shortcutControl)
{
#ifdef linux
	coreEvents= (CoreEvents*) new XEvents();
#endif
#ifdef __WIN32
	coreEvents= (CoreEvents*) new WindowsEvents();
#endif
	this->shortcutControl = shortcutControl;
	if (!this->shortcutControl) this->shortcutControl = new ShortcutControl();
	if (!this->shortcutControl->readShortcuts())
	{
		QMessageBox::critical(0, "Dateifehler", 
			"Beim auslesen der Tastenkürzel ist ein Fehler aufgetreten.");
	}
	
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
	#ifdef linux
	sleep(1);
	#endif
	#ifdef __WIN32
	Sleep(1000);
	#endif

	Shortcut *test = this->shortcutControl->getShortcut(word);
	if (test)
	{
	//	coreEvents->sendShortcut(*test);
		return;
	}

	for (int i=0; i < word.size();i++)
	{
		sendKey(word.at(i));
	}
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
	}
#endif
	//coreEvents->sendKey(c);

	coreEvents->unsetModifier(KeyShift);
}
