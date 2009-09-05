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
#include <commandpluginbase/commandmanager.h>
#include <simonscenariobase/versionnumber.h>

#ifdef SIMON_SCENARIOS
Action::Action(const QString& scenarioId, const QString& source, const QString& trigger) : ScenarioObject(scenarioId),
	m_source(source),
	pluginMinVersion(0),
	pluginMaxVersion(0)
{
	init(source, trigger);
}
#endif

/**
 * Deprecated constructor kept for compatibility reasons for now
 * FIXME: Remove it
 */

#ifdef SIMON_SCENARIOS
Action::Action(const QString& source, const QString& trigger) : ScenarioObject(""), m_source(source),
#else
Action::Action(const QString& source, const QString& trigger): m_source(source),
#endif
	pluginMinVersion(0),
	pluginMaxVersion(0)
{
	init(source, trigger);
}

#ifdef SIMON_SCENARIOS
Action* Action::createAction(const QString& scenarioId, const QDomElement& pluginElem)
{
	QString pluginSource = pluginElem.attribute("name");
	QString pluginTrigger = pluginElem.attribute("trigger");

	Action *a = new Action(scenarioId, pluginSource, pluginTrigger);

	if (!a->deSerialize(pluginElem)) {
		delete a;
		a = NULL;
	}

	return a;
}
#endif

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
		m_manager = factory->create<CommandManager>();
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

#ifdef SIMON_SCENARIOS
bool Action::deSerialize(const QDomElement& pluginElem)
{
	QDomElement pluginCompatibilityElem = pluginElem.firstChildElement("pluginCompatibility");
	QDomElement pluginMinVersionElem = pluginCompatibilityElem.firstChildElement();

	pluginMinVersion = VersionNumber::createVersionNumber("scenarioId", pluginMinVersionElem);
	pluginMaxVersion = VersionNumber::createVersionNumber("scenarioId", pluginMinVersionElem.nextSiblingElement());

	if (!pluginMinVersion) {
		kDebug() << "Couldn't parse version requirements of plugin";
		return false;
	} else {
		VersionNumber pluginCurVersion("scenarioId", getPluginVersion());
		if ((!pluginMinVersion->isValid()) || (pluginCurVersion < *pluginMinVersion) || 
			(pluginMaxVersion && pluginMaxVersion->isValid() && (!(pluginCurVersion <= *pluginMaxVersion)))) {
			kDebug() << "Scenario not compatible with this version of the plugin ";
			return false;
		}
	}
	
	QDomElement configElem = pluginElem.firstChildElement("config");
	if (!m_manager->deSerializeConfig(configElem, "scenarioId")) {
		kDebug() << "Couldn't load config of plugin";
		return false;
	}
	QDomElement commandsElem = pluginElem.firstChildElement("commands");
	if (!m_manager->deSerializeCommands(commandsElem, "scenarioId")) {
		kDebug() << "Couldn't load commands of plugin";
		return false;
	}

	return true;
}
#endif

#ifdef SIMON_SCENARIOS
QDomElement Action::serialize(QDomDocument *doc)
{
	QDomElement pluginElem = doc->createElement("plugin");
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

	//TODO: config elem is empty;
	pluginElem.appendChild(m_manager->serializeConfig(doc, "scenarioId"));

	//TODO: command elem is empty
	pluginElem.appendChild(m_manager->serializeCommands(doc, "scenarioId"));

	return pluginElem;
}
#endif

QIcon Action::icon()
{
	if (m_manager) 
		return m_manager->icon();

	return QIcon(); 
}


Action::~Action()
{
	if (m_manager)
		delete m_manager;
}

