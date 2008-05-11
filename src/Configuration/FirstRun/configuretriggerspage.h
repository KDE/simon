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
 \class ConfigureTriggersPage
 \brief Configures the triggers of the commands and the desktopgrid
 \date 13.1.2008
 \version 0.1
 \author Peter Grasch
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

};

#endif
