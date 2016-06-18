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

#include "textmacrocommandmanager.h"
#include <simonlogging/logger.h>
#include <simonscenarios/scenario.h>
#include "textmacrocommand.h"
#include <KI18n/klocalizedstring.h>

#include <KPluginFactory>
#include "createtextmacrocommandwidget.h"

K_PLUGIN_FACTORY( TextMacroCommandPluginFactory,
registerPlugin< TextMacroCommandManager >();
)

// K_EXPORT_PLUGIN( TextMacroCommandPluginFactory("simontextmacrocommand") )

TextMacroCommandManager::TextMacroCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}


const QString TextMacroCommandManager::name() const
{
  return TextMacroCommand::staticCategoryText();
}


bool TextMacroCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<TextMacroCommand*>(command) != 0);
}


const QString TextMacroCommandManager::iconSrc() const
{
  return "format-text-bold";
}


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(TextMacroCommandManager, TextMacroCommand);

CreateCommandWidget* TextMacroCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateTextMacroCommandWidget(this, parent);
}


TextMacroCommandManager::~TextMacroCommandManager()
{
}

#include "textmacrocommandmanager.moc"
