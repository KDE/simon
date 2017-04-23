/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include <KPluginFactory>

K_PLUGIN_FACTORY( PlaceCommandPluginFactory,
registerPlugin< PlaceCommandManager >();
)

// K_EXPORT_PLUGIN( PlaceCommandPluginFactory("simonplacecommand") )

PlaceCommandManager::PlaceCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}


const QString PlaceCommandManager::iconSrc() const
{
  return ("folder");
}


CreateCommandWidget* PlaceCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreatePlaceCommandWidget(this, parent);
}


const QString PlaceCommandManager::name() const
{
  return PlaceCommand::staticCategoryText();
}


bool PlaceCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<PlaceCommand*>(command) != 0);
}


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(PlaceCommandManager, PlaceCommand);

PlaceCommandManager::~PlaceCommandManager()
{
}

#include "placecommandmanager.moc"
