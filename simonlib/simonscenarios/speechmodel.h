/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SPEECHMODEL_H_85D1B1451A8C4485957368A4AEF19F9B
#define SIMON_SPEECHMODEL_H_85D1B1451A8C4485957368A4AEF19F9B

namespace SpeechModel {
enum ModelElements {
	ShadowVocabulary=1,
	ScenarioVocabulary=2,
	ScenarioGrammar=4,
	ScenarioActions=8,
	AllScenariosVocabulary=16,
	AllScenariosGrammar=32,
	AllScenariosActions=64
};
}



#endif

