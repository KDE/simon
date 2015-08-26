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
#include "compositecommandmanager.h"
#include "compositecommand.h"
#include "createcompositecommandwidget.h"
#include <simonscenarios/scenario.h>
#include <simonlogging/logger.h>
#include <KI18n/klocalizedstring.h>


K_PLUGIN_FACTORY( CompositeCommandPluginFactory,
registerPlugin< CompositeCommandManager >();
)

// K_EXPORT_PLUGIN( CompositeCommandPluginFactory("simoncompositecommand") )

CompositeCommandManager::CompositeCommandManager(QObject* parent, const QVariantList& args) :CommandManager((Scenario*) parent, args)
{
}


const QString CompositeCommandManager::iconSrc() const
{
  return "view-choose";
}


bool CompositeCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<CompositeCommand*>(command) != 0);
}


const QString CompositeCommandManager::name() const
{
  return CompositeCommand::staticCategoryText();
}


CreateCommandWidget* CompositeCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateCompositeCommandWidget(this, parent);
}


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(CompositeCommandManager, CompositeCommand);

CompositeCommandManager::~CompositeCommandManager()
{
}

#include "compositecommandmanager.moc"