//
// C++ Implementation: xevents
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "logger.h"
#include <QCoreApplication>
#include <QDebug>
#include "xevents.h"
/**
 * @brief Constructor
 * 
 * Opens the display (member)
 *
 * @param char* displayName
 * The name of the display to open
 * 
 * @author Peter Grasch
 */
XEvents::XEvents(char* displayName)
{
	display = openDisplay(displayName);
	strgSet=false;
	shiftSet=false;
	altgrSet=false;
	altSet=false;
	superSet=false;
	capsSet=false;

	strgOnce=false;
	shiftOnce=false;
	altgrOnce=false;
	altOnce=false;
	superOnce=false;
	capsOnce=false;
}


/**
 * @brief Opens the Display and returns the handle
 *
 * @param int keycombination
 * The keycombination - this is not yet defined
 * @todo Define the combination structure
 * 
 * @author Peter Grasch
 */
Display* XEvents::openDisplay(char* displayName)
{
	int Event, Error;
	int Major, Minor;
	
	Logger::log(QCoreApplication::tr("[INF] Öffne display \"%1\"").arg(QString(displayName)));

	Display * display = XOpenDisplay(displayName);

	if (!display) {
		Logger::log(QCoreApplication::tr("[ERR] Fehler beim öffnen des display \"%1\"").arg(QString(displayName)));
		QMessageBox::critical(0,QCoreApplication::tr("Error"),QString(QCoreApplication::tr("Konnte Display nicht öffnen. Bitte überprüfen Sie ihre Konfiguration und / oder setzen Sie sich mit den simon-Entwickler in Verbindung. (Display: \""))+QString(XDisplayName ( displayName ))+QString("\")"));
		return NULL;
	}

	//check wether the XTest extension is installed
	if ( !XTestQueryExtension(display, &Event, &Error, &Major, &Minor) ) {
		Logger::log("CRITICAL: Display "+QString(displayName)+" does not support XTest");
		QMessageBox::critical(0,"Fehler","Der X-Server unterstützt die \"XTest\" nicht - bitte kontaktieren sie die simon-Entwickler. (Display: \""+QString(DisplayString(display)) + "\")");

		XCloseDisplay(display);
		return NULL;
	}


	//The following should be logged somewhere... Interresting for debugging purposes...
	//We'll do that once we have the logging classes...
	Logger::log(QCoreApplication::tr("[INF] Test für Server \"%1\" ist Version %2.%3").arg(QString(DisplayString(display))).arg(Major).arg(Minor));

	Logger::log(QCoreApplication::tr("[INF] Aufnahme der display Kontrolle"));
	XTestGrabControl( display, True ); 
	Logger::log(QCoreApplication::tr("[INF] Syncronisiere Display"));
	XSync( display,True ); 
	return display;
}



/**
 * \brief Sends the given Shortcut
 * 
 * This will set all the given modifiers and keys and then revert to the original
 * position (assuming the once flag is correctly interpreted)
 * 
 * \author Peter Grasch
 * @param shortcut The shortcut to send
 */
void XEvents::sendShortcut(Shortcut shortcut)
{
	setModifierKey(shortcut.getModifiers(), true);

	int action = shortcut.getActionKeys();
	if (action & KeyBackspace)
		sendKeySymString("BackSpace");
	if (action & KeyEscape)
		sendKeySymString("Escape");
	if (action & KeyClear)
		sendKeySymString("Clear");
	if (action & KeyPrintScr)
		sendKeySymString("Sys_Req");
	if (action & KeyPause)
		sendKeySymString("Pause");
	if (action & KeyUndo)
		sendKeySymString("Undo");
	if (action & KeyRedo)
		sendKeySymString("Redo");
	if (action & KeyEnter)
		sendKeySymString("Return");

	int movement = shortcut.getMovementKeys();
	if (movement & KeyArrowLeft) sendKeySymString("Left");

	if (movement & KeyArrowRight)
		sendKeySymString("Right");
	if (movement & KeyArrowDown)
		sendKeySymString("Down");
	if (movement & KeyArrowUp)
		sendKeySymString("Up");
	if (movement & KeyPageUp)
		sendKeySymString("Page_Up");
	if (movement & KeyPageDown)
		sendKeySymString("Page_Down");
	if (movement & KeyEnd)
		sendKeySymString("End");
	if (movement & KeyBegin)
		sendKeySymString("Home");


	int fkeys = shortcut.getFunctionKeys();
	if (fkeys & KeyF1) sendKeySymString("F1");
	if (fkeys & KeyF2) { sendKeySymString("F2"); }
	if (fkeys & KeyF3) sendKeySymString("F3");
	if (fkeys & KeyF4) sendKeySymString("F4");
	if (fkeys & KeyF5) sendKeySymString("F5");
	if (fkeys & KeyF6) sendKeySymString("F6");
	if (fkeys & KeyF7) sendKeySymString("F7");
	if (fkeys & KeyF8) sendKeySymString("F8");
	if (fkeys & KeyF9) sendKeySymString("F9");
	if (fkeys & KeyF10) sendKeySymString("F10");
	if (fkeys & KeyF11) sendKeySymString("F11");
	if (fkeys & KeyF12) sendKeySymString("F12");

	char key = shortcut.getCharKey();
	if (key != '_') pressKey(key);

	unsetUnneededModifiers();
}

/**
 * \brief Clicks the coordinates with a simple LMB-Click and release
 * \author Peter Grasch
 * @param x The x coordinate
 * @param y The y coordinate
 */
void XEvents::click(int x, int y)
{
	//IMPLEMENT ME
	if (!display) return;
	XTestFakeMotionEvent(display, 0, x, y, 10);
	XTestFakeButtonEvent(display, 1, true, 10);
	XTestFakeButtonEvent(display, 1, false, 10);
	XFlush(display);
}

/**
 * \brief Resolves the string to an appropriate keysym and sends it using pressKey(...)
 * \author Peter Grasch
 * @param keysymString The string to convert/send
 * @see sendKey()
 */
void XEvents::sendKeySymString(QString keysymString)
{
	pressKey(XStringToKeysym(keysymString.toAscii().data()));
}

/**
 * \brief Overloaded function; Calls sendKey(unsigned short)
 * @param key The key will be casted to unsigned short and passed to sendKey
 */
void XEvents::sendChar(char key)
{
	sendKey((unsigned short) key);
}

/**
 * \brief Sends the raw unicode character given by key
 * \author Peter Grasch
 * @param key The key to send
 */
void XEvents::sendKey(unsigned short key /*unicode*/)
{
	if (!display) return;

	KeySym keyToSendLowerCase, keyToSendUpperCase;
	KeySym keyToSend=key;

	int syms;

	KeySym shift=XK_Shift_L;

	if (key=='\n') {
		keyToSend=XStringToKeysym("Return");
	} else 
	if (key=='\t') {
		keyToSend=XStringToKeysym("Tab");
	}


	KeyCode keyToSendcode = XKeysymToKeycode(display, keyToSend);
	KeySym *keyToSendShifted=XGetKeyboardMapping(display, keyToSendcode, 1, &syms);
	if (!keyToSendShifted) return;	//get the keyToSendboard mapping and go back
	for (; syms && (!keyToSendShifted[syms-1]); syms--); //to the first in the list
	if (!syms) return;	//return on error
	XConvertCase(keyToSend,&keyToSendLowerCase,&keyToSendUpperCase); //extract the keyToSend with and
	//without shift
	//if the keyToSend is in the retrieved list and upper and lowercase are both the same original
	//keyToSend - shift is not nescessairy
	if (keyToSend==keyToSendShifted[0] && (keyToSend==keyToSendLowerCase && keyToSend==keyToSendUpperCase)) shift=NoSymbol;
	//if the original keyToSend is the same as the keyToSend with shift and the uppercase version isn't
	//we shouldn't press shift!
	if (keyToSend==keyToSendLowerCase && keyToSend!=keyToSendUpperCase) shift=NoSymbol;

	if (shift!=NoSymbol && (!(key==keyToSendShifted[2])))
		setModifierKey(KeyShift, true);
	else unsetModifier(KeyShift);

	if (keyToSend==keyToSendShifted[2]) {
		unsetModifier(KeyShift);
		setModifierKey(KeyAltGr, true);
	}
	else unsetModifier(KeyAltGr);


	pressKey(keyToSend);

	unsetUnneededModifiers();
}


void XEvents::pressKey(KeySym key)
{
	XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), True, 15);
	XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), False, 15);
	XFlush ( display );
}


void XEvents::unsetUnneededModifiers()
{
	if (shiftOnce) {
		shiftOnce=false;
		unsetModifier(KeyShift);
	}
	if (altgrOnce) {
		altgrOnce=false;
		unsetModifier(KeyAltGr);
	}
	if (strgOnce) {
		strgOnce=false;
		unsetModifier(KeyStrg);
	}
	if (altOnce) {
		altOnce=false;
		unsetModifier(KeyAlt);
	}
	if (superOnce) {
		superOnce=false;
		unsetModifier(KeySuper);
	}
	if (capsOnce) {
		capsOnce=false;
		unsetModifier(KeyCapsLock);
	}
}


/**
 * @brief Sets the modifier key
 *
 * @param int virtualKey
 * The keycode
 * @param bool once
 * Wether the modifier should be unset after one "normal" key
 * 
 * @author Peter Grasch
 */
void XEvents::setModifierKey(int virtualKey, bool once)
{
	if ((!shiftSet) && (virtualKey & KeyShift))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), True, 5);
		shiftSet=true;
		shiftOnce=once;
	}
	if ((!altgrSet) && (virtualKey & KeyAltGr))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), True, 5);
		altgrSet=true;
		altgrOnce=once;
	}
	if ((!strgSet) && (virtualKey & KeyStrg))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), True, 5);
		strgSet=true;
		strgOnce=once;
	}
	if ((!altSet) && (virtualKey & KeyAlt))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), True, 5);
		altSet=true;
		altOnce=once;
	}
	if ((!superSet) && (virtualKey & KeySuper))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), True, 5);
		superSet=true;
		superOnce=once;
	}
	if ((!capsSet) && (virtualKey & KeyCapsLock))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Caps_Lock), True, 5);
		capsSet=true;
		capsOnce=once;
	}
	
	XFlush ( display );
}

/**
 * @brief Un-Set the modifier
 *
 * @param int virtualKey
 * The keycode
 * 
 * @author Peter Grasch
 */
void XEvents::unsetModifier(int virtualKey)
{
	if ((virtualKey & KeyShift))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), False, 5);
		shiftSet=false;
	}
	if ((virtualKey & KeyAltGr))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), False, 5);
		altgrSet=false;
	}
	if ((virtualKey & KeyStrg))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), False, 5);
		strgSet=false;
	}
	if ((virtualKey & KeyAlt))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), False, 5);
		altSet=false;
	}
	if ((virtualKey & KeySuper))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), False, 5);
		superSet=false;
	}
	if ((virtualKey & KeyCapsLock))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Caps_Lock), False, 5);
		capsSet=false;
	}
	XFlush ( display );
}

/**
 * @brief Destructor
 *
 * 
 *
 * @author Peter Grasch
 */
XEvents::~XEvents()
{
}


