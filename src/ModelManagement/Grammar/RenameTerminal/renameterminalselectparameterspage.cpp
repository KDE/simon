//
// C++ Implementation: renameterminalselectparameterspage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "renameterminalselectparameterspage.h"
#include "../../WordList/wordlistmanager.h"

RenameTerminalSelectParametersPage::RenameTerminalSelectParametersPage(QWidget *parent)
 : QWizardPage(parent)
{
	ui.setupUi(this);
	registerField("renameNewName*",ui.leNewName);
	registerField("renameTerminal*",ui.lwTerminal, "currentText", SIGNAL(currentRowChanged(int)));
	registerField("renameIncludeShadow", ui.cbIncludeShadow);
	registerField("renameIncludeGrammar", ui.cbIncludeGrammar);
}

void RenameTerminalSelectParametersPage::initializePage()
{
	QStringList availableTerminals;
	availableTerminals = WordListManager::getInstance()->getTerminals(true);
	ui.lwTerminal->clear();
	ui.lwTerminal->addItems(availableTerminals);
	
}

RenameTerminalSelectParametersPage::~RenameTerminalSelectParametersPage()
{
}


