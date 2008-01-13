//
// C++ Interface: configuretriggerspage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGURETRIGGERSPAGE_H
#define CONFIGURETRIGGERSPAGE_H

#include <QWizardPage>
#include "ui_configuretriggerspage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ConfigureTriggersPage : public QWizardPage
{
private:
	Ui::ConfigureTriggersPage ui;
public:
    ConfigureTriggersPage(QWidget *parent);

	void initializePage();
	bool isComplete() const;
	bool validatePage();

    ~ConfigureTriggersPage();

};

#endif
