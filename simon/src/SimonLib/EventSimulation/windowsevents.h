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


#ifndef WINDOWSEVENTS_H
#define WINDOWSEVENTS_H

#include "coreevents.h"
#include <QString>
#include <windows.h>
#include <QHash>

/**
 *	@class WindowsEvents
 *	@brief The WinAPI Event Backend
 *	
 *	Implements CoreEvents
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Phillip Goriup
 */
class WindowsEvents : public CoreEvents {
private:

	int modloger;
	QHash <char, int> *keycodes; 
	QHash <char, char> *shiftcodes;
	QHash <char, char> *altgrcodes;
	QHash <int, char> *specialcodes;

public:
	WindowsEvents();
	void click(int x, int y);
	void sendKey(unsigned int key /*unicode representation*/);

	inline void sendChar(char key);
	inline void setModifierKey(int virtualKey, bool once);
	inline void unsetModifier(int virtualKey);
	void unsetUnneededModifiers();
	~WindowsEvents();

};

#endif
