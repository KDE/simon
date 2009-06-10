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
	
	deadKeys.insert(373, new DeadKey('^', 'w'));
	deadKeys.insert(234, new DeadKey('^', 'e'));
	deadKeys.insert(7825, new DeadKey('^', 'z'));
	deadKeys.insert(251, new DeadKey('^', 'u'));
	deadKeys.insert(238, new DeadKey('^', 'i'));
	deadKeys.insert(244, new DeadKey('^', 'o'));
	deadKeys.insert(226, new DeadKey('^', 'a'));
	deadKeys.insert(349, new DeadKey('^', 's'));
	deadKeys.insert(285, new DeadKey('^', 'g'));
	deadKeys.insert(293, new DeadKey('^', 'h'));
	deadKeys.insert(309, new DeadKey('^', 'j'));
	deadKeys.insert(375, new DeadKey('^', 'y'));
	deadKeys.insert(265, new DeadKey('^', 'c'));
	
	
	deadKeys.insert(7811, new DeadKey(180, 'w'));
	deadKeys.insert(233, new DeadKey(180, 'e'));
	deadKeys.insert(341, new DeadKey(180, 'r'));
	deadKeys.insert(378, new DeadKey(180, 'z'));
	deadKeys.insert(250, new DeadKey(180, 'u'));
	deadKeys.insert(237, new DeadKey(180, 'i'));
	deadKeys.insert(243, new DeadKey(180, 'o'));
	deadKeys.insert(7765, new DeadKey(180, 'p'));
	deadKeys.insert(472, new DeadKey(180, 252 /*ü*/));
	deadKeys.insert(225, new DeadKey(180, 'a'));
	deadKeys.insert(347, new DeadKey(180, 's'));
	deadKeys.insert(501, new DeadKey(180, 'g'));
	deadKeys.insert(7729, new DeadKey(180, 'k'));
	deadKeys.insert(314, new DeadKey(180, 'l'));
	deadKeys.insert(253, new DeadKey(180, 'y'));
	deadKeys.insert(263, new DeadKey(180, 'c'));
	deadKeys.insert(324, new DeadKey(180, 'n'));
	deadKeys.insert(7743, new DeadKey(180, 'm'));
	
	deadKeys.insert(7809, new DeadKey('`', 'w'));
	deadKeys.insert(232, new DeadKey('`', 'e'));
	deadKeys.insert(249, new DeadKey('`', 'u'));
	deadKeys.insert(236, new DeadKey('`', 'i'));
	deadKeys.insert(242, new DeadKey('`', 'o'));
	deadKeys.insert(476, new DeadKey('`', 252 /*ü*/));
	deadKeys.insert(224, new DeadKey('`', 'a'));
	deadKeys.insert(7923, new DeadKey('`', 'y'));
	deadKeys.insert(505, new DeadKey('`', 'n'));
	
                                                
	deadKeys.insert(372, new DeadKey('^', 'W'));
	deadKeys.insert(202, new DeadKey('^', 'E'));
	deadKeys.insert(7824, new DeadKey('^', 'Z'));
	deadKeys.insert(219, new DeadKey('^', 'U'));
	deadKeys.insert(206, new DeadKey('^', 'I'));
	deadKeys.insert(212, new DeadKey('^', 'O'));
	deadKeys.insert(194, new DeadKey('^', 'A'));
	deadKeys.insert(348, new DeadKey('^', 'S'));
	deadKeys.insert(284, new DeadKey('^', 'G'));
	deadKeys.insert(292, new DeadKey('^', 'H'));
	deadKeys.insert(308, new DeadKey('^', 'J'));
	deadKeys.insert(374, new DeadKey('^', 'Y'));
	deadKeys.insert(264, new DeadKey('^', 'C'));
	
	
	deadKeys.insert(7810, new DeadKey(180, 'W'));
	deadKeys.insert(201, new DeadKey(180, 'E'));
	deadKeys.insert(340, new DeadKey(180, 'R'));
	deadKeys.insert(377, new DeadKey(180, 'Z'));
	deadKeys.insert(218, new DeadKey(180, 'U'));
	deadKeys.insert(205, new DeadKey(180, 'I'));
	deadKeys.insert(211, new DeadKey(180, 'O'));
	deadKeys.insert(7764, new DeadKey(180, 'P'));
	deadKeys.insert(471, new DeadKey(180, 220 /*Ü*/));
	deadKeys.insert(193, new DeadKey(180, 'A'));
	deadKeys.insert(346, new DeadKey(180, 'S'));
	deadKeys.insert(500, new DeadKey(180, 'G'));
	deadKeys.insert(7728, new DeadKey(180, 'K'));
	deadKeys.insert(313, new DeadKey(180, 'L'));
	deadKeys.insert(221, new DeadKey(180, 'Y'));
	deadKeys.insert(262, new DeadKey(180, 'C'));
	deadKeys.insert(323, new DeadKey(180, 'N'));
	deadKeys.insert(7742, new DeadKey(180, 'M'));
	
	deadKeys.insert(7808, new DeadKey('`', 'W'));
	deadKeys.insert(200, new DeadKey('`', 'E'));
	deadKeys.insert(217, new DeadKey('`', 'U'));
	deadKeys.insert(204, new DeadKey('`', 'I'));
	deadKeys.insert(210, new DeadKey('`', 'O'));
 	deadKeys.insert(475, new DeadKey('`', 220 /*ü*/));
	deadKeys.insert(192, new DeadKey('`', 'A'));
	deadKeys.insert(7922, new DeadKey('`', 'Y'));
	deadKeys.insert(504, new DeadKey('`', 'N'));
	
	
	
	deadKeys.insert(553, new DeadKey(184 /*¸*/, 'e'));
	deadKeys.insert(343, new DeadKey(184 /*¸*/, 'r'));
	deadKeys.insert(355, new DeadKey(184 /*¸*/, 't'));
	deadKeys.insert(351, new DeadKey(184 /*¸*/, 's'));
	deadKeys.insert(7697, new DeadKey(184 /*¸*/, 'd'));
	deadKeys.insert(291, new DeadKey(184 /*¸*/, 'g'));
	deadKeys.insert(7721, new DeadKey(184 /*¸*/, 'h'));
	deadKeys.insert(311, new DeadKey(184 /*¸*/, 'k'));
	deadKeys.insert(316, new DeadKey(184 /*¸*/, 'l'));
	deadKeys.insert(231, new DeadKey(184 /*¸*/, 'c'));
	deadKeys.insert(326, new DeadKey(184 /*¸*/, 'n'));


	deadKeys.insert(7869, new DeadKey('~', 'e'));
	deadKeys.insert(361, new DeadKey('~', 'u'));
	deadKeys.insert(297, new DeadKey('~', 'i'));
	deadKeys.insert(245, new DeadKey('~', 'o'));
	deadKeys.insert(227, new DeadKey('~', 'a'));
	deadKeys.insert(7929, new DeadKey('~', 'y'));
	deadKeys.insert(7805, new DeadKey('~', 'v'));
	deadKeys.insert(241, new DeadKey('~', 'n'));


	deadKeys.insert(552, new DeadKey(184 /*¸*/, 'E'));
	deadKeys.insert(342, new DeadKey(184 /*¸*/, 'R'));
	deadKeys.insert(354, new DeadKey(184 /*¸*/, 'T'));
	deadKeys.insert(350, new DeadKey(184 /*¸*/, 'S'));
	deadKeys.insert(7696, new DeadKey(184 /*¸*/, 'D'));
	deadKeys.insert(290, new DeadKey(184 /*¸*/, 'G'));
	deadKeys.insert(7720, new DeadKey(184 /*¸*/, 'H'));
	deadKeys.insert(310, new DeadKey(184 /*¸*/, 'K'));
	deadKeys.insert(315, new DeadKey(184 /*¸*/, 'L'));
	deadKeys.insert(199, new DeadKey(184 /*¸*/, 'C'));
	deadKeys.insert(325, new DeadKey(184 /*¸*/, 'N'));
	

	deadKeys.insert(7868, new DeadKey('~', 'E'));
	deadKeys.insert(360, new DeadKey('~', 'U'));
	deadKeys.insert(296, new DeadKey('~', 'I'));
	deadKeys.insert(213, new DeadKey('~', 'O'));
	deadKeys.insert(195, new DeadKey('~', 'A'));
	deadKeys.insert(7928, new DeadKey('~', 'Y'));
	deadKeys.insert(7804, new DeadKey('~', 'V'));
	deadKeys.insert(209, new DeadKey('~', 'N'));
}

void CoreEvents::sendKey(unsigned int key /*unicode representation*/)
{
	//handle dead keys
	DeadKey* d = deadKeys.value(key);
	
	if (d) {
		sendKeyPrivate(d->deadKey());
		key = d->baseKey();
	}
	
	sendKeyPrivate(key);
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
void CoreEvents::sendShortcut(const QKeySequence& shortcut)
{
	int key = shortcut[0] & ~(Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);

	//make key lowercase
	if ((key >= 65) && (key <= 90))
		key += 32;

		
	Qt::KeyboardModifiers mods = Qt::KeyboardModifiers(shortcut[0] & (Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT));
	if (key == Qt::Key_Backtab)
	{
		key = Qt::Key_Tab;
		mods |= Qt::ShiftModifier;
	}
	setModifierKey(mods, true);
	sendKey(key);
	unsetUnneededModifiers();
}

