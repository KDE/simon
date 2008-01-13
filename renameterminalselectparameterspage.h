//
// C++ Interface: renameterminalselectparameterspage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENAMETERMINALSELECTPARAMETERSPAGE_H
#define RENAMETERMINALSELECTPARAMETERSPAGE_H

#include <QWizardPage>
#include "ui_renameterminalselectparameterspage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class WordListManager;
class RenameTerminalSelectParametersPage : public QWizardPage
{
private:
	Ui::RenameTerminalSelectParametersDlg ui;
	WordListManager *wordListManager;
public:
    RenameTerminalSelectParametersPage(QWidget *parent, WordListManager *wordListManager);
    void initializePage();

    ~RenameTerminalSelectParametersPage();

};

#endif
