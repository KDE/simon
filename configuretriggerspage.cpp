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

/**
 * \brief Constructor - sets up the gui
 * \author Peter Grasch
 * @param parent Set the parent to the given widget
 */
ConfigureTriggersPage::ConfigureTriggersPage(QWidget *parent)
 : QWizardPage(parent)
{
	setTitle("Triggerkonfiguration");
	ui.setupUi(this);

	connect(ui.leMainTrigger, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
	connect(ui.leDesktopgridTrigger, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
}

/**
 * \brief Initializes the page with the settings of the keyword and desktopgrid-trigger
 * \author Peter Grasch
 */
void ConfigureTriggersPage::initializePage()
{
	ui.leMainTrigger->setText(Settings::getS("Commands/Keyword"));
	ui.leDesktopgridTrigger->setText(Settings::getS("Desktopgrid/Trigger"));
}

/**
 * \brief Returns true if both values are set
 * @return true if both triggers are valid
 */
bool ConfigureTriggersPage::isComplete() const
{
	return ((! ui.leMainTrigger->text().isEmpty()) && 
			(! ui.leDesktopgridTrigger->text().isEmpty()));
}


/**
 * \brief Writes the triggers to the config file
 * \author Peter Grasch
 * @return isComplete()
 */
bool ConfigureTriggersPage::validatePage()
{
	Settings::set("Commands/Keyword", ui.leMainTrigger->text());
	Settings::set("Desktopgrid/Trigger", ui.leDesktopgridTrigger->text());
	return isComplete();
}
