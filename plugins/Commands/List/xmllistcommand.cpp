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

#include "xmllistcommand.h"
#include <simonactions/listcommand.h>

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
XMLListCommand::XMLListCommand(const QString& path):XMLDomReader(path)
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
bool XMLListCommand::save(CommandList *list, const QString &path)
{
	if (this->doc)
		this->doc->clear();
	else doc = new QDomDocument ();

	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < list->size(); i++)
	{
		ListCommand *com = dynamic_cast<ListCommand*>(list->at(i));
		if (!com) continue;
		
		QDomElement command = doc->createElement("list");
		QDomElement name = doc->createElement("name");
		name.appendChild(doc->createTextNode(com->getTrigger()));
		QDomElement icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(com->getIconSrc()));

		command.appendChild(name);
		command.appendChild(icon);

		QStringList commands = com->getCommands();
		QStringList iconsrcs = com->getIconSrcs();
		QStringList commandTypes = com->getCommandTypes();
		for (int i=0; i < commands.count(); i++)
		{
			QDomElement childCommand = doc->createElement("childTrigger");
			childCommand.appendChild(doc->createTextNode(commands[i]));
			command.appendChild(childCommand);
			QDomElement childIconSrc = doc->createElement("childIconsrc");
			childIconSrc.appendChild(doc->createTextNode(iconsrcs[i]));
			command.appendChild(childIconSrc);
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
CommandList* XMLListCommand::load(bool &ok, const QString &path)
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
		QStringList childIconSrcs;
		do 
		{
			childTriggers << childCommand.text();
			childCommand = childCommand.nextSiblingElement();
			childIconSrcs << childCommand.text();
			childCommand = childCommand.nextSiblingElement();
			childCategories << childCommand.text();
			childCommand = childCommand.nextSiblingElement();
		} while (!childCommand.isNull());

		list->append(new ListCommand(name.text(), icon.text(), childTriggers, childIconSrcs, childCategories));
		comp = comp.nextSiblingElement();
	}
	ok = true;
	return list;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLListCommand::~XMLListCommand()
{

}
