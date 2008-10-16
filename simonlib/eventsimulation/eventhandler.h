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


#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "eventsimulation_export.h"

class CoreEvents;
class QChar;
class QString;
class QKeySequence;
/**
 *	@class EventHandler
 *	@brief Manages the underlying CoreEventhandlers
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Peter Grasch
 */
class EVENTSIMULATION_EXPORT EventHandler {
private:
	static EventHandler* instance;
	CoreEvents* coreEvents; //!< The event backend
protected:
	EventHandler();
	void sendKey(const QChar& key) const;

public:
	static EventHandler* getInstance() {
		if (!instance) instance = new EventHandler();
		return instance;
	}

       ~EventHandler();

	void click(int x, int y);
	void sendWord(const QString& word) const;
	void sendShortcut(const QKeySequence& shortcut) const;
};

#endif