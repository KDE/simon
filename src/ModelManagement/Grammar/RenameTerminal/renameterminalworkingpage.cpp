//
// C++ Implementation: renameterminalworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "renameterminalworkingpage.h"
#include "renameterminal.h"
#include "../grammarmanager.h"

RenameTerminalWorkingPage::RenameTerminalWorkingPage(QWidget *parent)
 : QWizardPage(parent)
{
	complete = false;
	renameTerminal = new RenameTerminal(this);
	connect(renameTerminal, SIGNAL(progress(int)), this, SLOT(displayProgress(int)));
	connect(renameTerminal, SIGNAL(done()), this, SLOT(finish()));
	ui.setupUi(this);
	setTitle(i18n("Benenne Terminal um..."));
}

void RenameTerminalWorkingPage::displayProgress(int progress)
{
	ui.pgProgress->setValue(progress);
}

void RenameTerminalWorkingPage::finish()
{
	complete = true;
	emit completeChanged();
	emit done();
}

void RenameTerminalWorkingPage::initializePage()
{
	//do the work
	renameTerminal->setOldName(field("renameTerminal").toString());

	renameTerminal->setNewName(field("renameNewName").toString());
	renameTerminal->setIncludeShadow((field("renameIncludeShadow").toBool()));
	renameTerminal->setIncludeGrammar(field("renameIncludeGrammar").toBool());
	renameTerminal->start();
}


RenameTerminalWorkingPage::~RenameTerminalWorkingPage()
{
}


