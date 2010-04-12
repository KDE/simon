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
	: shiftSet(false),
	altgrSet(false),
	altSet(false),
	superSet(false),
	strgSet(false),
	shiftOnce(false),
	altgrOnce(false),
	altOnce(false),
	superOnce(false),
	strgOnce(false)
{
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
	
	
	// ẉẹṛṭẓụịọạṣḍḥḳḷỵṿḅṇ
	deadKeys.insert(7817, new DeadKey(803 /*combining dot below*/, 'w'));
	deadKeys.insert(7865, new DeadKey(803 /*combining dot below*/, 'e'));
	deadKeys.insert(7771, new DeadKey(803 /*combining dot below*/, 'r'));
	deadKeys.insert(7789, new DeadKey(803 /*combining dot below*/, 't'));
	deadKeys.insert(7827, new DeadKey(803 /*combining dot below*/, 'z'));
	deadKeys.insert(7909, new DeadKey(803 /*combining dot below*/, 'u'));
	deadKeys.insert(7883, new DeadKey(803 /*combining dot below*/, 'i'));
	deadKeys.insert(7885, new DeadKey(803 /*combining dot below*/, 'o'));
	deadKeys.insert(7841, new DeadKey(803 /*combining dot below*/, 'a'));
	deadKeys.insert(7779, new DeadKey(803 /*combining dot below*/, 's'));
	deadKeys.insert(7693, new DeadKey(803 /*combining dot below*/, 'd'));
	deadKeys.insert(7717, new DeadKey(803 /*combining dot below*/, 'h'));
	deadKeys.insert(7731, new DeadKey(803 /*combining dot below*/, 'k'));
	deadKeys.insert(7735, new DeadKey(803 /*combining dot below*/, 'l'));
	deadKeys.insert(7925, new DeadKey(803 /*combining dot below*/, 'y'));
	deadKeys.insert(7807, new DeadKey(803 /*combining dot below*/, 'v'));
	deadKeys.insert(7685, new DeadKey(803 /*combining dot below*/, 'b'));
	deadKeys.insert(7751, new DeadKey(803 /*combining dot below*/, 'n'));




	// ẈẸṚṬẒỤỊỌẠṢḌḤḲḶỴṾḄṆṂ
	deadKeys.insert(7816, new DeadKey(803 /*combining dot below*/, 'W'));
	deadKeys.insert(7864, new DeadKey(803 /*combining dot below*/, 'E'));
	deadKeys.insert(7770, new DeadKey(803 /*combining dot below*/, 'R'));
	deadKeys.insert(7788, new DeadKey(803 /*combining dot below*/, 'T'));
	deadKeys.insert(7826, new DeadKey(803 /*combining dot below*/, 'Z'));
	deadKeys.insert(7908, new DeadKey(803 /*combining dot below*/, 'U'));
	deadKeys.insert(7882, new DeadKey(803 /*combining dot below*/, 'I'));
	deadKeys.insert(7884, new DeadKey(803 /*combining dot below*/, 'O'));
	deadKeys.insert(7840, new DeadKey(803 /*combining dot below*/, 'A'));
	deadKeys.insert(7778, new DeadKey(803 /*combining dot below*/, 'S'));
	deadKeys.insert(7692, new DeadKey(803 /*combining dot below*/, 'D'));
	deadKeys.insert(7716, new DeadKey(803 /*combining dot below*/, 'H'));
	deadKeys.insert(7730, new DeadKey(803 /*combining dot below*/, 'K'));
	deadKeys.insert(7734, new DeadKey(803 /*combining dot below*/, 'L'));
	deadKeys.insert(7924, new DeadKey(803 /*combining dot below*/, 'Y'));
	deadKeys.insert(7806, new DeadKey(803 /*combining dot below*/, 'V'));
	deadKeys.insert(7684, new DeadKey(803 /*combining dot below*/, 'B'));
	deadKeys.insert(7750, new DeadKey(803 /*combining dot below*/, 'N'));
	deadKeys.insert(7746, new DeadKey(803 /*combining dot below*/, 'M'));









	// ẇėṙṫżıȯṗȧṡḋḟġḣẏẋċḃṅṁ
	deadKeys.insert(7815, new DeadKey(775 /* dot above*/, 'w'));
	deadKeys.insert(279, new DeadKey(775 /* dot above*/, 'e')); 
	deadKeys.insert(7769, new DeadKey(775 /* dot above*/, 'r'));
	deadKeys.insert(7787, new DeadKey(775 /* dot above*/, 't'));
	deadKeys.insert(380, new DeadKey(775 /* dot above*/, 'z')); 
	deadKeys.insert(305, new DeadKey(775 /* dot above*/, 'i')); 
	deadKeys.insert(559, new DeadKey(775 /* dot above*/, 'o')); 
	deadKeys.insert(7767, new DeadKey(775 /* dot above*/, 'p'));
	deadKeys.insert(551, new DeadKey(775 /* dot above*/, 'a')); 
	deadKeys.insert(7777, new DeadKey(775 /* dot above*/, 's'));
	deadKeys.insert(7691, new DeadKey(775 /* dot above*/, 'd'));
	deadKeys.insert(7711, new DeadKey(775 /* dot above*/, 'f'));
	deadKeys.insert(289, new DeadKey(775 /* dot above*/, 'g')); 
	deadKeys.insert(7715, new DeadKey(775 /* dot above*/, 'h'));
	deadKeys.insert(7823, new DeadKey(775 /* dot above*/, 'y'));
	deadKeys.insert(7819, new DeadKey(775 /* dot above*/, 'x'));
	deadKeys.insert(267, new DeadKey(775 /* dot above*/, 'c')); 
	deadKeys.insert(7683, new DeadKey(775 /* dot above*/, 'b'));
	deadKeys.insert(7749, new DeadKey(775 /* dot above*/, 'n'));
	deadKeys.insert(7745, new DeadKey(775 /* dot above*/, 'm'));




	// ẆĖṘṪŻȮṖȦṠḊḞĠḢẎẊĊḂṄṀ
	deadKeys.insert(7814, new DeadKey(775 /* dot above*/, 'W'));
	deadKeys.insert(278, new DeadKey(775 /* dot above*/, 'E')); 
	deadKeys.insert(7768, new DeadKey(775 /* dot above*/, 'R'));
	deadKeys.insert(7786, new DeadKey(775 /* dot above*/, 'T'));
	deadKeys.insert(379, new DeadKey(775 /* dot above*/, 'Z')); 
	deadKeys.insert(558, new DeadKey(775 /* dot above*/, 'O')); 
	deadKeys.insert(7766, new DeadKey(775 /* dot above*/, 'P'));
	deadKeys.insert(550, new DeadKey(775 /* dot above*/, 'A')); 
	deadKeys.insert(7776, new DeadKey(775 /* dot above*/, 'S'));
	deadKeys.insert(7690, new DeadKey(775 /* dot above*/, 'D'));
	deadKeys.insert(7710, new DeadKey(775 /* dot above*/, 'F'));
	deadKeys.insert(288, new DeadKey(775 /* dot above*/, 'G')); 
	deadKeys.insert(7714, new DeadKey(775 /* dot above*/, 'H'));
	deadKeys.insert(7822, new DeadKey(775 /* dot above*/, 'Y'));
	deadKeys.insert(7818, new DeadKey(775 /* dot above*/, 'X'));
	deadKeys.insert(266, new DeadKey(775 /* dot above*/, 'C')); 
	deadKeys.insert(7682, new DeadKey(775 /* dot above*/, 'B'));
	deadKeys.insert(7748, new DeadKey(775 /* dot above*/, 'N'));
	deadKeys.insert(7744, new DeadKey(775 /* dot above*/, 'M'));


	

	deadKeys.insert(7813, new DeadKey(776 /* diaeresis above*/, 'w'));
	deadKeys.insert(235, new DeadKey(776 /* diaeresis above*/, 'e')); 
	deadKeys.insert(7831, new DeadKey(776 /* diaeresis above*/, 't'));
	deadKeys.insert(239, new DeadKey(776 /* diaeresis above*/, 'i')); 
	deadKeys.insert(7719, new DeadKey(776 /* diaeresis above*/, 'h'));
	deadKeys.insert(255, new DeadKey(776 /* diaeresis above*/, 'y')); 
	deadKeys.insert(7821, new DeadKey(776 /* diaeresis above*/, 'x'));




	deadKeys.insert(7812, new DeadKey(776 /* diaeresis above*/, 'W'));
	deadKeys.insert(203, new DeadKey(776 /* diaeresis above*/, 'E')); 
	deadKeys.insert(776, new DeadKey(776 /* diaeresis above*/, 'T')); 
	deadKeys.insert(207, new DeadKey(776 /* diaeresis above*/, 'I')); 
	deadKeys.insert(7718, new DeadKey(776 /* diaeresis above*/, 'H'));
	deadKeys.insert(376, new DeadKey(776 /* diaeresis above*/, 'Y')); 
	deadKeys.insert(7820, new DeadKey(776 /* diaeresis above*/, 'X'));






	


	// ẘůåẙ
	deadKeys.insert(7832, new DeadKey(778 /* ring above*/, 'w'));
	deadKeys.insert(367, new DeadKey(778 /* ring above*/, 'u')); 
	deadKeys.insert(229, new DeadKey(778 /* ring above*/, 'a')); 
	deadKeys.insert(7833, new DeadKey(778 /* ring above*/, 'y'));




	// W̊ŮÅY̊
	deadKeys.insert(778, new DeadKey(778 /* ring above*/, 'W'));
	deadKeys.insert(366, new DeadKey(778 /* ring above*/, 'U'));
	deadKeys.insert(197, new DeadKey(778 /* ring above*/, 'A'));
	deadKeys.insert(778, new DeadKey(778 /* ring above*/, 'Y'));









	// ēūīōǖāḡȫǟȳ
	deadKeys.insert(275, new DeadKey(772 /* macron */, 'e'));
	deadKeys.insert(363, new DeadKey(772 /* macron */, 'u'));
	deadKeys.insert(299, new DeadKey(772 /* macron */, 'i'));
	deadKeys.insert(333, new DeadKey(772 /* macron */, 'o'));
	deadKeys.insert(470, new DeadKey(772 /* macron */, 252 /*ü*/));
	deadKeys.insert(257, new DeadKey(772 /* macron */, 'a'));
	deadKeys.insert(7713, new DeadKey(772 /* macron */, 'g'));
	deadKeys.insert(555, new DeadKey(772 /* macron */, 246 /*ö*/)); 
	deadKeys.insert(479, new DeadKey(772 /* macron */, 231 /*ä*/)); 
	deadKeys.insert(563, new DeadKey(772 /* macron */, 'y')); 




	// ĒŪĪŌǕĀḠȪǞȲ
	deadKeys.insert(274, new DeadKey(772 /* macron */, 'E'));
	deadKeys.insert(362, new DeadKey(772 /* macron */, 'U'));
	deadKeys.insert(298, new DeadKey(772 /* macron */, 'I'));
	deadKeys.insert(332, new DeadKey(772 /* macron */, 'O'));
	deadKeys.insert(469, new DeadKey(772 /* macron */, 220 /*Ü*/));
	deadKeys.insert(256, new DeadKey(772 /* macron */, 'A'));
	deadKeys.insert(7712, new DeadKey(772 /* macron */, 'G'));
	deadKeys.insert(554, new DeadKey(772 /* macron */, 214 /*Ö*/)); 
	deadKeys.insert(478, new DeadKey(772 /* macron */, 196 /*Ä*/)); 
	deadKeys.insert(562, new DeadKey(772 /* macron */, 'Y')); 







	// ěřťžǔǐǒǚǎšďȟǰǩľčň
	deadKeys.insert(283, new DeadKey(780 /* caron */, 'e'));
	deadKeys.insert(345, new DeadKey(780 /* caron */, 'r'));
	deadKeys.insert(357, new DeadKey(780 /* caron */, 't'));
	deadKeys.insert(382, new DeadKey(780 /* caron */, 'z'));
	deadKeys.insert(468, new DeadKey(780 /* caron */, 'u'));
	deadKeys.insert(464, new DeadKey(780 /* caron */, 'i'));
	deadKeys.insert(466, new DeadKey(780 /* caron */, 'o'));
	deadKeys.insert(474, new DeadKey(780 /* caron */, 252 /*ü*/));
	deadKeys.insert(462, new DeadKey(780 /* caron */, 'a'));
	deadKeys.insert(353, new DeadKey(780 /* caron */, 's'));
	deadKeys.insert(271, new DeadKey(780 /* caron */, 'd'));
	deadKeys.insert(543, new DeadKey(780 /* caron */, 'h'));
	deadKeys.insert(496, new DeadKey(780 /* caron */, 'j'));
	deadKeys.insert(489, new DeadKey(780 /* caron */, 'k'));
	deadKeys.insert(318, new DeadKey(780 /* caron */, 'l'));
	deadKeys.insert(269, new DeadKey(780 /* caron */, 'c'));
	deadKeys.insert(328, new DeadKey(780 /* caron */, 'n'));




	// ĚŘŤŽǓǏǑǙǍŠĎȞJ̌ǨĽČŇ
	deadKeys.insert(282, new DeadKey(780 /* caron */, 'E'));
	deadKeys.insert(344, new DeadKey(780 /* caron */, 'R'));
	deadKeys.insert(356, new DeadKey(780 /* caron */, 'T'));
	deadKeys.insert(381, new DeadKey(780 /* caron */, 'Z'));
	deadKeys.insert(467, new DeadKey(780 /* caron */, 'U'));
	deadKeys.insert(463, new DeadKey(780 /* caron */, 'I'));
	deadKeys.insert(465, new DeadKey(780 /* caron */, 'O'));
	deadKeys.insert(473, new DeadKey(780 /* caron */, 220 /*Ü*/));
	deadKeys.insert(461, new DeadKey(780 /* caron */, 'A'));
	deadKeys.insert(352, new DeadKey(780 /* caron */, 'S'));
	deadKeys.insert(270, new DeadKey(780 /* caron */, 'D'));
	deadKeys.insert(542, new DeadKey(780 /* caron */, 'H'));
	deadKeys.insert(780, new DeadKey(780 /* caron */, 'J'));
	deadKeys.insert(488, new DeadKey(780 /* caron */, 'K'));
	deadKeys.insert(317, new DeadKey(780 /* caron */, 'L'));
	deadKeys.insert(268, new DeadKey(780 /* caron */, 'C'));
	deadKeys.insert(327, new DeadKey(780 /* caron */, 'N'));









	// űő
	deadKeys.insert(369, new DeadKey(779 /* double acute */, 'u'));
	deadKeys.insert(337, new DeadKey(779 /* double acute */, 'o'));




	// ŰŐ
	deadKeys.insert(368, new DeadKey(779 /* double acute */, 'U'));
	deadKeys.insert(336, new DeadKey(779 /* double acute */, 'O'));









	// ęųįǫą
	deadKeys.insert(281, new DeadKey(808 /* ogonek */, 'e'));
	deadKeys.insert(371, new DeadKey(808 /* ogonek */, 'u'));
	deadKeys.insert(303, new DeadKey(808 /* ogonek */, 'i'));
	deadKeys.insert(491, new DeadKey(808 /* ogonek */, 'o'));
	deadKeys.insert(261, new DeadKey(808 /* ogonek */, 'a'));




	// ĘŲĮǪĄ
	deadKeys.insert(280, new DeadKey(808 /* ogonek */, 'E'));
	deadKeys.insert(370, new DeadKey(808 /* ogonek */, 'U'));
	deadKeys.insert(302, new DeadKey(808 /* ogonek */, 'I'));
	deadKeys.insert(490, new DeadKey(808 /* ogonek */, 'O'));
	deadKeys.insert(260, new DeadKey(808 /* ogonek */, 'A'));
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
	int key = 0;
	int modifiers = 0;
	for(unsigned int i=0; i < shortcut.count(); i++) {
		key |= shortcut[i] & ~(Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);
		modifiers |= shortcut[i] & (Qt::SHIFT | Qt::META | Qt::CTRL | Qt::ALT);
	}

	//make key lowercase
	if ((key >= 65) && (key <= 90))
		key += 32;

		
	Qt::KeyboardModifiers mods = Qt::KeyboardModifiers(modifiers);
	if (key == Qt::Key_Backtab)
	{
		key = Qt::Key_Tab;
		mods |= Qt::ShiftModifier;
	}
	setModifierKey(mods, true);
	sendKey(key);
	unsetUnneededModifiers();
}

