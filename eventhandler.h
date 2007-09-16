//
// C++ Interface: eventhandler
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QString>
#include <QChar>
#include "coreevents.h"

#ifdef linux
#include "xevents.h"
#endif
#ifdef __WIN32
#include "windowsevents.h"
#endif

class ShortcutControl;

/**
 *	@class EventHandler
 *	@brief Manages the underlying CoreEventhandlers
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Peter Grasch
 */
class EventHandler{
private:
	CoreEvents *coreEvents; //!< The event backend
	bool capslock;
public:
	EventHandler();
	void click(int x, int y);
	void sendShortcut(Shortcut *shortcut);
	void sendWord(QString word);
	void sendKey(QChar key);
	void setModifier(int virtualKey, bool once);
	void unsetModifier(int virtualKey);
	void sendKeyCode(int keycode);
	void runCommandKey(int keycombination);
};

#endif
