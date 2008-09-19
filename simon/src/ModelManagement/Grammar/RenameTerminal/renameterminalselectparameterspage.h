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
class RenameTerminalSelectParametersPage : public QWizardPage
{
private:
	Ui::RenameTerminalSelectParametersDlg ui;
public:
    RenameTerminalSelectParametersPage(QWidget *parent);
    void initializePage();

    ~RenameTerminalSelectParametersPage();

};

#endif
