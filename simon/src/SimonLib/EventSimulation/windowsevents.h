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
	void sendKey(unsigned short key /*unicode representation*/);
//  	void sendShortcut(const QKeySequence& shortcut);

	inline void sendKey(int key);
	inline void sendChar(char key);
	inline void setModifierKey(int virtualKey, bool once);
	inline void unsetModifier(int virtualKey);
	void unsetUnneededModifiers();
	~WindowsEvents();

};

#endif
