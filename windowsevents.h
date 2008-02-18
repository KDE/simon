//
// 
//
// Description: 
//
//
// Author: Phillip Goriup , (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WINDOWSEVENTS_H
#define WINDOWSEVENTS_H

#include "coreevents.h"
#include <QString>
#include <windows.h>
#include <QHash>


/*const int KeyAlt = VK_MENU;
const int KeyStrg = VK_CONTROL;
const int KeyShift = VK_SHIFT;
const int KeySuper = VK_LWIN;
const int KeyAltGr = VK_RMENU;
const int KeyCapsLock = VK_CAPITAL;*/

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
	void sendKey(unsigned short key /*unicode representation*/);
	void sendShortcut(Shortcut shortcut);

	inline void sendKey(int key);
	inline void sendChar(char key);
	inline void setModifierKey(int virtualKey, bool once);
	inline void unsetModifier(int virtualKey);
	void unsetUnneededModifiers();
	~WindowsEvents();

};

#endif
