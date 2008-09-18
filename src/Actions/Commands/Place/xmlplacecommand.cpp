#include "xmlplacecommand.h"
#include "placecommand.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>

/**
 * \brief Creates a new XMLPlaceCommand object
 * \author Peter Grasch
 * \param QString path
 * Path to the commands.xml; Default: conf/commands.xml
 */
XMLPlaceCommand::XMLPlaceCommand(const QString &path):XMLDomReader(path)
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
bool XMLPlaceCommand::save(const CommandList *list, const QString& path)
{
	if (this->doc)
		this->doc->clear();
	else doc = new QDomDocument ();

	QDomElement root = doc->createElement("commands");
	doc->appendChild(root);
	
	for (int i=0; i < list->size(); i++)
	{
		PlaceCommand *com = dynamic_cast<PlaceCommand*>(list->at(i));
		if (!com) continue;
		
		QDomElement command = doc->createElement("place");
		QDomElement name = doc->createElement("name");
		name.appendChild(doc->createTextNode(com->getTrigger()));
		QDomElement icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(com->getIconSrc()));
		QDomElement url = doc->createElement("url");
		url.appendChild(doc->createTextNode(com->getURL().pathOrUrl()));
		
		command.appendChild(name);
		command.appendChild(icon);
		command.appendChild(url);
		
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
CommandList* XMLPlaceCommand::load(bool &ok, const QString& path)
{
	if (!XMLDomReader::load(path) || !this->doc)
	{
		ok = false;
		return 0;
	}

	CommandList *list = new CommandList();

	QDomElement root = this->doc->documentElement();
	QDomElement place = root.firstChildElement();
	while(!place.isNull())
	{
		QDomElement name = place.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement url = icon.nextSiblingElement();
		list->append(new PlaceCommand(name.text(), icon.text(), url.text()));
		place = place.nextSiblingElement();
	}
	ok = true;
	return list;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLPlaceCommand::~XMLPlaceCommand()
{

}
