#include "xmlexecutablecommand.h"
#include "executablecommand.h"

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
XMLExecutableCommand::XMLExecutableCommand(QString path):XMLDomReader(path)
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
bool XMLExecutableCommand::save(CommandList *list, QString path)
{
	if (this->doc)
		this->doc->clear();
	else doc = new QDomDocument ();

	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < list->size(); i++)
	{
		ExecutableCommand *com = dynamic_cast<ExecutableCommand*>(list->at(i));
		if (!com) continue;
		
		QDomElement command = doc->createElement("executable");
		QDomElement name = doc->createElement("name");
		name.appendChild(doc->createTextNode(com->getTrigger()));
		QDomElement icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(com->getIconSrc()));
		QDomElement workingDir = doc->createElement("workingdirectory");
		workingDir.appendChild(doc->createTextNode(com->getWorkingDirectory().toString()));
		QDomElement executable = doc->createElement("executable");
		executable.appendChild(doc->createTextNode(com->getExecutable()));
		
		command.appendChild(name);
		command.appendChild(icon);
		command.appendChild(workingDir);
		command.appendChild(executable);
		
		root.appendChild(command);
	}
	
	return XMLDomReader::save(path);
}
/**
 * \brief Loads the commands
 * @author Peter Grasch
 */
CommandList* XMLExecutableCommand::load(bool &ok, QString path)
{
	if (!XMLDomReader::load(path) || !this->doc)
	{
		ok = false;
		return 0;
	}

	CommandList *list = new CommandList();

	QDomElement root = this->doc->documentElement();
	QDomElement exe = root.firstChildElement();
	while(!exe.isNull())
	{
		QDomElement name = exe.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement workingDir = icon.nextSiblingElement();
		QDomElement executable = workingDir.nextSiblingElement();
		list->append(new ExecutableCommand(name.text(), icon.text(), executable.text(), workingDir.text()));
		exe = exe.nextSiblingElement();
	}
	ok = true;
	return list;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLExecutableCommand::~XMLExecutableCommand()
{

}
