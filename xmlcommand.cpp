#include "xmlcommand.h"

#include <QtXml>
#include <QString>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>

/**
 * \brief Creates a new XMLCommand object
 * \author Peter Grasch
 * \param QString path
 * Path to the commands.xml; Default: conf/commands.xml
 */
XMLCommand::XMLCommand(QString path):XMLDomReader(path)
{
	this->commandlist = CommandList();
}



/**
 * \brief Saves the commands
 * @author Susanne Tschernegg
 * @param commandlist 
 * The list of commands
 * @param path
 * The path to save to; Default: conf/commands.xml
 * @return returns, if the command were saved or not
 */
bool XMLCommand::save(QString path)
{
	if (this->doc)
		this->doc->clear();
	else doc = new QDomDocument ();

	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < commandlist.size(); i++)
	{
		QDomElement command = doc->createElement("command");
		command.setAttribute("type", QString::number(commandlist.at(i)->getType()));
		command.setAttribute("name", commandlist.at(i)->getName());
		command.setAttribute("iconPath", commandlist.at(i)->getIconPath());
		command.setAttribute("workingDirectory", commandlist.at(i)->getWorkingDirectory());
		QDomText commandValue = doc->createTextNode(commandlist.at(i)->getValue());
		command.appendChild(commandValue);

		root.appendChild(command);
	}
	
	return XMLDomReader::save(path);
}
/**
 * \brief Loads the commands
 * @author Peter Grasch
 * @param commandlist 
 * The list of commands
 * @param path
 * The path to save to; If not given the path used to construct the object is used
 */
bool XMLCommand::load(QString path)
{
	commandlist.clear();
	if (!XMLDomReader::load(path) || !this->doc)
		return false;

	QDomElement root = this->doc->documentElement();
	QDomElement command = root.firstChildElement();
	while(!command.isNull())
	{
		QString name = command.attribute("name");
		QString type = command.attribute("type");
		QString value = command.text().trimmed();
		QString iconPath = command.attribute("iconPath");
		QString workingDirectory = command.attribute("workingDirectory");
		
			
		commandlist.append(new Command(name, CommandType(type.toInt()), value, iconPath, workingDirectory));

		command = command.nextSiblingElement();
	}
	return true;
}


/**
 * \brief Returns the commandlist
 * \author Peter Grasch
 * @return CommandList
 * The member:commandlist
 */
CommandList XMLCommand::getCommands()
{
	return this->commandlist;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLCommand::~XMLCommand()
{

}
