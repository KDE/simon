//
// C++ Interface: coreevents
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COREEVENTS_H
#define COREEVENTS_H

#include "shortcut.h"

/**
 *	@class CoreEvents
 *	@brief Abstract class defining the methods that a EventBackend has to support
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Peter Grasch
 */
class CoreEvents{
public:
	virtual void sendChar(char key)=0;
/*
	virtual void click(int x, int y)=0;
	virtual void sendKey(unsigned short key *unicode representation*)=0;
	virtual void sendShortcut(Shortcut shortcut)=0;
	virtual void setModifierKey(int virtualKey, bool once)=0;*/

// 	virtual void sendSpecial(int modifiers=0, int action=0, int fkeys=0, 
// 			int movement=0, char key='_')=0;
	virtual void unsetModifier(int virtualKey)=0;
	virtual ~CoreEvents() {}
};

#endif
