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
#include <simonactions/listcommand.h>
#include "createlistcommandwidget.h"
#include <simonlogging/logger.h>
#include <simonscenarios/scenario.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( ListCommandPluginFactory, 
			registerPlugin< ListCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( ListCommandPluginFactory("simonlistcommand") )


ListCommandManager::ListCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args)
{
}

const KIcon ListCommandManager::icon() const
{
	return ListCommand::staticCategoryIcon();
}

bool ListCommandManager::addCommand(Command *command)
{
	if (dynamic_cast<ListCommand*>(command))
	{
		beginInsertRows(QModelIndex(), commands->count(), commands->count());
		this->commands->append(command);
		endInsertRows();
		return parentScenario->save();
	}
	return false;
}

const QString ListCommandManager::name() const
{
	return ListCommand::staticCategoryText();
}


CreateCommandWidget* ListCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateListCommandWidget(this, parent);
}

bool ListCommandManager::deSerializeCommands(const QDomElement& elem)
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
		QDomElement childCommandsElem = icon.nextSiblingElement();
		QDomElement childCommandElem = childCommandsElem.firstChildElement();
		QStringList childCommandTrigger;
		QStringList childCommandIcons;
		QStringList childCommandCategory;
		while (!childCommandElem.isNull()) {
			QDomElement childCommandTriggerElem = childCommandElem.firstChildElement();
			QDomElement childCommandIconElem = childCommandTriggerElem.nextSiblingElement();
			QDomElement childCommandCategoryElem = childCommandIconElem.nextSiblingElement();
			childCommandTrigger << childCommandTriggerElem.text();
			childCommandIcons << childCommandIconElem.text();
			childCommandCategory << childCommandCategoryElem.text();
			childCommandElem = childCommandElem.nextSiblingElement();
		}
		kDebug() << "Triggers: " << childCommandTrigger;
		kDebug() << "Icons: " << childCommandIcons;
		kDebug() << "Categories: " << childCommandCategory;

		commands->append(new ListCommand(name.text(), icon.text(), 
						childCommandTrigger, childCommandIcons, childCommandCategory));
		commandElem = commandElem.nextSiblingElement();
	}


	return true;
}

void ListCommandManager::setFont(const QFont& font)
{
	foreach (Command *c, *commands) {
		ListCommand *lc = dynamic_cast<ListCommand*>(c);
		if (!lc) continue;
		lc->setFont(font);
	}
}


ListCommandManager::~ListCommandManager()
{
}
