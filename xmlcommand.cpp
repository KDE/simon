#include "xmlcommand.h"


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
 * @todo This is not yet implemented
 */
void XMLCommand::save(QString path)
{
	this->doc->clear();
	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < commandlist.size(); i++)
	{
		QDomElement command = doc->createElement("command");
		command.setAttribute("type", QString::number(commandlist.at(i)->getType()));
		command.setAttribute("name", commandlist.at(i)->getName());
		QDomText commandValue = doc->createTextNode(commandlist.at(i)->getValue());
		command.appendChild(commandValue);
		
		root.appendChild(command);
	}
	
	XMLDomReader::save(path);
}


/**
 * \brief Replaces a command of the commandlist (member) identified by <commandName> with the new command supplied by <newCommand>
 * \author Susanne Tschernegg
 */
void XMLCommand::replaceCommand(QString commandName, Command newCommand)
{
	
}

/**
 * \brief Replaces a command of the commandlist (member) identified by <commandName> with the new command supplied by <newCommand>
 * \author Susanne Tschernegg
 */
void XMLCommand::addCommand(Command newCommand)
{
	
}


/**
 * \brief Loads the commands
 * @author Peter Grasch
 * @param commandlist 
 * The list of commands
 * @param path
 * The path to save to; If not given the path used to construct the object is used
 */
void XMLCommand::load(QString path)
{
	XMLDomReader::load(path);
	if (!this->doc) return;
	
	QDomElement root = this->doc->documentElement();
	QDomElement command = root.firstChildElement();
	
	while(!command.isNull())
	{
		QString name = command.attribute("name");
		QString type = command.attribute("type");
		QString value = command.text().trimmed();
		
			
		commandlist.append(new Command(name, CommandType(type.toInt()), value));

		command = command.nextSiblingElement();
	}
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
