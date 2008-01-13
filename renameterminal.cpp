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

RenameTerminal::RenameTerminal(QObject* parent, WordListManager* wordListManager, 
	GrammarManager* grammarManager): QThread(parent)
{
	this->wordListManager = wordListManager;
	this->grammarManager = grammarManager;
}


RenameTerminal::~RenameTerminal()
{
}


void RenameTerminal::run()
{
	emit progress(0);

	this->wordListManager->renameTerminal(oldName, newName, includeShadow);
	this->wordListManager->save();
	emit progress(80);
	if (includeGrammar)
	{
		this->grammarManager->renameTerminal(oldName, newName);
		this->grammarManager->save();
	}

	emit progress(100);
	emit done();
}

