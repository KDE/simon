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
#include <simonlogging/logger.h>
#include "xmltextmacrocommand.h"
#include "textmacrocommand.h"
#include <KLocalizedString>
#include <KStandardDirs>
#include "createtextmacrocommandwidget.h"

K_PLUGIN_FACTORY( TextMacroCommandPluginFactory, 
			registerPlugin< TextMacroCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( TextMacroCommandPluginFactory("TextMacroCommandManager") )


TextMacroCommandManager::TextMacroCommandManager(QObject *parent, const QVariantList& args) : CommandManager(parent, args)
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
	QString commandPath = KStandardDirs::locate("appdata", "conf/textmacros.xml");
	Logger::log(i18n("[INF] Lade Text-Makro-Kommandos von %1", commandPath));

	bool ok = false;
	this->commands = xmlTextMacroCommand->load(ok, commandPath);
	return ok;
}

CreateCommandWidget* TextMacroCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateTextMacroCommandWidget(parent);
}

bool TextMacroCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/textmacros.xml");
	Logger::log(i18n("[INF] Speichere Text-Makro-Kommandos nach %1", commandPath));
	return xmlTextMacroCommand->save(commands, commandPath);
}


TextMacroCommandManager::~TextMacroCommandManager()
{
	if (xmlTextMacroCommand) xmlTextMacroCommand->deleteLater();
}
