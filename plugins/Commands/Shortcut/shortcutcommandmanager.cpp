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


ShortcutCommandManager::ShortcutCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
	this->xmlShortcutCommand = new XMLShortcutCommand();
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

const QString ShortcutCommandManager::name() const
{
	return ShortcutCommand::staticCategoryText();
}

bool ShortcutCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/shortcuts.xml");
	Logger::log(i18n("[INF] Loading shortcuts from %1", commandPath));

	bool ok = false;
	this->commands = xmlShortcutCommand->load(ok, commandPath);
	return ok;
}

bool ShortcutCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/shortcuts.xml");
	Logger::log(i18n("[INF] Saving Shortcuts to %1", commandPath));
	return xmlShortcutCommand->save(commands, commandPath);
}


ShortcutCommandManager::~ShortcutCommandManager ()
{
	if (xmlShortcutCommand) xmlShortcutCommand->deleteLater();
}
