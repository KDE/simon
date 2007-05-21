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
Dict::Dict()
{
	ipaToXsp = QHash<int, QString>();
	
	
	ipaToXsp.insert(0x00E6, "&");
	ipaToXsp.insert(0x00E7, "C");
	ipaToXsp.insert(0x00F0, "D");
	ipaToXsp.insert(0x0020, "2");
	ipaToXsp.insert(0x002E, " ");
	ipaToXsp.insert(0x002E, ".");
	ipaToXsp.insert(0x007C, "|");
	ipaToXsp.insert(0x0061, "a");
	ipaToXsp.insert(0x0062, "b");
	ipaToXsp.insert(0x0063, "c");
	ipaToXsp.insert(0x0064, "d");
	ipaToXsp.insert(0x0065, "e");
	ipaToXsp.insert(0x0066, "f");
	ipaToXsp.insert( 0x0068, "h");
	ipaToXsp.insert( 0x0069, "i");
	ipaToXsp.insert( 0x006A, "j");
	ipaToXsp.insert( 0x006B, "k");
	ipaToXsp.insert(0x006C, "l");
	ipaToXsp.insert(0x006D, "m");
	ipaToXsp.insert(0x006E, "n");
	ipaToXsp.insert(0x006F, "o");
	ipaToXsp.insert(0x0070, "p");
	ipaToXsp.insert(0x0071, "q");
	ipaToXsp.insert(0x0072, "r");
	ipaToXsp.insert(0x0073, "s");
	ipaToXsp.insert(0x0074, "t");
	ipaToXsp.insert(0x0075, "u");
	ipaToXsp.insert(0x0076, "v");
	ipaToXsp.insert(0x0077, "w");
	ipaToXsp.insert(0x0078, "x");
	ipaToXsp.insert(0x0079, "y");
	ipaToXsp.insert(0x007A, "z");
	ipaToXsp.insert(0x0127, "X");
	ipaToXsp.insert(0x014B, "N");
	ipaToXsp.insert(0x0153, "9");
	ipaToXsp.insert(0x0180, "B");
	ipaToXsp.insert(0x01A5, "p_<");
	ipaToXsp.insert(0x01AB, "t_j");
	ipaToXsp.insert(0x01AD, "t_<");
	ipaToXsp.insert(0x01BB, "dz)");
	ipaToXsp.insert(0x01C0, "|");
	ipaToXsp.insert(0x01C1, "||");
	ipaToXsp.insert(0x01C2, "=");
	ipaToXsp.insert(0x01C3, "!");
	ipaToXsp.insert(0x0250, "6");
	ipaToXsp.insert(0x0251, "A");
	ipaToXsp.insert(0x0252, "Q");
	ipaToXsp.insert(0x0253, "b_<");
	ipaToXsp.insert(0x0254, "O");
	ipaToXsp.insert(0x0255, "s");
	ipaToXsp.insert(0x0256, "d`");
	ipaToXsp.insert(0x0257, "d_<");
	ipaToXsp.insert(0x0258, "@");
	ipaToXsp.insert(0x0259, "@");
	ipaToXsp.insert(0x025A, "@`");
	ipaToXsp.insert(0x025B, "E");
	ipaToXsp.insert(0x025C, "3");
	ipaToXsp.insert(0x025D, "3`");
	ipaToXsp.insert(0x025E, "3");
	ipaToXsp.insert(0x025F, "J");
	ipaToXsp.insert(0x0260, "g_<");
	ipaToXsp.insert(0x0261, "g");
	ipaToXsp.insert(0x0262, "G");
	ipaToXsp.insert(0x0263, "G");
	ipaToXsp.insert(0x0264, "7");
	ipaToXsp.insert(0x0265, "H");
	ipaToXsp.insert(0x0266, "h");
	ipaToXsp.insert(0x0267, "x");
	ipaToXsp.insert(0x0268, "1");
	ipaToXsp.insert(0x0269, "I");
	ipaToXsp.insert(0x026A, "I");
	ipaToXsp.insert(0x026B, "5");
	ipaToXsp.insert(0x026C, "K");
	ipaToXsp.insert(0x026D, "l`");
	ipaToXsp.insert(0x026E, "K");
	ipaToXsp.insert(0x026F, "M");
	ipaToXsp.insert(0x0270, "M");
	ipaToXsp.insert(0x0271, "F");
	ipaToXsp.insert(0x0272, "J");
	ipaToXsp.insert(0x0273, "n`");
	ipaToXsp.insert(0x0274, "N");
	ipaToXsp.insert(0x0275, "8");
	ipaToXsp.insert(0x0276, "&");
	ipaToXsp.insert(0x0277, "U");
	ipaToXsp.insert(0x0278, "p");
	ipaToXsp.insert(0x0279, "r");
	ipaToXsp.insert(0x027A, "l");
	ipaToXsp.insert(0x027B, "r`");
	ipaToXsp.insert(0x027C, "r_r");
	ipaToXsp.insert(0x027D, "r`");
	ipaToXsp.insert(0x027E, "4");
	ipaToXsp.insert(0x027F, "z=");
	ipaToXsp.insert(0x0280, "R");
	ipaToXsp.insert(0x0281, "R");
	ipaToXsp.insert(0x0282, "s`");
	ipaToXsp.insert(0x0283, "S");
	ipaToXsp.insert(0x0284, "J_<");
	ipaToXsp.insert(0x0285, "z`=");
	ipaToXsp.insert(0x0286, "S_j");
	ipaToXsp.insert(0x0287, "|");
	ipaToXsp.insert(0x0288, "t`");
	ipaToXsp.insert(0x0289, "u");
	ipaToXsp.insert(0x028A, "U");
	ipaToXsp.insert(0x028B, "v");
	ipaToXsp.insert(0x028C, "V");
	ipaToXsp.insert(0x028D, "W");
	ipaToXsp.insert(0x028E, "L");
	ipaToXsp.insert(0x028F, "Y");
	ipaToXsp.insert(0x0290, "z`");
	ipaToXsp.insert(0x0291, "z");
	ipaToXsp.insert(0x0292, "Z");
	ipaToXsp.insert(0x0293, "Z_j");
	ipaToXsp.insert(0x0294, "?");
	ipaToXsp.insert(0x0295, "?");
	ipaToXsp.insert(0x0296, "||");
	ipaToXsp.insert(0x0297, "!");
	ipaToXsp.insert(0x0298, "O");
	ipaToXsp.insert(0x0299, "B");
	ipaToXsp.insert(0x029A, "&");
	ipaToXsp.insert(0x029B, "G_<");
	ipaToXsp.insert(0x029C, "H");
	ipaToXsp.insert(0x029D, "j");
	ipaToXsp.insert(0x029F, "L");
	ipaToXsp.insert(0x02A0, "q_<");
	ipaToXsp.insert(0x02A1, ">");
	ipaToXsp.insert(0x02A2, "<");
	ipaToXsp.insert(0x02A3, "dz)");
	ipaToXsp.insert(0x02A4, "dZ)");
	ipaToXsp.insert(0x02A5, "dz)");
	ipaToXsp.insert(0x02A6, "ts)");
	ipaToXsp.insert(0x02A7, "tS)");
	ipaToXsp.insert(0x02A8, "ts)");
	ipaToXsp.insert(0x02A9, "fN)");
	ipaToXsp.insert(0x02AA, "ls)");
	ipaToXsp.insert(0x02AB, "lz)");
	ipaToXsp.insert(0x02AC, "._w_w");
	ipaToXsp.insert(0x02AD, "._d_d");
	ipaToXsp.insert(0x02B0, "_h");
	ipaToXsp.insert(0x02B1, "_t");
	ipaToXsp.insert(0x02B2, "_j");
	ipaToXsp.insert(0x02B7, "_w");
	ipaToXsp.insert(0x02B8, "_j");
	ipaToXsp.insert(0x02BC, "_>");
	ipaToXsp.insert(0x02C0, "_>");
	ipaToXsp.insert(0x02C7, "_F_R");
	ipaToXsp.insert(0x02C8, "'");
	ipaToXsp.insert(0x02C9, "_T");
	ipaToXsp.insert(0x02CD, "_L");
	ipaToXsp.insert(0x02CE, "_L_B");
	ipaToXsp.insert(0x02CF, "_B_L");
	ipaToXsp.insert(0x02D0, ":");
	ipaToXsp.insert(0x02D1, ":");
	ipaToXsp.insert(0x02D2, "_O");
	ipaToXsp.insert(0x02D3, "_c");
	ipaToXsp.insert(0x02D4, "_r");
	ipaToXsp.insert(0x02D5, "_o");
	ipaToXsp.insert(0x02D6, "_+");
	ipaToXsp.insert(0x02D7, "_-");
	ipaToXsp.insert(0x02D8, "_X");
	ipaToXsp.insert(0x02DA, "_0");
	ipaToXsp.insert(0x02DC, "~");
	ipaToXsp.insert(0x02DD, "_T");
	ipaToXsp.insert(0x02DE, "`");
	ipaToXsp.insert(0x02E0, "_G");
	ipaToXsp.insert(0x02E1, "_l");
	ipaToXsp.insert(0x02E4, "_?");
	ipaToXsp.insert(0x02E5, "_T");
	ipaToXsp.insert(0x02E6, "_H");
	ipaToXsp.insert(0x02E7, "_M");
	ipaToXsp.insert(0x02E8, "_L");
	ipaToXsp.insert(0x02E9, "_B");
	ipaToXsp.insert(0x02EC, "_v");
	ipaToXsp.insert(0x0300, "_L");
	ipaToXsp.insert(0x0301, "_H");
	ipaToXsp.insert(0x0302, "_F");
	ipaToXsp.insert(0x0303, "~");
	ipaToXsp.insert(0x0304, "_M");
	ipaToXsp.insert(0x0306, "_X");
	ipaToXsp.insert(0x0308, "_\"");
	ipaToXsp.insert(0x030A, "_0");
	ipaToXsp.insert(0x030B, "_T");
	ipaToXsp.insert(0x030C, "_R");
	ipaToXsp.insert(0x030D, "=");
	ipaToXsp.insert(0x030F, "_B");
	ipaToXsp.insert(0x0318, "_A");
	ipaToXsp.insert(0x0319, "_q");
	ipaToXsp.insert(0x031A, "_}");
	ipaToXsp.insert(0x031C, "_c");
	ipaToXsp.insert(0x031D, "_r");
	ipaToXsp.insert(0x031E, "_o");
	ipaToXsp.insert(0x031F, "_+");
	ipaToXsp.insert(0x0320, "_-");
	ipaToXsp.insert(0x0321, "_j");
	ipaToXsp.insert(0x0322, "`");
	ipaToXsp.insert(0x0324, "_t");
	ipaToXsp.insert(0x0325, "_0");
	ipaToXsp.insert(0x0328, "~");
	ipaToXsp.insert(0x0329, "=");
	ipaToXsp.insert(0x032A, "_d");
	ipaToXsp.insert(0x032B, "_w");
	ipaToXsp.insert(0x032C, "_v");
	ipaToXsp.insert(0x032F, "_^");
	ipaToXsp.insert(0x0330, "_k");
	ipaToXsp.insert(0x0334, "_e");
	ipaToXsp.insert(0x0339, "_O");
	ipaToXsp.insert(0x033A, "_a");
	ipaToXsp.insert(0x033B, "_m");
	ipaToXsp.insert(0x033C, "_N");
	ipaToXsp.insert(0x033D, "_x");
	ipaToXsp.insert(0x0361, "_");
	ipaToXsp.insert(0x03B2, "B");
	ipaToXsp.insert(0x03B8, "T");
	ipaToXsp.insert(0x03C7, "X");
	ipaToXsp.insert(0x2016, "||");
	ipaToXsp.insert(0x203F, "-");
	ipaToXsp.insert(0x207F, "_n");
	ipaToXsp.insert(0x2191, "^");
	ipaToXsp.insert(0x2193, "!");
	ipaToXsp.insert(0x2197, "<R>");
	ipaToXsp.insert(0x2198, "<F>");
	
	
	//simon modifications
	ipaToXsp.insert('g', "g");
	ipaToXsp.insert('\'', "\"");
	ipaToXsp.insert('`', "\"");
	ipaToXsp.insert(0x2019, "\"");
	ipaToXsp.insert(0x02C8, "\"");
	
	ipaToXsp.insert(':', ":");
	ipaToXsp.insert('-', "-");
	ipaToXsp.insert(0x00E3, "e~");
	ipaToXsp.insert(0x00F5, "o~");
	ipaToXsp.insert(0x222B, "S");
	ipaToXsp.insert(0x02B6, "R");
	ipaToXsp.insert(0x02CC, "%");
	ipaToXsp.insert(0x01A6, "R\\");
	ipaToXsp.insert(0x00F8, "2");
	ipaToXsp.insert(0x00F6, "2");
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
		out += ipaToXsp.value(ipa.at(i).unicode());
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


