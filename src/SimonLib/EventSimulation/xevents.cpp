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
#include <QHash> 	// this needs to be included first because the 
#include <KMessageBox>	// X11 headers included in the xevents header define "Status"



#include "xevents.h"
#include <KLocalizedString>
#include "../Logging/logger.h"


#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XTest.h>

#ifdef None
#undef None
#endif

#ifdef KeyPress
#undef KeyPress
#endif

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
XEvents::XEvents(char* displayName) : CoreEvents()
{
	display = openDisplay(displayName);
}


/**
 * @brief Opens the Display and returns the handle
 *
 * @param int keycombination
 * The keycombination
 * 
 * @author Peter Grasch
 */
Display* XEvents::openDisplay(char* displayName)
{
	int Event, Error;
	int Major, Minor;
	
	Logger::log(i18n("[INF] Öffne display \"%1\"", QString(displayName)));

	Display * display = XOpenDisplay(displayName);

	if (!display) {
		Logger::log(i18n("[ERR] Fehler beim öffnen des display \"%1\"", QString(displayName)));
		KMessageBox::error(0,i18n("Konnte Display nicht öffnen. Bitte überprüfen Sie ihre Konfiguration und / oder setzen Sie sich mit den simon-Entwickler in Verbindung. (Display: \"%1\")", QString(XDisplayName ( displayName ))));
		return NULL;
	}

	//check whether the XTest extension is installed
	if ( !XTestQueryExtension(display, &Event, &Error, &Major, &Minor) ) {
		Logger::log("[ERR] Display "+QString(displayName)+" unterstützt XTest nicht");
		KMessageBox::error(0,i18n("Der X-Server unterstützt die \"XTest\" nicht - bitte installieren Sie diese. (Display: \"%1\")", QString(DisplayString(display))));

		XCloseDisplay(display);
		return NULL;
	}


	//The following should be logged somewhere... Interresting for debugging purposes...
	//We'll do that once we have the logging classes...
	Logger::log(i18n("[INF] XTest für Server \"%1\" ist Version %2.%3", QString(DisplayString(display)), Major, Minor));

	Logger::log(i18n("[INF] Aufnahme der Display-Kontrolle"));
	XTestGrabControl( display, True ); 
	Logger::log(i18n("[INF] Synchronisiere Display"));
	XSync( display,True ); 
	return display;
}

/**
 * \brief Clicks the coordinates with a simple LMB-Click and release
 * \author Peter Grasch
 * @param x The x coordinate
 * @param y The y coordinate
 */
void XEvents::click(int x, int y)
{
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
 * \brief Overloaded function; Calls sendKey(unsigned int)
 * @param key The key will be casted to unsigned short and passed to sendKey
 */
void XEvents::sendChar(char key)
{
	sendKey((unsigned int) key);
}

/**
 * \brief Sends the raw unicode character given by key
 * \author Peter Grasch
 * @param key The key to send
 */
void XEvents::sendKey(unsigned int key /*unicode*/)
{
	if (!display) return;
	KeyCode keyCode = XKeysymToKeycode(display, key);
	
	if (keyCode)
	{
		KeySym shiftSym = XKeycodeToKeysym(display, keyCode, 1);
		KeySym altGrSym = XKeycodeToKeysym(display, keyCode, 2);
		
		if (shiftSym == key)
			setModifierKey(Qt::SHIFT, true);
		else if (altGrSym == key)
			setModifierKey(Qt::Key_AltGr, true);
		
		pressKeyCode(keyCode);
	} else {
		QKeySequence k(key); //do some magic
		pressKey(XStringToKeysym(k.toString().toUtf8().data())); //this was: toLatin1(); TEST THIS!
	}
	
	unsetUnneededModifiers();
}

void XEvents::pressKey(KeySym key)
{
	pressKeyCode(XKeysymToKeycode(display, key));
}

void XEvents::pressKeyCode(KeyCode code)
{
	XTestFakeKeyEvent(display, code, True, 15);
	XTestFakeKeyEvent(display, code, False, 15);
	XFlush ( display );
}


/**
 * @brief Sets the modifier key
 *
 * @param int virtualKey
 * The keycode
 * @param bool once
 * Whether the modifier should be unset after one "normal" key
 * 
 * @author Peter Grasch
 */
void XEvents::setModifierKey(int virtualKey, bool once)
{
	if ((!shiftSet) && (virtualKey & Qt::SHIFT))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), True, 5);
		shiftSet=true;
		shiftOnce=once;
	}
	if ((!altgrSet) && (virtualKey & Qt::Key_AltGr))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), True, 5);
		altgrSet=true;
		altgrOnce=once;
	}
	if ((!strgSet) && (virtualKey & Qt::CTRL))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), True, 5);
		strgSet=true;
		strgOnce=once;
	}
	if ((!altSet) && (virtualKey & Qt::ALT))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), True, 5);
		altSet=true;
		altOnce=once;
	}
	if ((!superSet) && (virtualKey & Qt::META))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), True, 5);
		superSet=true;
		superOnce=once;
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
	if ((virtualKey & Qt::SHIFT))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), False, 5);
		shiftSet=false;
	}
	if ((virtualKey & Qt::Key_AltGr))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), False, 5);
		altgrSet=false;
	}
	if ((virtualKey & Qt::CTRL))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), False, 5);
		strgSet=false;
	}
	if ((virtualKey & Qt::ALT))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), False, 5);
		altSet=false;
	}
	if ((virtualKey & Qt::META))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), False, 5);
		superSet=false;
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
	XCloseDisplay(display);
	delete display;
}


