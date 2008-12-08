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

#include "coreevents.h"
#include <QKeySequence>

CoreEvents::CoreEvents()
{
	strgSet=false;
	shiftSet=false;
	altgrSet=false;
	altSet=false;
	superSet=false;

	strgOnce=false;
	shiftOnce=false;
	altgrOnce=false;
	altOnce=false;
	superOnce=false;
}

void CoreEvents::unsetUnneededModifiers()
{
	if (shiftOnce) {
		shiftOnce=false;
		unsetModifier(Qt::SHIFT);
	}
	if (altgrOnce) {
		altgrOnce=false;
		unsetModifier(Qt::Key_AltGr);
	}
	if (strgOnce) {
		strgOnce=false;
		unsetModifier(Qt::CTRL);
	}
	if (altOnce) {
		altOnce=false;
		unsetModifier(Qt::ALT);
	}
	if (superOnce) {
		superOnce=false;
		unsetModifier(Qt::META);
	}
}




/**
 * \brief Sends the given Shortcut
 * 
 * This will set all the given modifiers and keys and then revert to the original
 * position (assuming the once flag is correctly interpreted)
 * 
 * \author Peter Grasch
 * @param shortcut The shortcut to send
 */
 #include <KMessageBox>
void CoreEvents::sendShortcut(const QKeySequence& shortcut)
{
	//KMessageBox::information(0, "Sending: "+shortcut.toString());
	int key = shortcut[0] & ~(Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);

	//make key lowercase
	if ((key >= 65) && (key <= 90))
		key += 32;
		
	
	//KMessageBox::information(0, "Key: "+QString::number(key));

	Qt::KeyboardModifiers mods = Qt::KeyboardModifiers(shortcut[0] & (Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT));
	//KMessageBox::information(0, "Modifiers: "+QString::number(mods));
	setModifierKey(mods, true);
	sendKey(key);
	unsetUnneededModifiers();
}
