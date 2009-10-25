/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "action.h"
#include <KService>
#include <KPluginInfo>
#include <KDebug>
#include <simonscenarios/commandmanager.h>
#include <simonscenariobase/versionnumber.h>

Action::Action(Scenario *parent, const QString& source, const QString& trigger) : ScenarioObject(parent),
	m_source(source),
	pluginMinVersion(0),
	pluginMaxVersion(0)
{
	init(source, trigger);
}

/**
 * Deprecated constructor kept for compatibility reasons for now
 * FIXME: Remove it
 */

Action::Action(const QString& source, const QString& trigger) : ScenarioObject(NULL), m_source(source),
	pluginMinVersion(0),
	pluginMaxVersion(0)
{
	init(source, trigger);
}

Action* Action::createAction(Scenario *parent, const QDomElement& pluginElem)
{
	QString pluginSource = pluginElem.attribute("name");
	QString pluginTrigger = pluginElem.attribute("trigger");

	Action *a = new Action(parent, pluginSource, pluginTrigger);

	if (!a->deSerialize(pluginElem)) {
		delete a;
		a = NULL;
	}

	return a;
}

void Action::init(const QString& source, const QString& trigger)
{
	KService::Ptr service = KService::serviceByStorageId(source);
	if (!service) {
		kWarning() << "Service not found!";
		m_manager=NULL;
		return;
	}

	KPluginInfo pluginInfo(service);
	m_enabledByDefault = pluginInfo.isPluginEnabledByDefault();
	KPluginFactory *factory = KPluginLoader(service->library()).factory();
 
	m_trigger = trigger;

	m_version = pluginInfo.version();

	if (factory) {
		m_manager = factory->create<CommandManager>((QObject*)parentScenario);
		if (m_manager == NULL) {
			kWarning() << "Failed to create instance of " << source;
			return;
		}
		if (trigger.isNull()) {
			m_trigger = m_manager->preferredTrigger();
		}
	} else {
		kWarning() << "Factory not found!";
		m_manager = NULL;
	}
}

bool Action::deSerialize(const QDomElement& pluginElem)
{
	QDomElement pluginCompatibilityElem = pluginElem.firstChildElement("pluginCompatibility");
	QDomElement pluginMinVersionElem = pluginCompatibilityElem.firstChildElement();

	pluginMinVersion = VersionNumber::createVersionNumber(parentScenario, pluginMinVersionElem);
	pluginMaxVersion = VersionNumber::createVersionNumber(parentScenario, pluginMinVersionElem.nextSiblingElement());

	if (!pluginMinVersion) {
		kDebug() << "Couldn't parse version requirements of plugin";
		return false;
	} else {
		VersionNumber pluginCurVersion(parentScenario, getPluginVersion());
		if ((!pluginMinVersion->isValid()) || (pluginCurVersion < *pluginMinVersion) || 
			(pluginMaxVersion && pluginMaxVersion->isValid() && (!(pluginCurVersion <= *pluginMaxVersion)))) {
			kDebug() << "Scenario not compatible with this version of the plugin ";
			return false;
		}
	}
	
	if (!m_manager->deSerialize(pluginElem))
		return false;

	return true;
}

QDomElement Action::serialize(QDomDocument *doc)
{
	QDomElement pluginElem = m_manager->serialize(doc);

	pluginElem.setAttribute("name", m_source);
	pluginElem.setAttribute("trigger", m_trigger);
	QDomElement pluginCompatibilityElem = doc->createElement("pluginCompatibility");

	QDomElement minimumVersionElem = doc->createElement("minimumVersion");
	minimumVersionElem.appendChild(pluginMinVersion->serialize(doc));
	pluginCompatibilityElem.appendChild(minimumVersionElem);

	QDomElement maximumVersionElem = doc->createElement("maximumVersion");
	if (pluginMaxVersion)
		maximumVersionElem.appendChild(pluginMaxVersion->serialize(doc));
	pluginCompatibilityElem.appendChild(maximumVersionElem);

	pluginElem.appendChild(pluginCompatibilityElem);

	return pluginElem;
}

QIcon Action::icon()
{
	if (m_manager) 
		return m_manager->icon();

	return QIcon(); 
}

bool Action::hasCommands()
{
	if (!m_manager)
		return false;

	return m_manager->hasCommands();
}

bool Action::removeCommand(Command *command)
{
	return m_manager->deleteCommand(command);
}


Action::~Action()
{
	if (m_manager)
		delete m_manager;
}

