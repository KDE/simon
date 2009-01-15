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

#include "xmlcompositecommand.h"
#include "compositecommand.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>

/**
 * \brief Creates a new XMLCommand object
 * \author Peter Grasch
 * \param QString path
 * Path to the commands.xml; Default: conf/commands.xml
 */
XMLCompositeCommand::XMLCompositeCommand(const QString& path):XMLDomReader(path)
{ }



/**
 * \brief Saves the commands
 * @author Peter Grasch
 * @param commandlist 
 * The list of commands
 * @param path
 * The path to save to
 * @return returns, if the command were saved or not
 */
bool XMLCompositeCommand::save(CommandList *list, const QString &path)
{
	if (this->doc)
		this->doc->clear();
	else doc = new QDomDocument ();

	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < list->size(); i++)
	{
		CompositeCommand *com = dynamic_cast<CompositeCommand*>(list->at(i));
		if (!com) continue;
		
		QDomElement command = doc->createElement("composite");
		QDomElement name = doc->createElement("name");
		name.appendChild(doc->createTextNode(com->getTrigger()));
		QDomElement icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(com->getIconSrc()));

		command.appendChild(name);
		command.appendChild(icon);

		QStringList commands = com->getCommands();
		QStringList commandTypes = com->getCommandTypes();
		for (int i=0; i < commands.count(); i++)
		{
			QDomElement childCommand = doc->createElement("childTrigger");
			childCommand.appendChild(doc->createTextNode(commands[i]));
			command.appendChild(childCommand);
			QDomElement childCommandType = doc->createElement("childCategory");
			childCommandType.appendChild(doc->createTextNode(commandTypes[i]));
			command.appendChild(childCommandType);
		}
		
		root.appendChild(command);
	}
	
	return XMLDomReader::save(path);
}
/**
 * \brief Loads the commands
 * @author Peter Grasch
 */
CommandList* XMLCompositeCommand::load(bool &ok, const QString &path)
{
	if (!XMLDomReader::load(path) || !this->doc)
	{
		ok = false;
		return 0;
	}

	CommandList *list = new CommandList();

	QDomElement root = this->doc->documentElement();
	QDomElement comp = root.firstChildElement();
	while(!comp.isNull())
	{
		QDomElement name = comp.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement childCommand = icon.nextSiblingElement();
		
		QStringList childTriggers;
		QStringList childCategories;
		do 
		{
			childTriggers << childCommand.text();
			childCommand = childCommand.nextSiblingElement();
			childCategories << childCommand.text();
			childCommand = childCommand.nextSiblingElement();
		} while (!childCommand.isNull());

		list->append(new CompositeCommand(name.text(), icon.text(), childTriggers, childCategories));
		comp = comp.nextSiblingElement();
	}
	ok = true;
	return list;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLCompositeCommand::~XMLCompositeCommand()
{

}
