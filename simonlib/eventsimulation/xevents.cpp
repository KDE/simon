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


#include <Qt>
#include "xevents.h"
#include "xeventsprivate.h"


/**
 * @brief Constructor
 * 
 * Opens the display (member)
 *
 * @param char* displayName
 * The name of the display to open
 * 
 * @author Peter Grasch
 */
XEvents::XEvents(char* displayName) : CoreEvents()
{
	d = new XEventsPrivate(displayName);
}


void XEvents::click(int x, int y)
{
	d->click(x, y);
}


void XEvents::sendKeyPrivate(unsigned int key)
{
	d->sendKeyPrivate(key);
	unsetUnneededModifiers();
}


void XEvents::setModifierKey(int virtualKey, bool once)
{
	if ((!shiftSet) && (virtualKey & Qt::SHIFT))
	{
		d->setModifierKey(Qt::SHIFT);
		shiftSet=true;
		shiftOnce=once;
	}
	if ((!altgrSet) && (virtualKey & Qt::Key_AltGr))
	{
		d->setModifierKey(Qt::Key_AltGr);
		altgrSet=true;
		altgrOnce=once;
	}
	if ((!strgSet) && (virtualKey & Qt::CTRL))
	{
		d->setModifierKey(Qt::CTRL);
		strgSet=true;
		strgOnce=once;
	}
	if ((!altSet) && (virtualKey & Qt::ALT))
	{
		d->setModifierKey(Qt::ALT);
		altSet=true;
		altOnce=once;
	}
	if ((!superSet) && (virtualKey & Qt::META))
	{
		d->setModifierKey(Qt::META);
		superSet=true;
		superOnce=once;
	}
}


void XEvents::unsetModifier(int virtualKey)
{
	
	if (virtualKey & Qt::SHIFT)
	{
		d->unsetModifier(Qt::SHIFT);
		shiftSet=false;
	}
	if (virtualKey & Qt::Key_AltGr)
	{
		d->unsetModifier(Qt::Key_AltGr);
		altgrSet=false;
	}
	if (virtualKey & Qt::CTRL)
	{
		d->unsetModifier(Qt::CTRL);
		strgSet=false;
	}
	if (virtualKey & Qt::ALT)
	{
		d->unsetModifier(Qt::ALT);
		altSet=false;
	}
	if (virtualKey & Qt::META)
	{
		d->unsetModifier(Qt::META);
		superSet=false;
	}
}



/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
XEvents::~XEvents()
{
	delete d;
}


