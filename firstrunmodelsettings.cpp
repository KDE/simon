//
// C++ Implementation: firstrunmodelsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "firstrunmodelsettings.h"
#include <QVariant>
#include "firstrunwizard.h"
#include "modelsettings.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the systemwidget
 */
FirstRunModelSettings::FirstRunModelSettings(QWidget* parent): SystemWidgetPage(parent)
{
	setChild(new ModelSettings(this));
}


/**
 * \brief Returns the next id of the wizard
 * @return either FirstRunWizard::CreateDictionaryPage if the user selected to create a new model or FirstRunWizard::FinishedPage
 */
int FirstRunModelSettings::nextId() const
{
	if (field("newModel").toBool())
	{
		return FirstRunWizard::CreateDictionaryPage;
	} else 
		return FirstRunWizard::FinishedPage;
}
