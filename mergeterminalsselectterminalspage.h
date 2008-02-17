
//
// C++ Interface: mergeterminalsselectterminalspage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MERGETERMINALSSELECTTERMINALSPAGE_H
#define MERGETERMINALSSELECTTERMINALSPAGE_H

#include <QWizardPage>

#include "ui_mergeterminalsselectpage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class MergeTerminalsSelectTerminalsPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::MergeTerminalsSelectTerminalsPage ui;
public:
    MergeTerminalsSelectTerminalsPage(QWidget* parent);

	void initializePage();

    ~MergeTerminalsSelectTerminalsPage();

};

#endif
