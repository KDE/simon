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
#include "compositecommandmanager.h"
#include "compositecommand.h"
#include "createcompositecommandwidget.h"
#include <simonscenarios/scenario.h>
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KStandardDirs>

K_PLUGIN_FACTORY( CompositeCommandPluginFactory, 
			registerPlugin< CompositeCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( CompositeCommandPluginFactory("simoncompositecommand") )


CompositeCommandManager::CompositeCommandManager(QObject* parent, const QVariantList& args) :CommandManager((Scenario*) parent, args)  
{
}

const QString CompositeCommandManager::iconSrc() const
{
	return "view-choose";
}

bool CompositeCommandManager::shouldAcceptCommand(Command *command)
{
	return (dynamic_cast<CompositeCommand*>(command) != NULL);
}

const QString CompositeCommandManager::name() const
{
	return CompositeCommand::staticCategoryText();
}


CreateCommandWidget* CompositeCommandManager::getCreateCommandWidget(QWidget *parent)
{
	return new CreateCompositeCommandWidget(this, parent);
}

/*
bool CompositeCommandManager::deSerializeCommands(const QDomElement& elem)
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
		QDomElement childCommandsElem = description.nextSiblingElement();
		QDomElement childCommandElem = childCommandsElem.firstChildElement();
		QStringList childCommandTrigger;
		QStringList childCommandCategory;
		while (!childCommandElem.isNull()) {
			QDomElement childCommandTriggerElem = childCommandElem.firstChildElement();
			QDomElement childCommandCategoryElem = childCommandTriggerElem.nextSiblingElement();
			childCommandTrigger << childCommandTriggerElem.text();
			childCommandCategory << childCommandCategoryElem.text();
			kDebug() << childCommandTriggerElem.text();
			kDebug() << childCommandCategoryElem.text();
			childCommandElem = childCommandElem.nextSiblingElement();
		}

		commands->append(new CompositeCommand(name.text(), icon.text(), description.text(),
						childCommandTrigger, childCommandCategory));
		commandElem = commandElem.nextSiblingElement();
	}

	return true;
}
*/


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(CompositeCommandManager, CompositeCommand);

CompositeCommandManager::~CompositeCommandManager()
{
}
