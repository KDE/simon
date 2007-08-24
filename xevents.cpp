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
	specialkeys.insert( '!', 0x021);
// 	specialkeys.insert( '\n', XStringToKeysym("Return"));
	specialkeys.insert( '"', 0x022);
	specialkeys.insert( '#', 0x023);
	specialkeys.insert( '$', 0x024);
	specialkeys.insert( '%', 0x025);
	specialkeys.insert( '&', 0x026);
	specialkeys.insert( '\'', 0x027);
	specialkeys.insert( '(', 0x028);
	specialkeys.insert( ')', 0x029);
	specialkeys.insert( '*', 0x02a);
	specialkeys.insert( '+', 0x02b);
	specialkeys.insert( ',', 0x02c);
	specialkeys.insert( '-', 0x02d);
	specialkeys.insert( '.', 0x02e);
	specialkeys.insert( '/', 0x02f);
	specialkeys.insert( ':', 0x03a);
	specialkeys.insert( ';', 0x03b);
	specialkeys.insert( '<', 0x03c);
	specialkeys.insert( '=', 0x03d);
	specialkeys.insert( '>', 0x03e);
	specialkeys.insert( '?', 0x03f);
	specialkeys.insert( '@', 0x040);
	specialkeys.insert( '[', 0x05b);
	specialkeys.insert( '\\', 0x05c);
	specialkeys.insert( ']', 0x05d);
	specialkeys.insert( '^', 0x05e);
	specialkeys.insert( '_', 0x05f);
	specialkeys.insert( '`', 0x060);
	specialkeys.insert( '{', 0x07b);
	specialkeys.insert( '|', 0x07c);
	specialkeys.insert( '}', 0x07d);
	specialkeys.insert( '~', 0x07e);
	specialkeys.insert( ' ', 0x0a0);
	specialkeys.insert( '¡', 0x0a1);
	specialkeys.insert( '¢', 0x0a2);
	specialkeys.insert( '£', 0x0a3);
	specialkeys.insert( '¤', 0x0a4);
	specialkeys.insert( '¥', 0x0a5);
	specialkeys.insert( '¦', 0x0a6);
	specialkeys.insert( '§', 0x0a7);
	specialkeys.insert( '¨', 0x0a8);
	specialkeys.insert( '©', 0x0a9);
	specialkeys.insert( 'ª', 0x0aa);
	specialkeys.insert( '«', 0x0ab);
	specialkeys.insert( '¬', 0x0ac);
	specialkeys.insert( '­', 0x0ad);
	specialkeys.insert( '®', 0x0ae);
	specialkeys.insert( '¯', 0x0af);
	specialkeys.insert( '°', 0x0b0);
	specialkeys.insert( '±', 0x0b1);
	specialkeys.insert( '²', 0x0b2);
	specialkeys.insert( '³', 0x0b3);
	specialkeys.insert( '´', 0x0b4);
	specialkeys.insert( 'µ', 0x0b5);
	specialkeys.insert( '¶', 0x0b6);
	specialkeys.insert( '·', 0x0b7);
	specialkeys.insert( 'ç', 0x0b8);
	specialkeys.insert( '¹', 0x0b9);
	specialkeys.insert( 'º', 0x0ba);
	specialkeys.insert( '»', 0x0bb);
	specialkeys.insert( '¼', 0x0bc);
	specialkeys.insert( '½', 0x0bd);
	specialkeys.insert( '¾', 0x0be);
	specialkeys.insert( '¿', 0x0bf);
	specialkeys.insert( 'À', 0x0c0);
	specialkeys.insert( 'Á', 0x0c1);
	specialkeys.insert( 'Â', 0x0c2);
	specialkeys.insert( 'Ã', 0x0c3);
	specialkeys.insert( 'Ä', 0x0c4);
	specialkeys.insert( 'Å', 0x0c5);
	specialkeys.insert( 'Æ', 0x0c6);
	specialkeys.insert( 'Ç', 0x0c7);
	specialkeys.insert( 'È', 0x0c8);
	specialkeys.insert( 'É', 0x0c9);
	specialkeys.insert( 'Ê', 0x0ca);
	specialkeys.insert( 'Ë', 0x0cb);
	specialkeys.insert( 'Ì', 0x0cc);
	specialkeys.insert( 'Í', 0x0cd);
	specialkeys.insert( 'Î', 0x0ce);
	specialkeys.insert( 'Ï', 0x0cf);
	specialkeys.insert( 'Ð', 0x0d0);
	specialkeys.insert( 'Ñ', 0x0d1);
	specialkeys.insert( 'Ò', 0x0d2);
	specialkeys.insert( 'Ó', 0x0d3);
	specialkeys.insert( 'Ô', 0x0d4);
	specialkeys.insert( 'Õ', 0x0d5);
	specialkeys.insert( 'Ö', 0x0d6);
	specialkeys.insert( '×', 0x0d7);
	specialkeys.insert( 'Ø', 0x0d8);
	specialkeys.insert( 'Ù', 0x0d9);
	specialkeys.insert( 'Ú', 0x0da);
	specialkeys.insert( 'Û', 0x0db);
	specialkeys.insert( 'Ü', 0x0dc);
	specialkeys.insert( 'Ý', 0x0dd);
	specialkeys.insert( 'Þ', 0x0de);
	specialkeys.insert( 'ß', 0x0df);
	specialkeys.insert( 'à', 0x0e0);
	specialkeys.insert( 'á', 0x0e1);
	specialkeys.insert( 'â', 0x0e2);
	specialkeys.insert( 'ã', 0x0e3);
	specialkeys.insert( 'ä', 0x0e4);
	specialkeys.insert( 'å', 0x0e5);
	specialkeys.insert( 'æ', 0x0e6);
	specialkeys.insert( 'ç', 0x0e7);
	specialkeys.insert( 'è', 0x0e8);
	specialkeys.insert( 'é', 0x0e9);
	specialkeys.insert( 'ê', 0x0ea);
	specialkeys.insert( 'ë', 0x0eb);
	specialkeys.insert( 'ì', 0x0ec);
	specialkeys.insert( 'í', 0x0ed);
	specialkeys.insert( 'î', 0x0ee);
	specialkeys.insert( 'ï', 0x0ef);
	specialkeys.insert( 'ð', 0x0f0);
	specialkeys.insert( 'ñ', 0x0f1);
	specialkeys.insert( 'ò', 0x0f2);
	specialkeys.insert( 'ó', 0x0f3);
	specialkeys.insert( 'ô', 0x0f4);
	specialkeys.insert( 'õ', 0x0f5);
	specialkeys.insert( 'ö', 0x0f6);
	specialkeys.insert( '÷', 0x0f7);
	specialkeys.insert( 'ø', 0x0f8);
	specialkeys.insert( 'ù', 0x0f9);
	specialkeys.insert( 'ú', 0x0fa);
	specialkeys.insert( 'û', 0x0fb);
	specialkeys.insert( 'ü', 0x0fc);
	specialkeys.insert( 'ý', 0x0fd);
	specialkeys.insert( 'þ', 0x0fe);
	specialkeys.insert( 'ÿ', 0x0ff);

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

void XEvents::sendKey(int key)
{
	
}

void XEvents::sendChar(char key)
{
	if (!display) return;
	bool shiftPressed;
	KeySym keyToSendLowerCase, keyToSendUpperCase;
	KeySym keyToSend=key;
	int syms;

	KeySym shift=XK_Shift_L;

	if (specialkeys.contains(keyToSend)) keyToSend = specialkeys.value(keyToSend);
	else if (key=='\n') {
		keyToSend=XStringToKeysym("Return");
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
// 		qDebug() << "bin da";
		setModifierKey(KeyAltGr, true);
	}
	else unsetModifier(KeyAltGr);

// 	qDebug() << keyToSend << (int) key << keyToSendShifted[0] << keyToSendShifted[1]
// 			 << keyToSendShifted[2] << keyToSendShifted[3];
	
	XTestFakeKeyEvent(display, XKeysymToKeycode(display, keyToSend), True, 15);
	XTestFakeKeyEvent(display, XKeysymToKeycode(display, keyToSend), False, 15);
	XFlush ( display );




	if (shiftOnce) {
		shiftOnce=false;
		unsetModifier(KeyShift);
	}
	if (altgrOnce) {
		altgrOnce=false;
		unsetModifier(KeyAltGr);
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
	if ((!shiftSet) && (virtualKey==KeyShift))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), True, 5);
		XFlush ( display );
		shiftSet=true;
		shiftOnce=once;
	}
	if ((!altgrSet) && (virtualKey==KeyAltGr))
	{
// 		qDebug() << "setzte gerade in modifier";
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), True, 5);
		XFlush ( display );
		altgrSet=true;
		altgrOnce=once;
	}
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
	if ((virtualKey==KeyShift))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), False, 5);
		XFlush ( display );
		shiftSet=false;
	}
	if ((virtualKey==KeyAltGr))
	{
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), False, 5);
		XFlush ( display );
		altgrSet=false;
	}
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


