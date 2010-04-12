/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QString>
// this needs to be included first because the 
#include <KMessageBox>	// X11 headers included in the xevents header define "Status"

#include <unistd.h>
#include <KDebug>
#include <KLocale>
#include "xeventsprivate.h"
// #include "../Logging/logger.h"
#include <KLocalizedString>

XEventsPrivate::XEventsPrivate(char* displayName)
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
Display* XEventsPrivate::openDisplay(char* displayName)
{
	int Event, Error;
	int Major, Minor;
	
// 	Logger::log(i18n("[INF] Öffne display \"%1\"", QString(displayName)));

	Display * display = XOpenDisplay(displayName);

	if (!display) {
// 		Logger::log(i18n("[ERR] Fehler beim öffnen des display \"%1\"", QString(displayName)));
		KMessageBox::error(0,i18n("Couldn't open display. Please check your configuration and / or contact the simond developers. (Display: \"%1\")", QString(XDisplayName ( displayName ))));
		return NULL;
	}

	//check whether the XTest extension is installed
	if ( !XTestQueryExtension(display, &Event, &Error, &Major, &Minor) ) {
// 		Logger::log("[ERR] Display "+QString(displayName)+" unterstützt XTest nicht");
		KMessageBox::error(0,i18n("The X-Server does not support the XTest extension. Please install it."));

		XCloseDisplay(display);
		return NULL;
	}


// 	Logger::log(i18n("[INF] XTest für Server \"%1\" ist Version %2.%3", QString(DisplayString(display)), Major, Minor));

// 	Logger::log(i18n("[INF] Aufnahme der Display-Kontrolle"));
	XTestGrabControl( display, True ); 
// 	Logger::log(i18n("[INF] Synchronisiere Display"));
	XSync( display,True ); 
	return display;
}

/**
 * \brief Clicks the coordinates with a simple LMB-Click and release
 * \author Peter Grasch
 * @param x The x coordinate
 * @param y The y coordinate
 */
void XEventsPrivate::click(int x, int y, EventSimulation::ClickMode clickMode)
{
	if (!display) return;

	XTestFakeMotionEvent(display, 0, x, y, 10);

	switch (clickMode) {
		case EventSimulation::LMB:
			XTestFakeButtonEvent(display, 1, true, 10);
			XTestFakeButtonEvent(display, 1, false, 10);
			break;
		case EventSimulation::LMBDouble:
			for (int i=0; i<2; i++) {
				XTestFakeButtonEvent(display, 1, true, 10);
				XTestFakeButtonEvent(display, 1, false, 10);
			}
			break;
		case EventSimulation::LMBDown:
			XTestFakeButtonEvent(display, 1, true, 10);
			break;
		case EventSimulation::LMBUp:
			XTestFakeButtonEvent(display, 1, false, 10);
			break;
		case EventSimulation::RMB:
			XTestFakeButtonEvent(display, 3, true, 10);
			XTestFakeButtonEvent(display, 3, false, 10);
			break;
		case EventSimulation::MMB:
			XTestFakeButtonEvent(display, 2, true, 10);
			XTestFakeButtonEvent(display, 2, false, 10);
			break;
	}

	XFlush(display);
}

void XEventsPrivate::dragAndDrop(int xStart, int yStart, int x, int y)
{
	if (!display) return;

	XTestFakeMotionEvent(display, 0, xStart, yStart, 10);
	XTestFakeButtonEvent(display, 1, true, 10);
	XTestFakeMotionEvent(display, 0, x, y, 150);
	XTestFakeButtonEvent(display, 1, false, 10);

	XFlush(display);
}


/**
 * \brief Resolves the string to an appropriate keysym and sends it using pressKey(...)
 * \author Peter Grasch
 * @param keysymString The string to convert/send
 * @see sendKey()
 */
void XEventsPrivate::sendKeySymString(const QString& keysymString)
{
	pressKey(XStringToKeysym(keysymString.toAscii().data()));
}


/**
 * \brief Sends the raw unicode character given by key
 * \author Peter Grasch
 * @param key The key to send
 */
void XEventsPrivate::sendKeyPrivate(unsigned int key /*unicode*/)
{
	if (!display) return;
	KeyCode keyCode;
	
	switch (key)
	{
		case 9: 
			key = XK_Tab;
			break;
		case 10:
			key = XK_Return;
			break;
		case 16777219:
			key = XK_BackSpace;
			break;
		case 16777216:
			key = XK_Escape;
			break;
		case 16777238:
			key = XK_Prior;
			break;
		case 16777239:
			key = XK_Next;
			break;
		case 16777223:
			key = XK_Delete;
			break;
		case Qt::Key_VolumeUp:
			key = XF86XK_AudioRaiseVolume;
			break;
		case Qt::Key_VolumeDown:
			key = XF86XK_AudioLowerVolume;
			break;
		case Qt::Key_VolumeMute:
			key = XF86XK_AudioMute;
			break;
		case Qt::Key_MediaPlay:
			key = XF86XK_AudioPlay;
			break;
		case Qt::Key_MediaStop:
			key = XF86XK_AudioStop;
			break;
		case Qt::Key_MediaPrevious:
			key = XF86XK_AudioPrev;
			break;
		case Qt::Key_MediaNext:
			key = XF86XK_AudioNext;
			break;
		case Qt::Key_MediaRecord:
			key = XF86XK_AudioRecord;
			break;
			
		/* BEGIN DEADKEYS */
		case 94:
			key = XK_dead_circumflex;
			break;
		case 180:
			key = XK_dead_acute;
			break;
		case 96:
			key = XK_dead_grave;
			break;

		case 184:
			key = XK_dead_cedilla;
			break;
		case 126:
			key = XK_dead_tilde;
			break;
		case 803:
			key = XK_dead_belowdot;
			break;
		case 775:
			key = XK_dead_abovedot;
			break;
		case 776:
			key = XK_dead_diaeresis;
			break;
		case 778:
			key = XK_dead_abovering;
			break;
		case 772:
			key = XK_dead_macron;
			break;
		case 780:
			key = XK_dead_caron;
			break;
		case 779:
			key = XK_dead_doubleacute;
			break;
		case 808:
			key = XK_dead_ogonek;
			break;
		/* END DEADKEYS */

		case 913:
			key = XK_Greek_ALPHA;
	}
	
	keyCode = XKeysymToKeycode(display, key);
	kDebug() << "Requesting key: " << key << keyCode;
	
	if (keyCode)
	{
		int syms;
		KeySym *keyToSendShifted=XGetKeyboardMapping(display, keyCode, 1, &syms);
		if (!keyToSendShifted) return;
		KeySym shiftSym = keyToSendShifted[1]; //XKeycodeToKeysym(display, keyCode, 1);
		KeySym altGrSym = 0;
		KeySym altGrShiftSym = 0;
		if (syms >= 4)
			altGrSym = keyToSendShifted[4];
		if (syms >= 5)
			altGrShiftSym = keyToSendShifted[5];
		
		kDebug() << "here: " << key << shiftSym << altGrSym << altGrShiftSym;
		XFree(keyToSendShifted);
		
		if (((shiftSym == key) || (altGrShiftSym == key)) && (key < 0xff08))
		{
			setModifierKey(Qt::SHIFT);
		}
		if (((key==altGrSym) || (altGrShiftSym == key)) && (key < 0xff08)) {
			setModifierKey(Qt::Key_AltGr);
		}
		
		pressKeyCode(keyCode);
		
		if (((shiftSym == key) || (altGrShiftSym == key)) && (key < 0xff08))
			unsetModifier(Qt::SHIFT);
		if (((key==altGrSym) || (altGrShiftSym == key)) && (key < 0xff08))
			unsetModifier(Qt::Key_AltGr);
	} else {
		QKeySequence k(key); //do some magic
		QString shortcut = k.toString(); //somthing like "Ctrl+L"
		QStringList keys = shortcut.split("+"); 
		QList<KeyCode> shortcutCodes;
		
		foreach (const QString keyStr, keys)
		{
			shortcutCodes << XKeysymToKeycode(display, XStringToKeysym(keyStr.toUtf8().data()));
		}
		
		foreach (KeyCode shortcutCode, shortcutCodes)
			XTestFakeKeyEvent(display, shortcutCode, True, 15);
		
		foreach (KeyCode shortcutCode, shortcutCodes)
			XTestFakeKeyEvent(display, shortcutCode, False, 15);
		
		XFlush ( display );
	}
}

void XEventsPrivate::pressKey(const KeySym& key)
{
	pressKeyCode(XKeysymToKeycode(display, key));
}

void XEventsPrivate::pressKeyCode(const KeyCode& code)
{
	XTestFakeKeyEvent(display, code, True, 15);
	XTestFakeKeyEvent(display, code, False, 15);
	XFlush ( display );
}


/**
 * @brief Sets the modifier key
 *
 * @param int virtualKey
 * The keycode (see Qts Key)
 * 
 * @author Peter Grasch
 */
void XEventsPrivate::setModifierKey(int virtualKey)
{
	switch (virtualKey)
	{
		case Qt::SHIFT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), True, 5);
			break;
			
		case Qt::Key_AltGr:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), True, 5);
			break;
			
		case Qt::CTRL:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), True, 5);
			break;
			
		case Qt::ALT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), True, 5);
			break;
			
		case Qt::META:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), True, 5);
			break;
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
void XEventsPrivate::unsetModifier(int virtualKey)
{
	switch (virtualKey)
	{
		case Qt::SHIFT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), False, 5);
			break;
		
		case Qt::Key_AltGr:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), False, 5);
			break;
		
		case Qt::CTRL:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), False, 5);
			break;
		
		case Qt::ALT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), False, 5);
			break;
		
		case Qt::META:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), False, 5);
			break;
	}
	XFlush ( display );
}



XEventsPrivate::~XEventsPrivate()
{
	XCloseDisplay(display);
	XFree(display);
}

