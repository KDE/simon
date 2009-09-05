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

#include "shortcutcommandmanager.h"
#include <simonlogging/logger.h>
#include "xmlshortcutcommand.h"
#include "shortcutcommand.h"
#include "createshortcutcommandwidget.h"
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ShortcutCommandPluginFactory, 
			registerPlugin< ShortcutCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( ShortcutCommandPluginFactory("simonshortcutcommand") )


ShortcutCommandManager::ShortcutCommandManager(QObject* parent, const QVariantList& args) : CommandManager(parent, args),
#ifndef SIMON_SCENARIOS
	xmlShortcutCommand(new XMLShortcutCommand())
#endif
{
}

const QString ShortcutCommandManager::name() const
{
	return ShortcutCommand::staticCategoryText();
}


bool ShortcutCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<const ShortcutCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

CreateCommandWidget* ShortcutCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateShortcutCommandWidget(parent);
}

const KIcon ShortcutCommandManager::icon() const
{
	return ShortcutCommand::staticCategoryIcon();
}

bool ShortcutCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/shortcuts.xml");
	Logger::log(i18n("[INF] Loading shortcuts from %1", commandPath));

	bool ok = false;
#ifndef SIMON_SCENARIOS
	this->commands = xmlShortcutCommand->load(ok, commandPath);
	return ok;
#else
	return true;
#endif
}

bool ShortcutCommandManager::deSerializeCommands(const QDomElement& elem, const QString& scenarioId)
{
	Q_UNUSED(scenarioId);

	if (commands)
		qDeleteAll(*commands);
	commands = new CommandList();

	if (elem.isNull()) return false;

	QDomElement commandElem = elem.firstChildElement();
	while(!commandElem.isNull())
	{
		QDomElement name = commandElem.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement shortcut = icon.nextSiblingElement();
		commands->append(new ShortcutCommand(name.text(), icon.text(), 
						shortcut.text()));
		commandElem = commandElem.nextSiblingElement();
	}

	return true;
}



bool ShortcutCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/shortcuts.xml");
	Logger::log(i18n("[INF] Saving Shortcuts to %1", commandPath));
#ifndef SIMON_SCENARIOS
	return xmlShortcutCommand->save(commands, commandPath);
#else
	return true;
#endif
}


ShortcutCommandManager::~ShortcutCommandManager ()
{
#ifndef SIMON_SCENARIOS
	if (xmlShortcutCommand) xmlShortcutCommand->deleteLater();
#endif
}
