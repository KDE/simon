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

#include "textmacrocommandmanager.h"
#include <KLocalizedString>
#include "../../../SimonLib/Settings/settings.h"
#include "../../../SimonLib/Logging/logger.h"
#include "xmltextmacrocommand.h"
#include "textmacrocommand.h"

TextMacroCommandManager::TextMacroCommandManager(QObject *parent) : CommandManager(parent)
{
	this->xmlTextMacroCommand = new XMLTextMacroCommand();
}

bool TextMacroCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<TextMacroCommand*>(command))
	{
		this->commands->append(command);
		return save();
	}
	return false;
}

const QString TextMacroCommandManager::name() const
{
	return TextMacroCommand::staticCategoryText();
}

bool TextMacroCommandManager::load()
{
	QString commandPath = Settings::getS("Commands/TextMacro/PathToConfig");
	Logger::log(i18n("[INF] Lade Text-Makro-Kommandos von %1", commandPath));

	bool ok = false;
	this->commands = xmlTextMacroCommand->load(ok, commandPath);
	return ok;
}

bool TextMacroCommandManager::save()
{
	QString commandPath = Settings::getS("Commands/TextMacro/PathToConfig");
	Logger::log(i18n("[INF] Speichere Text-Makro-Kommandos nach %1", commandPath));
	return xmlTextMacroCommand->save(commands, commandPath);
}


TextMacroCommandManager::~TextMacroCommandManager()
{
	if (xmlTextMacroCommand) xmlTextMacroCommand->deleteLater();
}
