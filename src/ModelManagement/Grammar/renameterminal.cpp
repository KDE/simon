//
// C++ Implementation: renameterminal
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "renameterminal.h"
#include "wordlistmanager.h"
#include "grammarmanager.h"

RenameTerminal::RenameTerminal(QObject* parent): QThread(parent)
{}


RenameTerminal::~RenameTerminal()
{
}


void RenameTerminal::run()
{
	emit progress(0);

	WordListManager *wordListManager = WordListManager::getInstance();
	GrammarManager *grammarManager = GrammarManager::getInstance();
	wordListManager->renameTerminal(oldName, newName, includeShadow);
	wordListManager->save();
	emit progress(80);
	if (includeGrammar)
	{
		grammarManager->renameTerminal(oldName, newName);
		grammarManager->save();
	}

	emit progress(100);
	emit done();
}

