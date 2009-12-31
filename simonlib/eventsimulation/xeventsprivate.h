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


#ifndef SIMON_XEVENTSPRIVATE_H_19828198203F448BA166FF11295B2B25
#define SIMON_XEVENTSPRIVATE_H_19828198203F448BA166FF11295B2B25

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
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

class XEventsPrivate {
private:
	Display *display; //!< The opened Display


	void pressKey(const KeySym& key);
	void pressKeyCode(const KeyCode& code);
	void sendKeySymString(const QString& keysymString);
public:
	void click(int x, int y);
	void sendKeyPrivate(unsigned int key);

	void setModifierKey(int virtualKey);

	void unsetModifier(int virtualKey);

	XEventsPrivate(char* displayName);
	~XEventsPrivate();
	Display* openDisplay(char* displayName);
};

#endif //XEVENTSPRIVATE_H
