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
#include <simoninfo/simoninfo.h>
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



FilterCommandManager::FilterCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
	isActive(false),
	activateAction(new KAction(this))
{
	activateAction->setText(i18n("Activate Filter"));
	activateAction->setIcon(KIcon("view-filter"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(toggle()));

	guiActions << activateAction;
}

const QString FilterCommandManager::name() const
{
	return i18n("Filter");
}

const KIcon FilterCommandManager::icon() const
{
	return KIcon("view-filter");
}

void FilterCommandManager::updateAction()
{
	if (!isActive) {
		SimonInfo::showMessage(i18n("Filter deactivated"), 2500, new KIcon("view-filter"));
		activateAction->setText(i18n("Activate Filter"));
	} else {
		SimonInfo::showMessage(i18n("Filter activated"), 2500, new KIcon("view-filter"));
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

bool FilterCommandManager::deSerializeConfig(const QDomElement& elem, Scenario *parent)
{
	FilterConfiguration::getInstance(dynamic_cast<QWidget*>(QObject::parent()), QVariantList())->load();
	return true;
}


FilterCommandManager::~FilterCommandManager()
{
// 	FilterConfiguration::getInstance()->destroy();
}
