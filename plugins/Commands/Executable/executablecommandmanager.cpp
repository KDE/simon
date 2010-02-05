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
#include "executablecommand.h"
#include "createexecutablecommandwidget.h"
#include <simonlogging/logger.h>
#include <simonscenarios/scenario.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ExecutableCommandPluginFactory, 
			registerPlugin< ExecutableCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( ExecutableCommandPluginFactory("simonexecutablecommand") )


ExecutableCommandManager::ExecutableCommandManager(QObject* parent, const QVariantList& args) : 
	CommandManager((Scenario*) parent, args)
{
}

bool ExecutableCommandManager::addCommandPrivate(Command *command)
{
	if (dynamic_cast<ExecutableCommand*>(command))
	{
		beginInsertRows(QModelIndex(), commands->count(), commands->count());
		this->commands->append(command);
		endInsertRows();
		return parentScenario->save();
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
	return new CreateExecutableCommandWidget(this, parent);
}

DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(ExecutableCommandManager, ExecutableCommand);

ExecutableCommandManager::~ExecutableCommandManager()
{
}

