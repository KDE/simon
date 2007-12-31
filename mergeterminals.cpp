//
// C++ Implementation: mergeterminals
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mergeterminals.h"
#include "wordlistmanager.h"
#include "grammarmanager.h"

MergeTerminals::MergeTerminals(WordListManager *wordListManager, GrammarManager *grammarManager, QObject* parent): QThread(parent)
{
	this->wordListManager = wordListManager;
	this->grammarManager = grammarManager;
}

void MergeTerminals::run()
{
	emit status(tr("Bearbeite Wörter des Terminals %1").arg(this->terminalA));
	emit progress(0,100);

	wordListManager->renameTerminal(terminalA, newName, includeShadow);

	emit status(tr("Bearbeite Wörter des Terminals %1").arg(this->terminalB));
	emit progress(45,100);


	wordListManager->renameTerminal(terminalB, newName, includeShadow);

	if (includeGrammar)
	{
		emit status(tr("Passe Grammatik an"));
		emit progress(90,100);
		grammarManager->renameTerminal(terminalA, newName);
		emit progress(95,100);
		grammarManager->renameTerminal(terminalB, newName);
	}

	emit status(tr("Fertig"));
	emit progress(100,100);
	wordListManager->save();
	grammarManager->save();
	emit done();
}


MergeTerminals::~MergeTerminals()
{
}


