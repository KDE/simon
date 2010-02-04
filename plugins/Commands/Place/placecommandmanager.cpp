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

#include "placecommandmanager.h"
#include "createplacecommandwidget.h"
#include <simonlogging/logger.h>
#include <simonscenarios/scenario.h>
#include "placecommand.h"
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( PlaceCommandPluginFactory, 
			registerPlugin< PlaceCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( PlaceCommandPluginFactory("simonplacecommand") )

PlaceCommandManager::PlaceCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}


const KIcon PlaceCommandManager::icon() const
{
	return PlaceCommand::staticCategoryIcon();
}


CreateCommandWidget* PlaceCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreatePlaceCommandWidget(this, parent);
}

const QString PlaceCommandManager::name() const
{
	return PlaceCommand::staticCategoryText();
}

bool PlaceCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<PlaceCommand*>(command))
	{
		beginInsertRows(QModelIndex(), commands->count(), commands->count());
		this->commands->append(command);
		endInsertRows();
		return parentScenario->save();
	}
	return false;
}


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(PlaceCommandManager, PlaceCommand);

/*
 * bool PlaceCommandManager::deSerializeCommands(const QDomElement& elem)
{
	if (commands)
		qDeleteAll(*commands);
	commands = new CommandList();

	if (elem.isNull()) return false;

	QDomElement commandElem = elem.firstChildElement();
	while(!commandElem.isNull())
	{
		QDomElement name = commandElem.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement description = icon.nextSiblingElement();
		QDomElement url = description.nextSiblingElement();
		commands->append(new PlaceCommand(name.text(), icon.text(), description.text(),
						url.text()));
		commandElem = commandElem.nextSiblingElement();
	}

	return true;
}
*/

PlaceCommandManager::~PlaceCommandManager() 
{
}
