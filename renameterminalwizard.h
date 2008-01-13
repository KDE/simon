//
// C++ Interface: renameterminalwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENAMETERMINALWIZARD_H
#define RENAMETERMINALWIZARD_H

#include "simonwizard.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class QWizardPage;
class WordListManager;
class GrammarManager;

class RenameTerminalWizard : public SimonWizard
{

private:
	QWizardPage *createIntroPage();
	QWizardPage *createSelectParametersPage(WordListManager *wordListManager);
	QWizardPage *createWorkingPage(WordListManager*, GrammarManager*);
	QWizardPage *createFinishedPage();
public:
    RenameTerminalWizard(QWidget* parent, WordListManager *wordListManager, 
		GrammarManager *grammarManager);

    ~RenameTerminalWizard();

};

#endif
