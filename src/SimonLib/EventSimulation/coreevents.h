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

class QKeySequence;
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
