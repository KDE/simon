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
	
	Logger::log(tr("[INF] Öffne display \"%1\"").arg(QString(displayName)));

	Display * display = XOpenDisplay(displayName);

	if (!display) {
		Logger::log(tr("[ERR] Fehler beim öffnen des display \"%1\"").arg(QString(displayName));
		QMessageBox::critical(0,tr("Error"),QString(tr("Konnte Display nicht öffnen. Bitte überprüfen Sie ihre Konfiguration und / oder setzen Sie sich mit den simon-Entwickler in Verbindung. (Display: \""))+QString(XDisplayName ( displayName ))+QString("\")"));
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
	Logger::log(tr("[INF] Test für Server \"%1\" ist Version %2.%3").arg(QString(DisplayString(display))).arg(Major).arg(Minor));

	Logger::log(tr("[INF] Aufnahme der display Kontrolle"));
	XTestGrabControl( display, True ); 
	Logger::log(tr("[INF] Syncronisiere Display"));
	XSync( display,True ); 
	return display;
}

void XEvents::sendKey(int key)
{
	
}

void XEvents::sendChar(char key)
{
	int delay = 15;
	KeySym ks, sks, *kss, ksl, ksu;
	KeyCode kc, skc;
	int syms;

	sks=XK_Shift_L;
	
	switch (key)
	{
		case ' ':
			ks=XStringToKeysym("space");
			break;
		case '\n':
			ks=XStringToKeysym("return");
			break;
		default:	
			char* keyarr = new char[2];
			
			keyarr[0]=key;
			keyarr[1]='\0';
			ks=XStringToKeysym(keyarr);
			break;
	}
			
	
	if ( ( kc = XKeysymToKeycode ( display, ks ) ) == 0 )
	{
		Logger::log(tr("[INF] Kein keycode gefunden"));
		return;
	}
	if ( ( skc = XKeysymToKeycode ( display, sks ) ) == 0 )
	{
		Logger::log(tr("[INF] kein keycode gefunden"));
		return;
	}

	kss=XGetKeyboardMapping(display, kc, 1, &syms);
	if (!kss)
	{
		Logger::log(tr("[INF] XGetKeyBoardMapping am Remote-Display fehlgeschlagen für \"%1\"").arg(QString(kc)));
		return;
	}
	for (; syms && (!kss[syms-1]); syms--);
	if (!syms)
	{
		Logger::log(tr("[INF] XGetKeyBoardMapping am Remote-Display fehlgeschlagen für \"%1\"").arg(QString(kc)));
		XFree(kss);
	}
	XConvertCase(ks,&ksl,&ksu);
	
	if (ks==kss[0] && (ks==ksl && ks==ksu)) sks=NoSymbol;
	if (ks==ksl && ks!=ksu) sks=NoSymbol;
	if (sks!=NoSymbol) XTestFakeKeyEvent ( display, skc, True, delay );
	XTestFakeKeyEvent ( display, kc, True, delay );
	XFlush ( display );
	XTestFakeKeyEvent ( display, kc, False, delay );
	if (sks!=NoSymbol) XTestFakeKeyEvent ( display, skc, False, delay );
	XFlush ( display );
	XFree(kss);
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


