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
#include <simonscenarios/scenario.h>
#include "shortcutcommand.h"
#include "createshortcutcommandwidget.h"
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ShortcutCommandPluginFactory,
registerPlugin< ShortcutCommandManager >();
)

K_EXPORT_PLUGIN( ShortcutCommandPluginFactory("simonshortcutcommand") )

ShortcutCommandManager::ShortcutCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}


const QString ShortcutCommandManager::name() const
{
  return ShortcutCommand::staticCategoryText();
}


bool ShortcutCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<ShortcutCommand*>(command) != 0);
}


CreateCommandWidget* ShortcutCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateShortcutCommandWidget(this, parent);
}


const QString ShortcutCommandManager::iconSrc() const
{
  return "go-jump-locationbar";
}


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(ShortcutCommandManager, ShortcutCommand)

ShortcutCommandManager::~ShortcutCommandManager ()
{
}
