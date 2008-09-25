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
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/Logging/logger.h"
#include "xmlplacecommand.h"
#include "placecommand.h"
#include <KLocalizedString>

PlaceCommandManager::PlaceCommandManager(QObject *parent) : CommandManager(parent)
{
	this->xmlPlaceCommand = new XMLPlaceCommand();
}

const QString PlaceCommandManager::name() const
{
	return PlaceCommand::staticCategoryText();
}

bool PlaceCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<PlaceCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

bool PlaceCommandManager::load()
{
	QString commandPath = Settings::getS("Commands/Place/PathToConfig");
	Logger::log(i18n("[INF] Lade Ort-Kommandos von %1", commandPath));

	bool ok = false;
	this->commands = xmlPlaceCommand->load(ok, commandPath);
	return ok;
}

bool PlaceCommandManager::save()
{
	QString commandPath = Settings::getS("Commands/Place/PathToConfig");
	Logger::log(i18n("[INF] Speichere Ort-Kommandos nach %1", commandPath));
	return xmlPlaceCommand->save(commands, commandPath);
}

PlaceCommandManager::~PlaceCommandManager() 
{
	if (xmlPlaceCommand) xmlPlaceCommand->deleteLater();
}
