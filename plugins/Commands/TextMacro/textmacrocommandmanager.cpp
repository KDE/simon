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
        
K_EXPORT_PLUGIN( TextMacroCommandPluginFactory("simontextmacrocommand") )


TextMacroCommandManager::TextMacroCommandManager(QObject* parent, const QVariantList& args) : CommandManager(parent, args),
	xmlTextMacroCommand(new XMLTextMacroCommand())
{
}

const QString TextMacroCommandManager::name() const
{
	return TextMacroCommand::staticCategoryText();
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

const KIcon TextMacroCommandManager::icon() const
{
	return TextMacroCommand::staticCategoryIcon();
}

bool TextMacroCommandManager::load()
{
	QString commandPath = KStandardDirs::locate("appdata", "conf/textmacros.xml");
	Logger::log(i18n("[INF] Loading text-macro commands from %1", commandPath));

	bool ok = false;
	this->commands = xmlTextMacroCommand->load(ok, commandPath);
	return ok;
}

bool TextMacroCommandManager::deSerializeCommands(const QDomElement& elem, const QString& scenarioId)
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
		QDomElement textElem = icon.nextSiblingElement();
		commands->append(new TextMacroCommand(name.text(), icon.text(), 
						textElem.text()));
		commandElem = commandElem.nextSiblingElement();
	}

	return true;
}

CreateCommandWidget* TextMacroCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateTextMacroCommandWidget(parent);
}

bool TextMacroCommandManager::save()
{
	QString commandPath = KStandardDirs::locateLocal("appdata", "conf/textmacros.xml");
	Logger::log(i18n("[INF] Saving text-macro commands to %1", commandPath));
	return xmlTextMacroCommand->save(commands, commandPath);
}


TextMacroCommandManager::~TextMacroCommandManager()
{
	if (xmlTextMacroCommand) xmlTextMacroCommand->deleteLater();
}
