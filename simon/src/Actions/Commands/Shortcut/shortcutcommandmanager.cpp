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
#include "../../../SimonLib/Logging/logger.h"
#include "xmlshortcutcommand.h"
#include "shortcutcommand.h"
#include <KLocalizedString>
#include <KStandardDirs>


ShortcutCommandManager::ShortcutCommandManager(QObject *parent) :CommandManager(parent)  
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

const QString ShortcutCommandManager::name() const
{
	return ShortcutCommand::staticCategoryText();
}

bool ShortcutCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/shortcuts.xml");
	Logger::log(i18n("[INF] Lade Tastenkürzel von %1", commandPath));

	bool ok = false;
	this->commands = xmlShortcutCommand->load(ok, commandPath);
	return ok;
}

bool ShortcutCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/shortcuts.xml");
	Logger::log(i18n("[INF] Speichere Shortcut-Kommandos nach %1", commandPath));
	return xmlShortcutCommand->save(commands, commandPath);
}


ShortcutCommandManager::~ShortcutCommandManager ()
{
	if (xmlShortcutCommand) xmlShortcutCommand->deleteLater();
}
