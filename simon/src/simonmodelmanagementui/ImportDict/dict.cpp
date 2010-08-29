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

#include "dict.h"
#include <KStandardDirs>
#include <QFile>

/**
 * \brief Constructor
 * \author Peter Grasch
 */
Dict::Dict(QObject *parent) : QObject(parent)
{
  buildAllowedPhonemes();
}


void Dict::buildTranslationTables()
{
  translationLookup.insert(0x00E6, "&");
  translationLookup.insert(0x00E7, "C");
  translationLookup.insert(0x00F0, "D");
  translationLookup.insert(0x002E, " ");
  // 	translationLookup.insert(0x002E, ".");
  translationLookup.insert(0x0061, "a");
  translationLookup.insert(0x0062, "b");
  translationLookup.insert(0x0063, "c");
  translationLookup.insert(0x0064, "d");
  translationLookup.insert(0x0065, "e");
  translationLookup.insert(0x0066, "f");
  translationLookup.insert( 0x0068, "h");
  translationLookup.insert( 0x0069, "i");
  translationLookup.insert( 0x006A, "j");
  translationLookup.insert( 0x006B, "k");
  translationLookup.insert(0x006C, "l");
  translationLookup.insert(0x006D, "m");
  translationLookup.insert(0x006E, "n");
  translationLookup.insert(0x006F, "o");
  translationLookup.insert(0x0070, "p");
  translationLookup.insert(0x0071, "q");
  translationLookup.insert(0x0072, "r");
  translationLookup.insert(0x0073, "s");
  translationLookup.insert(0x0074, "t");
  translationLookup.insert(0x0075, "u");
  translationLookup.insert(0x0076, "v");
  translationLookup.insert(0x0077, "w");
  translationLookup.insert(0x0078, "x");

  translationLookup.insert(0x03C7, "x");

  translationLookup.insert(0x0079, "y");
  translationLookup.insert(0x007A, "z");
  translationLookup.insert(0x0127, "X");
  translationLookup.insert(0x014B, "N");
  translationLookup.insert(0x0180, "B");
  translationLookup.insert(0x01A5, "p_<");
  translationLookup.insert(0x01AB, "t_j");
  translationLookup.insert(0x01AD, "t_<");
  translationLookup.insert(0x01BB, "dz)");
  translationLookup.insert(0x01C3, "!");
  translationLookup.insert(0x0250, "ah");
  translationLookup.insert(0x0251, "A");
  translationLookup.insert(0x0252, "Q");
  translationLookup.insert(0x0253, "b_<");
  translationLookup.insert(0x0254, "O");
  translationLookup.insert(0x0255, "s");
  translationLookup.insert(0x0256, "d`");
  translationLookup.insert(0x0257, "d_<");
  translationLookup.insert(0x0258, "@");
  translationLookup.insert(0x0259, "@");
  translationLookup.insert(0x025A, "@`");
  translationLookup.insert(0x025B, "E");
  translationLookup.insert(0x025C, "3");
  translationLookup.insert(0x025D, "3`");
  translationLookup.insert(0x025E, "3");
  translationLookup.insert(0x025F, "J");
  translationLookup.insert(0x0260, "g_<");
  translationLookup.insert(0x0261, "g");
  translationLookup.insert(0x0262, "G");
  translationLookup.insert(0x0263, "G");
  translationLookup.insert(0x0264, "7");
  translationLookup.insert(0x0265, "H");
  translationLookup.insert(0x0266, "h");
  translationLookup.insert(0x0267, "x");
  translationLookup.insert(0x0268, "1");
  translationLookup.insert(0x0269, "I");
  translationLookup.insert(0x026A, "I");
  translationLookup.insert(0x026B, "5");
  translationLookup.insert(0x026C, "K");
  translationLookup.insert(0x026D, "l`");
  translationLookup.insert(0x026E, "K");
  translationLookup.insert(0x026F, "M");
  translationLookup.insert(0x0270, "M");
  translationLookup.insert(0x0271, "F");
  translationLookup.insert(0x0272, "J");
  translationLookup.insert(0x0273, "n`");
  translationLookup.insert(0x0274, "N");
  translationLookup.insert(0x0275, "8");
  translationLookup.insert(0x0276, "&");
  translationLookup.insert(0x0277, "U");
  translationLookup.insert(0x0278, "p");
  translationLookup.insert(0x0279, "r");
  translationLookup.insert(0x027A, "l");
  translationLookup.insert(0x027B, "r`");
  translationLookup.insert(0x027C, "r_r");
  translationLookup.insert(0x027D, "r`");
  translationLookup.insert(0x027E, "4");
  translationLookup.insert(0x027F, "z=");
  translationLookup.insert(0x0280, "R");
  translationLookup.insert(0x0281, "R");
  translationLookup.insert(0x0282, "s`");
  translationLookup.insert(0x0283, "S");
  translationLookup.insert(0x0284, "J_<");
  translationLookup.insert(0x0285, "z`=");
  translationLookup.insert(0x0286, "S_j");
  translationLookup.insert(0x0288, "t`");
  translationLookup.insert(0x0289, "u");
  translationLookup.insert(0x028A, "U");
  translationLookup.insert(0x028B, "v");
  translationLookup.insert(0x028C, "V");
  translationLookup.insert(0x028D, "W");
  translationLookup.insert(0x028E, "L");
  translationLookup.insert(0x028F, "Y");
  translationLookup.insert(0x0290, "z`");
  translationLookup.insert(0x0291, "z");
  translationLookup.insert(0x0292, "Z");
  translationLookup.insert(0x0293, "Z_j");
  translationLookup.insert(0x0294, "?");
  translationLookup.insert(0x0295, "?");
  translationLookup.insert(0x0297, "!");
  translationLookup.insert(0x0298, "O");
  translationLookup.insert(0x0299, "B");
  translationLookup.insert(0x029A, "&");
  translationLookup.insert(0x029B, "G_<");
  translationLookup.insert(0x029C, "H");
  translationLookup.insert(0x029D, "j");
  translationLookup.insert(0x029F, "L");
  translationLookup.insert(0x02A0, "q_<");
  translationLookup.insert(0x02A1, ">");
  translationLookup.insert(0x02A2, "<");
  translationLookup.insert(0x02A3, "dz)");
  translationLookup.insert(0x02A4, "dZ)");
  translationLookup.insert(0x02A5, "dz)");
  translationLookup.insert(0x02A6, "ts)");
  translationLookup.insert(0x02A7, "tS)");
  translationLookup.insert(0x02A8, "ts)");
  translationLookup.insert(0x02A9, "fN)");
  translationLookup.insert(0x02AA, "ls)");
  translationLookup.insert(0x02AB, "lz)");
  translationLookup.insert(0x02AC, "._w_w");
  translationLookup.insert(0x02AD, "._d_d");
  translationLookup.insert(0x02B0, "_h");
  translationLookup.insert(0x02B1, "_t");
  translationLookup.insert(0x02B2, "_j");
  translationLookup.insert(0x02B7, "_w");
  translationLookup.insert(0x02B8, "_j");
  translationLookup.insert(0x02C7, "_F_R");
  translationLookup.insert(0x02C9, "_T");
  translationLookup.insert(0x02CD, "_L");
  translationLookup.insert(0x02CE, "_L_B");
  translationLookup.insert(0x02CF, "_B_L");
  translationLookup.insert(0x02D2, "_O");
  translationLookup.insert(0x02D3, "_c");
  translationLookup.insert(0x02D4, "_r");
  translationLookup.insert(0x02D5, "_o");
  translationLookup.insert(0x02D8, "_X");
  translationLookup.insert(0x02DC, "~");
  translationLookup.insert(0x02DD, "_T");
  translationLookup.insert(0x02E0, "_G");
  translationLookup.insert(0x02E1, "_l");
  translationLookup.insert(0x02E4, "_?");
  translationLookup.insert(0x02E5, "_T");
  translationLookup.insert(0x02E6, "_H");
  translationLookup.insert(0x02E7, "_M");
  translationLookup.insert(0x02E8, "_L");
  translationLookup.insert(0x02E9, "_B");
  translationLookup.insert(0x02EC, "_v");
  translationLookup.insert(0x0300, "_L");
  translationLookup.insert(0x0301, "_H");
  translationLookup.insert(0x0302, "_F");
  translationLookup.insert(0x0303, "~");
  translationLookup.insert(0x0304, "_M");
  translationLookup.insert(0x0306, "_X");
  translationLookup.insert(0x030B, "_T");
  translationLookup.insert(0x030C, "_R");
  translationLookup.insert(0x030F, "_B");
  translationLookup.insert(0x0318, "_A");
  translationLookup.insert(0x0319, "_q");
  translationLookup.insert(0x031C, "_c");
  translationLookup.insert(0x031D, "_r");
  translationLookup.insert(0x031E, "_o");
  translationLookup.insert(0x0321, "_j");
  translationLookup.insert(0x0324, "_t");
  translationLookup.insert(0x0328, "~");
  translationLookup.insert(0x032A, "_d");
  translationLookup.insert(0x032B, "_w");
  translationLookup.insert(0x032C, "_v");
  translationLookup.insert(0x0330, "_k");
  translationLookup.insert(0x0334, "_e");
  translationLookup.insert(0x0339, "_O");
  translationLookup.insert(0x033A, "_a");
  translationLookup.insert(0x033B, "_m");
  translationLookup.insert(0x033C, "_N");
  translationLookup.insert(0x033D, "_x");
  translationLookup.insert(0x0361, "_");
  translationLookup.insert(0x03B2, "B");
  translationLookup.insert(0x03B8, "T");
  translationLookup.insert(0x03C7, "X");
  //translationLookup.insert(0x203F, "-");
  translationLookup.insert(0x207F, "_n");
  translationLookup.insert(0x2191, "^");
  translationLookup.insert(0x2193, "!");
  translationLookup.insert(0x0287, "|");
  translationLookup.insert(0x007C, "|");
  translationLookup.insert(0x01C0, "|");
  translationLookup.insert(0x01C1, "||");
  translationLookup.insert(0x0296, "||");
  translationLookup.insert(0x2016, "||");
  translationLookup.insert(0x2197, "<R>");
  translationLookup.insert(0x2198, "<F>");

  //simon modifications
  translationLookup.insert('g', "g");
  // 	translationLookup.insert('\'', "\\\"");
  // 	translationLookup.insert('`', "\\\"");
  // 	translationLookup.insert(0x2019, "\\\"");
  // 	translationLookup.insert(0x02C8, "\\\"");

  //translationLookup.insert('-', "-");
  translationLookup.insert(0x00E3, "e~");
  translationLookup.insert(0x00F5, "o~");
  translationLookup.insert(0x222B, "S");
  translationLookup.insert(0x02B6, "R");
  translationLookup.insert(0x02CC, "%");
  // 	translationLookup.insert(0x01A6, "R\\\\");
  translationLookup.insert(0x00F8, "oeh");
  translationLookup.insert(0x00F6, "oeh");
  translationLookup.insert(0x0020, "oeh");
  translationLookup.insert(0x0153, "oe");

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


void Dict::buildAllowedPhonemes()
{
  QFile phon(KStandardDirs::locate("appdata", "model/phonemes"));
  if (!phon.open(QIODevice::ReadOnly))
    return;

  QStringList phonemes;
  //sorting per phoneme length...
  while (!phon.atEnd()) {
    phonemes << phon.readLine(50).trimmed();      //read phonemes linewise
  }
  phon.close();

  int max =0;
  //find max
  for (int i=0; i < phonemes.count(); i++) {
    max = qMax(phonemes[i].count(), max);
  }
  int index;
  while (max > 0) {
    index=0;
    while (index < phonemes.count()) {
      if (phonemes[index].count() == max)
        allowedPhonemes << phonemes.takeAt(index);
      else index++;
    }
    --max;
  }
}


QString Dict::adaptToSimonPhonemeSet(QString sampa)
{
  QString out = sampa.replace('6', "ah");
  out = out.replace('2', "oeh");
  //out = out.remove('~');
  //out = out.remove('<');
  out = out.replace('~', "nas");
  out = out.replace('<', "nsb");
  out = out.remove('_');
  out = out.remove('^');
  //	out = out.remove('?');
  out = out.replace('?', "gls");
  out = out.replace('9', "oe");
  return out;
}


QString Dict::segmentSampa(const QString& sampa)
{
  QString filteredXsp = sampa;
  //filter xsp through 'sampa sieve'
  int phonemeIndex=0;
  QString currentPhoneme = "";
  QString xspFertig = "";

  while ((!filteredXsp.isEmpty()) && (allowedPhonemes.count() > phonemeIndex)) {
    currentPhoneme = allowedPhonemes[phonemeIndex++];
    if (filteredXsp.indexOf(currentPhoneme)==0) {
      xspFertig += ' '+currentPhoneme;
                                                  //remove phoneme at start
      filteredXsp.remove(0, currentPhoneme.count());
      phonemeIndex=0;
    }
  }

  if (filteredXsp.isEmpty()) {                    //found everything
    return xspFertig.trimmed();
  } else
  return sampa;
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
  for (int i=0; i < ipa.count(); i++) {
    if (translationLookup.contains(ipa.at(i).unicode()))
      out += translationLookup.value(ipa.at(i).unicode());
    else out += modifiers.value(ipa.at(i).unicode());
  }
  return segmentSampa(adaptToSimonPhonemeSet(out.trimmed()));
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
Dict::~Dict()
{
  allowedPhonemes.clear();

  words.clear();
  pronunciations.clear();
  terminals.clear();
}
