//
// C++ Implementation: configuretriggerspage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configuretriggerspage.h"
#include "settings.h"

ConfigureTriggersPage::ConfigureTriggersPage(QWidget *parent)
 : QWizardPage(parent)
{
	setTitle("Triggerkonfiguration");
	ui.setupUi(this);
}

void ConfigureTriggersPage::initializePage()
{
	ui.leMainTrigger->setText(Settings::getS("Commands/Keyword"));
	ui.leDesktopgridTrigger->setText(Settings::getS("Desktopgrid/Trigger"));
}

bool ConfigureTriggersPage::isComplete() const
{
	return ((! ui.leMainTrigger->text().isEmpty()) && 
			(! ui.leDesktopgridTrigger->text().isEmpty()));
}


bool ConfigureTriggersPage::validatePage()
{
	Settings::set("Commands/Keyword", ui.leMainTrigger->text());
	Settings::set("Desktopgrid/Trigger", ui.leDesktopgridTrigger->text());
	return isComplete();
}

ConfigureTriggersPage::~ConfigureTriggersPage()
{
}


