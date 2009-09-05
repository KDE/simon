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
#ifndef SIMON_SCENARIOS
	, xmlExecutableCommand(new XMLExecutableCommand())
#endif
{
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


bool ExecutableCommandManager::deSerializeCommands(const QDomElement& elem, const QString& scenarioId)
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
		QDomElement workingDir = icon.nextSiblingElement();
		QDomElement executable = workingDir.nextSiblingElement();
		commands->append(new ExecutableCommand(name.text(), icon.text(), 
						executable.text(), workingDir.text()));
		commandElem = commandElem.nextSiblingElement();
	}

	return true;
}


bool ExecutableCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/executables.xml");
	Logger::log(i18n("[INF] Loading executable commands from %1", commandPath));

#ifndef SIMON_SCENARIOS
	bool ok = false;
	this->commands = xmlExecutableCommand->load(ok, commandPath);
	return ok;
#else
	return true;
#endif
}

bool ExecutableCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/executables.xml");
	Logger::log(i18n("[INF] Saving executable commands to %1", commandPath));
#ifndef SIMON_SCENARIOS
	return xmlExecutableCommand->save(commands, commandPath);
#else
	return true;
#endif
}

ExecutableCommandManager::~ExecutableCommandManager()
{
#ifndef SIMON_SCENARIOS
	if (xmlExecutableCommand) xmlExecutableCommand->deleteLater();
#endif
}

