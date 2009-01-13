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
#include "xmlexecutablecommand.h"
#include "executablecommand.h"
#include "createexecutablecommandwidget.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ExecutableCommandPluginFactory, 
			registerPlugin< ExecutableCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( ExecutableCommandPluginFactory("simonexecutablecommand") )


ExecutableCommandManager::ExecutableCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
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

const KIcon ExecutableCommandManager::icon() const
{
	return ExecutableCommand::staticCategoryIcon();
}


CreateCommandWidget* ExecutableCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateExecutableCommandWidget(parent);
}

bool ExecutableCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/executables.xml");
	Logger::log(i18n("[INF] Loading executable commands from %1", commandPath));

	bool ok = false;
	this->commands = xmlExecutableCommand->load(ok, commandPath);
	return ok;
}

bool ExecutableCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/executables.xml");
	Logger::log(i18n("[INF] Saving executable commands to %1", commandPath));
	return xmlExecutableCommand->save(commands, commandPath);
}

ExecutableCommandManager::~ExecutableCommandManager()
{
	if (xmlExecutableCommand) xmlExecutableCommand->deleteLater();
}
