//
// C++ Interface: xevents
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XEVENTS_H
#define XEVENTS_H

#include "coreevents.h"
#include <QMessageBox>
#include <QString>
#include <iostream>

#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysymdef.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <QHash>
#ifdef None
#undef None
#endif


/**
 *	@class XEvents
 *	@brief The X11 Event Backend
 *	
 *	Implements CoreEvents
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Peter Grasch
 */

class XEvents : public CoreEvents {
private:
	Display *display; //!< The opened Display

	bool shiftSet, altgrSet, altSet, superSet, capsSet, strgSet;
	bool shiftOnce, altgrOnce, altOnce, superOnce, capsOnce, strgOnce;

	void pressKey(KeySym key);
	void sendKeySymString(QString keysymString);
public:
	XEvents(char* displayName=":0.0");
	Display* openDisplay(char* displayName);
	
	void click(int x, int y);
	void sendChar(char key);
	void sendKey(unsigned short key);
	void setModifierKey(int virtualKey, bool once);
	void unsetUnneededModifiers();
	void sendShortcut(Shortcut shortcut);
	void unsetModifier(int virtualKey);
	
	~XEvents();

};

#endif
