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
#include "listcommandmanager.h"
#include "xmllistcommand.h"
#include <simonlistcommand/listcommand.h>
#include "createlistcommandwidget.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ListCommandPluginFactory, 
			registerPlugin< ListCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( ListCommandPluginFactory("simonlistcommand") )


ListCommandManager::ListCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
	this->xmlListCommand = new XMLListCommand();
}

const KIcon ListCommandManager::icon() const
{
	return ListCommand::staticCategoryIcon();
}

bool ListCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<ListCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

const QString ListCommandManager::name() const
{
	return ListCommand::staticCategoryText();
}


CreateCommandWidget* ListCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateListCommandWidget(parent);
}

bool ListCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/lists.xml");
	Logger::log(i18n("[INF] Loading list commands from %1", commandPath));

	bool ok = false;
	this->commands = xmlListCommand->load(ok, commandPath);
	return ok;
}

bool ListCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/lists.xml");
	Logger::log(i18n("[INF] Saving list commands to %1", commandPath));
	return xmlListCommand->save(commands, commandPath);
}

ListCommandManager::~ListCommandManager()
{
	if (xmlListCommand) xmlListCommand->deleteLater();
}
