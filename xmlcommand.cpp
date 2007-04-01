#include "xmlcommand.h"


XMLCommand::XMLCommand(QString path)
{
	QDomDocument doc = QDomDocument("mydocument");
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))
		return;
	if(!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	//XMLReader reader(path);
}


void XMLCommand::save(CommandList commandlist, QString path)
{
	QDomDocument doc("mycommands");
	QDomElement root = doc.createElement("commandlist");
	doc.appendChild(root);
	
	QDomNode newnode;
	
	for(int i=0; i<=commandlist.size(); i++)
	{
		newnode.setNodeValue(commandlist[i]->getValue());
		
		QDomElement command = newnode.toElement();
		
		command.setTagName("command");
		
		command.setAttribute("name", commandlist[i]->getName());
		command.setAttribute("type", QString().setNum(commandlist[i]->getType()));
		
		//<command name="firefox" type="0"></command>
		
		
	/*	QDomElement name = doc.createTextNode(commandlist[i]->getName());
		command.appendChild(name);
		
		QDomElement type = doc.createTextNode((commandlist[i]->getType()).toString());
		command.appendChild(type);
		
		QDomElement value = doc.createTextNode(commandlist[i]->getValue());
		command.appendChild(value);*/
		
		root.appendChild(command);
	}
}


void XMLCommand::load()
{
	
	QDomElement root = this->doc.documentElement();
	
	QDomNode c = root.firstChild();
	QDomElement command = c.toElement();
	
	while(!command.isNull())
	{
		QString name = command.attribute("name");
		QString type = command.attribute("type");
		QString value = command.text();
		
			
		commandlist.append(new Command(name, CommandType(type.toInt()), value));

		command = c.nextSiblingElement();
	}
}


CommandList XMLCommand::getCommands()
{
	return this->commandlist;
}


Command XMLCommand::getCommand(int index)
{
	//return Command(commandlist[index].getName(), ((CommandType)commandlist[index].getType()), commandlist[index].getValue());
}


XMLCommand::~XMLCommand()
{

}