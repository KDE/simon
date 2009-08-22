/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "filtercommandmanager.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KGenericFactory>
#include <KAction>
#include <KActionCollection>
#include "filterconfiguration.h"

K_PLUGIN_FACTORY( FilterPluginFactory, 
			registerPlugin< FilterCommandManager >(); 
			registerPlugin< FilterConfiguration >(); 
		)
        
K_EXPORT_PLUGIN( FilterPluginFactory("simonfiltercommand") )



FilterCommandManager::FilterCommandManager(QObject *parent, const QVariantList& args) : CommandManager(parent, args)
{
	isActive = false;

	setXMLFile("simonfilterpluginui.rc");
	activateAction = new KAction(this);
	activateAction->setText(i18n("Activate Filter"));
	activateAction->setIcon(KIcon("view-filter"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(toggle()));
	actionCollection()->addAction("simonfilterplugin", activateAction);
}

const QString FilterCommandManager::name() const
{
	return i18n("Filter");
}


void FilterCommandManager::updateAction()
{
	if (!isActive) {
		activateAction->setText(i18n("Activate Filter"));
	} else {
		activateAction->setText(i18n("Deactivate Filter"));
	}
}

void FilterCommandManager::toggle()
{
	Logger::log(i18n("[INF] Changing filter state..."));

	isActive = !isActive;
	updateAction();
}

CommandConfiguration* FilterCommandManager::getConfigurationPage()
{
	return FilterConfiguration::getInstance();
}

bool FilterCommandManager::trigger(const QString& triggerName)
{
	if (isActive) {
		if (triggerName == FilterConfiguration::getInstance()->deactivateTrigger()) {
			//make inactive
			toggle();
		}
		return true;
	} else if (triggerName == FilterConfiguration::getInstance()->activateTrigger()) {
		//make active
		toggle();
		return true;
	}

	//not for us
	return false;
}

bool FilterCommandManager::load()
{
	FilterConfiguration::getInstance(dynamic_cast<QWidget*>(parent()), QVariantList())->load();
	return true;
}

bool FilterCommandManager::save()
{
	return true;
}

FilterCommandManager::~FilterCommandManager()
{
	activateAction->deleteLater();
// 	FilterConfiguration::getInstance()->destroy();
}
