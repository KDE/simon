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


#include "mergeterminals.h"
#include <KLocalizedString>

#ifdef SIMON_SCENARIOS
#include <speechmodelmanagement_scenario/wordlistmanager.h>
#include <speechmodelmanagement_scenario/grammarmanager.h>
#else
#include <speechmodelmanagement/wordlistmanager.h>
#include <speechmodelmanagement/grammarmanager.h>
#endif

MergeTerminals::MergeTerminals(QObject* parent): QThread(parent)
{
}

void MergeTerminals::run()
{
	emit status(i18n("Processing Words of Terminal %1", this->terminalA));
	emit progress(0,100);

	GrammarManager *grammarManager = GrammarManager::getInstance();
	WordListManager *wordListManager = WordListManager::getInstance();

	wordListManager->renameTerminal(terminalA, newName, includeShadow);

	emit status(i18n("Processing Words of Terminal %1", this->terminalB));
	emit progress(45,100);

	wordListManager->renameTerminal(terminalB, newName, includeShadow);

	if (includeGrammar)
	{
		emit status(i18n("Adapting Grammar"));
		emit progress(90,100);
		grammarManager->renameTerminal(terminalA, newName);
		emit progress(95,100);
		grammarManager->renameTerminal(terminalB, newName);
	}

	emit status(i18n("Finished"));
	emit progress(100,100);
	wordListManager->save();
	//grammarManager->save(); saved automagically
	emit done();
}


MergeTerminals::~MergeTerminals()
{
}


