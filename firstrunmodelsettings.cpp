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

FirstRunModelSettings::FirstRunModelSettings(QWidget* parent): SystemWidgetPage(parent)
{
	setChild(new ModelSettings(this));
}


int FirstRunModelSettings::nextId() const
{
	if (field("newModel").toBool())
	{
		return FirstRunWizard::CreateDictionaryPage;
	} else 
		return FirstRunWizard::FinishedPage;
}


FirstRunModelSettings::~FirstRunModelSettings()
{ }


