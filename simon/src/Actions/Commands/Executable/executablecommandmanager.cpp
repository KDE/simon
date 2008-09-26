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
#include "executablecommandmanager.h"
#include "../../../SimonLib/Logging/logger.h"
#include "xmlexecutablecommand.h"
#include "executablecommand.h"
#include <KLocalizedString>
#include <KStandardDirs>


ExecutableCommandManager::ExecutableCommandManager(QObject *parent) :CommandManager(parent)  
{
	this->xmlExecutableCommand = new XMLExecutableCommand();
}

bool ExecutableCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<ExecutableCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

const QString ExecutableCommandManager::name() const
{
	return ExecutableCommand::staticCategoryText();
}

bool ExecutableCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/executables.xml");
	Logger::log(i18n("[INF] Lade Ausführbare-Kommandos von %1", commandPath));

	bool ok = false;
	this->commands = xmlExecutableCommand->load(ok, commandPath);
	return ok;
}

bool ExecutableCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/executables.xml");
	Logger::log(i18n("[INF] Speichere Ausführbare-Kommandos nach %1", commandPath));
	return xmlExecutableCommand->save(commands, commandPath);
}

ExecutableCommandManager::~ExecutableCommandManager()
{
	if (xmlExecutableCommand) xmlExecutableCommand->deleteLater();
}
