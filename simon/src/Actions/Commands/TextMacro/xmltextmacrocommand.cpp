#include "xmltextmacrocommand.h"
#include "textmacrocommand.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>

/**
 * \brief Creates a new XMLTextMacroCommand object
 * \author Peter Grasch
 * \param QString path
 * Path to the commands.xml; Default: conf/commands.xml
 */
XMLTextMacroCommand::XMLTextMacroCommand(QString path):XMLDomReader(path)
{ }



/**
 * \brief Saves the commands
 * @author Peter Grasch
 * @param commandlist 
 * The list of commands
 * @param path
 * The path to save to; Default: conf/commands.xml
 * @return returns, if the command were saved or not
 */
bool XMLTextMacroCommand::save(CommandList *list, QString path)
{
	if (this->doc)
		this->doc->clear();
	else doc = new QDomDocument ();

	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < list->size(); i++)
	{
		//FIXME: implement
		TextMacroCommand *com = dynamic_cast<TextMacroCommand*>(list->at(i));
		if (!com) continue;
		
		QDomElement command = doc->createElement("textmacro");
		QDomElement name = doc->createElement("name");
		name.appendChild(doc->createTextNode(com->getTrigger()));
		QDomElement icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(com->getIconSrc()));
		QDomElement text = doc->createElement("text");
		text.appendChild(doc->createTextNode(com->getText()));
		
		command.appendChild(name);
		command.appendChild(icon);
		command.appendChild(text);
		
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
CommandList* XMLTextMacroCommand::load(bool &ok, QString path)
{
	if (!XMLDomReader::load(path) || !this->doc)
	{
		ok = false;
		return 0;
	}

	CommandList *list = new CommandList();

	QDomElement root = this->doc->documentElement();
	QDomElement textMacro = root.firstChildElement();
	while(!textMacro.isNull())
	{
		QDomElement name = textMacro.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement text = icon.nextSiblingElement();
		list->append(new TextMacroCommand(name.text(), icon.text(), text.text()));
		textMacro = textMacro.nextSiblingElement();
	}
	ok = true;
	return list;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLTextMacroCommand::~XMLTextMacroCommand()
{

}
