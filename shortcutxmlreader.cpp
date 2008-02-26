//
// C++ Implementation: shortcutxmlreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shortcutxmlreader.h"
#include <QDomDocument>
/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path The path to write to / read from
 * @param parent Parent of the object
 */
ShortcutXMLReader::ShortcutXMLReader(QString path, QObject* parent): XMLDomReader(path, parent)
{
	this->path=path;
	shortcuts=new ShortcutList();
}


/**
 * \brief Loads the XML file from the given path
 * \author Peter Grasch
 * 
 * If no path is given we try to use the member variable;
 * 
 * The function tries to read the given file and parse a ShortcutList out of it.
 * This list will then be made accessibly through the getShortcuts() method
 * 
 * @param path The path to read from
 * @return success
 */
bool ShortcutXMLReader::load(QString path)
{
	if (path.isEmpty()) path = this->path;

	XMLDomReader::load(path);
	
	bool allFine=true;
	bool errorOccured=false;

	QDomNodeList shortcuts = doc->elementsByTagName("shortcut");

	ShortcutList *shortcutList=new ShortcutList();

	for (int i=0; i < shortcuts.count(); i++)
	{
		allFine=true;
		QDomNode shortcut = shortcuts.at(i);
		QString name = shortcut.toElement().attribute("name");
		int modifiers = shortcut.firstChildElement("modifiers").text().toInt(&allFine);
		int actionkeys = shortcut.firstChildElement("actionkeys").text().toInt(&allFine);
		int functionkeys = shortcut.firstChildElement("functionkeys").text().toInt(&allFine);
		int movementkeys = shortcut.firstChildElement("movementkeys").text().toInt(&allFine);

		QByteArray chars = shortcut.firstChildElement("charkey").text().toLatin1();
		char charKey='_';
		if (chars.count()>0)
			charKey = chars.constData()[0];

		if (allFine)
			shortcutList->append(new Shortcut(name, modifiers, actionkeys, 
						functionkeys, movementkeys, charKey));
		else errorOccured = true;
	}
	this->shortcuts = shortcutList;
	return !errorOccured;
}

/**
 * \brief Returns the read list of shortcuts
 * \note This may be NULL if we didn't yet read the shortcuts from the file or were set to NULL by setShortcuts()
 * \author Peter Grasch
 * @return The ShortcutList
 */
ShortcutList* ShortcutXMLReader::getShortcuts()
{
	return shortcuts;
}


/**
 * \brief Sets the list of shortcuts to the given list
 * \author Peter Grasch
 * @param shortcuts The shortcuts to set to
 */
void ShortcutXMLReader::setShortcuts(ShortcutList *shortcuts)
{
	if (this->shortcuts) {
		delete this->shortcuts;
		this->shortcuts = 0;
	}
	this->shortcuts = new ShortcutList(*shortcuts);
}


/**
 * \brief Saves the list to the given path (or the member-path if the given one is empty)
 * 
 * You might want to set the list to save with the setShortcuts(...) function first
 * 
 * @param path The path to save to
 * @return Success
 */
bool ShortcutXMLReader::save(QString path)
{
	if (path.isEmpty()) path = this->path;
	if (!shortcuts) return false;
	
	if (doc) {
		delete doc;
		doc = 0;
	}

	doc = new QDomDocument();
	
	QDomElement root = doc->createElement("shortcuts");

	for (int i=0; i < this->shortcuts->count(); i++)
	{
		
		QDomElement shortcut = doc->createElement("shortcut");
		Shortcut *currentSC = shortcuts->at(i);
		shortcut.setAttribute("name", currentSC->getName());

		QDomNode modifier = doc->createElement("modifiers");
		modifier.appendChild(doc->createTextNode(QString::number(currentSC->getModifiers())));
		shortcut.appendChild(modifier);

		QDomNode actionkeys = doc->createElement("actionkeys");
		actionkeys.appendChild(doc->createTextNode(QString::number(currentSC->getActionKeys())));
		shortcut.appendChild(actionkeys);
		
		QDomNode functionkeys = doc->createElement("functionkeys");
		functionkeys.appendChild(doc->createTextNode(QString::number(currentSC->getFunctionKeys())));
		shortcut.appendChild(functionkeys);
		
		QDomNode movementkeys = doc->createElement("movementkeys");
		movementkeys.appendChild(doc->createTextNode(QString::number(currentSC->getMovementKeys())));
		shortcut.appendChild(movementkeys);
		
		QDomNode charkey = doc->createElement("charkey");
		charkey.appendChild(doc->createTextNode(QChar(currentSC->getCharKey())));
		shortcut.appendChild(charkey);

		root.appendChild(shortcut);
	}

	
	doc->appendChild(root);
	XMLDomReader::save(path);
	return true;
}

ShortcutXMLReader::~ShortcutXMLReader()
{
    if (shortcuts) delete shortcuts;
}
