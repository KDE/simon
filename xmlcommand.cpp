#include "xmlcommand.h"
#include <QMessageBox>

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
	this->doc->clear();
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
	
	return (XMLDomReader::save(path)==0);   //!=0 -> false
}


/**
 * \brief Replaces a command of the commandlist (member) identified by <commandName> with the new command supplied by <newCommand>
 * \author Susanne Tschernegg
 * @param QString commandName
 *      holds the name of the command
 * @param Command *newCommand
 *      is an object of a command
 */
void XMLCommand::replaceCommand(QString commandName, Command *newCommand)
{
	for (int i=0; i < commandlist.size(); i++)
	{
		if(commandlist.at(i)->getName()==commandName)
		{
			commandlist.replace(i, newCommand);
		}
	}
}

/**
 * \brief Adds a command to the commandlist (member)
 * \author Susanne Tschernegg
 * @param Command *newCommand
 *      is an object of a command
 */
void XMLCommand::addCommand(Command *newCommand)
{
	commandlist.append(newCommand);
}

/**
 * \brief deletes a command from the commandlist.
 * \author Susanne Tschernegg
 * @param QString commandName
 *      is an object of a command
 */
void XMLCommand::deleteCommand(QString commandName)//, QString commandValue)
{
	for (int i=0; i < commandlist.size(); i++)
	{
		if(commandlist.at(i)->getName()==commandName)
		{
            //if(commandlist.at(i)->getValue()==commandValue)
			    commandlist.removeAt(i);
		}
	}
}

/**
 * \brief returns wheater the command exists or not
 * \author Susanne Tschernegg
 * @param QString commandName
 *      holds the name of the command
 * @return bool
 *      if the command exists, this method will return true, otherwise it returns false
 */
bool XMLCommand::commandExists(QString commandName)
{
	for (int i=0; i < commandlist.size(); i++)
	{
		if(commandlist.at(i)->getName()==commandName)
		{
			return true;
		}
	}
	return false;
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
	XMLDomReader::load(path);
	if (!this->doc) return false;
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
