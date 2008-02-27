//
// C++ Implementation: atsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "atsettings.h"
#include "settings.h"
#include "atwatcher.h"
#include <QVariant>


ATSettings::ATSettings(QWidget* parent): SystemWidget(tr("AT-Einstellungen"), QIcon(":/images/icons/system-search.svg"), tr("Konfigurieren der AT Integration, die es erlaubt andere Programme einfach zu bedienen"), parent)
{
	ui.setupUi(this);
}


ATSettings::~ATSettings()
{
}


bool ATSettings::apply()
{
	Settings::set("GuiRecognition/SupportAT", ui.cbATIntegration->isChecked());
	Settings::set("GuiRecognition/QtWorkarounds", ui.cbQtWorkarounds->isChecked());
	Settings::set("GuiRecognition/QtMenuSupport", ui.cbQtMenu->isChecked());
	ATWatcher::getInstance()->applySettings();
	return true;
}

bool ATSettings::init()
{
	ui.cbATIntegration->setChecked(Settings::get("GuiRecognition/SupportAT").toBool());
	ui.cbQtWorkarounds->setChecked(Settings::get("GuiRecognition/QtWorkarounds").toBool());
	ui.cbQtMenu->setChecked(Settings::get("GuiRecognition/QtMenuSupport").toBool());
	return true;
}

bool ATSettings::isComplete()
{
	//nothing mandatory
	return true;
}

bool ATSettings::reset()
{
	return init();
}

