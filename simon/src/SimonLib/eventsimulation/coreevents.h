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

#ifndef COREEVENTS_H
#define COREEVENTS_H

class QKeySequence;
/**
 *	@class CoreEvents
 *	@brief Abstract class defining the methods that a EventBackend has to support
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Peter Grasch
 */
class CoreEvents {
	
public:
	bool shiftSet, altgrSet, altSet, superSet, strgSet;
	bool shiftOnce, altgrOnce, altOnce, superOnce, strgOnce;
public:
	CoreEvents();
	
	virtual void sendChar(char key)=0;

	virtual void click(int x, int y)=0;
	virtual void sendKey(unsigned int key /*unicode representation*/)=0;
	
	void unsetUnneededModifiers();
	void sendShortcut(const QKeySequence& shortcut);

	virtual void setModifierKey(int virtualKey, bool once)=0;
	virtual void unsetModifier(int virtualKey)=0;
	virtual ~CoreEvents() {}
};

#endif
