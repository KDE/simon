//
// C++ Implementation: dict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dict.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 */
Dict::Dict(QObject *parent) : QObject(parent)
{
	phonemes = QHash<int, QString>();
	
	
	phonemes.insert(0x00E6, "&");
	phonemes.insert(0x00E7, "C");
	phonemes.insert(0x00F0, "D");
	phonemes.insert(0x002E, " ");
// 	phonemes.insert(0x002E, ".");
	phonemes.insert(0x0061, "a");
	phonemes.insert(0x0062, "b");
	phonemes.insert(0x0063, "c");
	phonemes.insert(0x0064, "d");
	phonemes.insert(0x0065, "e");
	phonemes.insert(0x0066, "f");
	phonemes.insert( 0x0068, "h");
	phonemes.insert( 0x0069, "i");
	phonemes.insert( 0x006A, "j");
	phonemes.insert( 0x006B, "k");
	phonemes.insert(0x006C, "l");
	phonemes.insert(0x006D, "m");
	phonemes.insert(0x006E, "n");
	phonemes.insert(0x006F, "o");
	phonemes.insert(0x0070, "p");
	phonemes.insert(0x0071, "q");
	phonemes.insert(0x0072, "r");
	phonemes.insert(0x0073, "s");
	phonemes.insert(0x0074, "t");
	phonemes.insert(0x0075, "u");
	phonemes.insert(0x0076, "v");
	phonemes.insert(0x0077, "w");
	phonemes.insert(0x0078, "x");
	phonemes.insert(0x0079, "y");
	phonemes.insert(0x007A, "z");
	phonemes.insert(0x0127, "X");
	phonemes.insert(0x014B, "N");
	phonemes.insert(0x0180, "B");
	phonemes.insert(0x01A5, "p_<");
	phonemes.insert(0x01AB, "t_j");
	phonemes.insert(0x01AD, "t_<");
	phonemes.insert(0x01BB, "dz)");
	phonemes.insert(0x01C3, "!");
	phonemes.insert(0x0250, "6");
	phonemes.insert(0x0251, "A");
	phonemes.insert(0x0252, "Q");
	phonemes.insert(0x0253, "b_<");
	phonemes.insert(0x0254, "O");
	phonemes.insert(0x0255, "s");
	phonemes.insert(0x0256, "d`");
	phonemes.insert(0x0257, "d_<");
	phonemes.insert(0x0258, "@");
	phonemes.insert(0x0259, "@");
	phonemes.insert(0x025A, "@`");
	phonemes.insert(0x025B, "E");
	phonemes.insert(0x025C, "3");
	phonemes.insert(0x025D, "3`");
	phonemes.insert(0x025E, "3");
	phonemes.insert(0x025F, "J");
	phonemes.insert(0x0260, "g_<");
	phonemes.insert(0x0261, "g");
	phonemes.insert(0x0262, "G");
	phonemes.insert(0x0263, "G");
	phonemes.insert(0x0264, "7");
	phonemes.insert(0x0265, "H");
	phonemes.insert(0x0266, "h");
	phonemes.insert(0x0267, "x");
	phonemes.insert(0x0268, "1");
	phonemes.insert(0x0269, "I");
	phonemes.insert(0x026A, "I");
	phonemes.insert(0x026B, "5");
	phonemes.insert(0x026C, "K");
	phonemes.insert(0x026D, "l`");
	phonemes.insert(0x026E, "K");
	phonemes.insert(0x026F, "M");
	phonemes.insert(0x0270, "M");
	phonemes.insert(0x0271, "F");
	phonemes.insert(0x0272, "J");
	phonemes.insert(0x0273, "n`");
	phonemes.insert(0x0274, "N");
	phonemes.insert(0x0275, "8");
	phonemes.insert(0x0276, "&");
	phonemes.insert(0x0277, "U");
	phonemes.insert(0x0278, "p");
	phonemes.insert(0x0279, "r");
	phonemes.insert(0x027A, "l");
	phonemes.insert(0x027B, "r`");
	phonemes.insert(0x027C, "r_r");
	phonemes.insert(0x027D, "r`");
	phonemes.insert(0x027E, "4");
	phonemes.insert(0x027F, "z=");
	phonemes.insert(0x0280, "R");
	phonemes.insert(0x0281, "R");
	phonemes.insert(0x0282, "s`");
	phonemes.insert(0x0283, "S");
	phonemes.insert(0x0284, "J_<");
	phonemes.insert(0x0285, "z`=");
	phonemes.insert(0x0286, "S_j");
	phonemes.insert(0x0288, "t`");
	phonemes.insert(0x0289, "u");
	phonemes.insert(0x028A, "U");
	phonemes.insert(0x028B, "v");
	phonemes.insert(0x028C, "V");
	phonemes.insert(0x028D, "W");
	phonemes.insert(0x028E, "L");
	phonemes.insert(0x028F, "Y");
	phonemes.insert(0x0290, "z`");
	phonemes.insert(0x0291, "z");
	phonemes.insert(0x0292, "Z");
	phonemes.insert(0x0293, "Z_j");
	phonemes.insert(0x0294, "?");
	phonemes.insert(0x0295, "?");
	phonemes.insert(0x0297, "!");
	phonemes.insert(0x0298, "O");
	phonemes.insert(0x0299, "B");
	phonemes.insert(0x029A, "&");
	phonemes.insert(0x029B, "G_<");
	phonemes.insert(0x029C, "H");
	phonemes.insert(0x029D, "j");
	phonemes.insert(0x029F, "L");
	phonemes.insert(0x02A0, "q_<");
	phonemes.insert(0x02A1, ">");
	phonemes.insert(0x02A2, "<");
	phonemes.insert(0x02A3, "dz)");
	phonemes.insert(0x02A4, "dZ)");
	phonemes.insert(0x02A5, "dz)");
	phonemes.insert(0x02A6, "ts)");
	phonemes.insert(0x02A7, "tS)");
	phonemes.insert(0x02A8, "ts)");
	phonemes.insert(0x02A9, "fN)");
	phonemes.insert(0x02AA, "ls)");
	phonemes.insert(0x02AB, "lz)");
	phonemes.insert(0x02AC, "._w_w");
	phonemes.insert(0x02AD, "._d_d");
	phonemes.insert(0x02B0, "_h");
	phonemes.insert(0x02B1, "_t");
	phonemes.insert(0x02B2, "_j");
	phonemes.insert(0x02B7, "_w");
	phonemes.insert(0x02B8, "_j");
	phonemes.insert(0x02C7, "_F_R");
	phonemes.insert(0x02C9, "_T");
	phonemes.insert(0x02CD, "_L");
	phonemes.insert(0x02CE, "_L_B");
	phonemes.insert(0x02CF, "_B_L");
	phonemes.insert(0x02D2, "_O");
	phonemes.insert(0x02D3, "_c");
	phonemes.insert(0x02D4, "_r");
	phonemes.insert(0x02D5, "_o");
	phonemes.insert(0x02D8, "_X");
	phonemes.insert(0x02DC, "~");
	phonemes.insert(0x02DD, "_T");
	phonemes.insert(0x02E0, "_G");
	phonemes.insert(0x02E1, "_l");
	phonemes.insert(0x02E4, "_?");
	phonemes.insert(0x02E5, "_T");
	phonemes.insert(0x02E6, "_H");
	phonemes.insert(0x02E7, "_M");
	phonemes.insert(0x02E8, "_L");
	phonemes.insert(0x02E9, "_B");
	phonemes.insert(0x02EC, "_v");
	phonemes.insert(0x0300, "_L");
	phonemes.insert(0x0301, "_H");
	phonemes.insert(0x0302, "_F");
	phonemes.insert(0x0303, "~");
	phonemes.insert(0x0304, "_M");
	phonemes.insert(0x0306, "_X");
	phonemes.insert(0x030B, "_T");
	phonemes.insert(0x030C, "_R");
	phonemes.insert(0x030F, "_B");
	phonemes.insert(0x0318, "_A");
	phonemes.insert(0x0319, "_q");
	phonemes.insert(0x031C, "_c");
	phonemes.insert(0x031D, "_r");
	phonemes.insert(0x031E, "_o");
	phonemes.insert(0x0321, "_j");
	phonemes.insert(0x0324, "_t");
	phonemes.insert(0x0328, "~");
	phonemes.insert(0x032A, "_d");
	phonemes.insert(0x032B, "_w");
	phonemes.insert(0x032C, "_v");
	phonemes.insert(0x0330, "_k");
	phonemes.insert(0x0334, "_e");
	phonemes.insert(0x0339, "_O");
	phonemes.insert(0x033A, "_a");
	phonemes.insert(0x033B, "_m");
	phonemes.insert(0x033C, "_N");
	phonemes.insert(0x033D, "_x");
	phonemes.insert(0x0361, "_");
	phonemes.insert(0x03B2, "B");
	phonemes.insert(0x03B8, "T");
	phonemes.insert(0x03C7, "X");
	//phonemes.insert(0x203F, "-");
	phonemes.insert(0x207F, "_n");
	phonemes.insert(0x2191, "^");
	phonemes.insert(0x2193, "!");
	phonemes.insert(0x0287, "|");
	phonemes.insert(0x007C, "|");
	phonemes.insert(0x01C0, "|");
	phonemes.insert(0x01C1, "||");
	phonemes.insert(0x0296, "||");
	phonemes.insert(0x2016, "||");
	phonemes.insert(0x2197, "<R>");
	phonemes.insert(0x2198, "<F>");
	
	
	//simon modifications
	phonemes.insert('g', "g");
// 	phonemes.insert('\'', "\\\"");
// 	phonemes.insert('`', "\\\"");
// 	phonemes.insert(0x2019, "\\\"");
// 	phonemes.insert(0x02C8, "\\\"");
	
	//phonemes.insert('-', "-");
	phonemes.insert(0x00E3, "e~");
	phonemes.insert(0x00F5, "o~");
	phonemes.insert(0x222B, "S");
	phonemes.insert(0x02B6, "R");
	phonemes.insert(0x02CC, "%");
// 	phonemes.insert(0x01A6, "R\\\\");
// 	phonemes.insert(0x00F8, "2");
// 	phonemes.insert(0x00F6, "2");
// 	phonemes.insert(0x0020, "2");
// 	phonemes.insert(0x0153, "9");
	
	
	
	
// 	modifiers.insert(0x0308, "_\\\"");
	modifiers.insert(0x02D0, ":");
	modifiers.insert(':', ":");
	modifiers.insert(0x02D1, ":");
	modifiers.insert(0x02D6, "_+");
	modifiers.insert(0x031F, "_+");
	modifiers.insert(0x02D7, "_-");
	modifiers.insert(0x0320, "_-");
	modifiers.insert(0x02DA, "_0");
	modifiers.insert(0x030A, "_0");
	modifiers.insert(0x0325, "_0");
	modifiers.insert(0x01C2, "=");
	modifiers.insert(0x030D, "=");
	modifiers.insert(0x0329, "=");
	modifiers.insert(0x02BC, "_>");
	modifiers.insert(0x02C0, "_>");
	modifiers.insert(0x032F, "_^");
	modifiers.insert(0x02DE, "`");
	modifiers.insert(0x031A, "_}");
	modifiers.insert(0x0322, "`");
// 	modifiers.insert(0x02C8, "'");
}

/**
 * \brief Converts a given IPA string to the corresponding XSP
 * 
 * This Function uses the translation table from http://www.theiling.de/ipa/#machine
 * Thanks!
 * 
 * \author Peter Grasch
 * \param QString ipa
 * The IPA string
 * \return QString
 * The X-Sampa String
 */
QString Dict::ipaToXSampa(QString ipa)
{
	QString out;
	for (int i=0; i < ipa.count(); i++)
	{
		if (phonemes.contains(ipa.at(i).unicode()))
			out += phonemes.value(ipa.at(i).unicode());
		else out += modifiers.value(ipa.at(i).unicode());
	}
	return out;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
Dict::~Dict()
{
}


