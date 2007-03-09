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
	virtual void sendKey(int key)=0;
	virtual void setModifierKey(int virtualKey, bool once)=0;
	virtual void unsetModifier(int virtualKey)=0;
	
};

#endif
