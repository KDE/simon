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

#ifdef Q_OS_UNIX
#include "xevents.h"
#endif
#ifdef def Q_OS_WIN
#include "windowsevents.h"
#endif

// class ShortcutControl;

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
	static EventHandler* instance;
	CoreEvents *coreEvents; //!< The event backend
	bool capslock;
protected:
	EventHandler();

public:
	static EventHandler* getInstance() {
		if (!instance) instance = new EventHandler();
		return instance;
	}

       ~EventHandler();
	void click(int x, int y);
	void sendShortcut(const QKeySequence& shortcut) const;
	void sendWord(const QString word) const;
	void sendKey(const QChar key) const;
	void setModifier(const int virtualKey, const bool once) const;
	void unsetModifier(const int virtualKey) const;
	void sendKeyCode(const int keycode) const;
	void runCommandKey(const int keycombination) const;
};

#endif
