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
#include <QDomDocument>
#include "xmlshortcutcommand.h"
#include "shortcutcommand.h"
#include "../../../SimonLib/EventSimulation/shortcut.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path The path to write to / read from
 * @param parent Parent of the object
 */
XMLShortcutCommand::XMLShortcutCommand(QString path, QObject* parent): XMLDomReader(path, parent)
{
}



/**
 * \brief Loads the XML file from the given path
 * \author Peter Grasch
 * 
 * If no path is given we try to use the member variable;
 * 
 * @param path The path to read from
 * @param ok Reference if everything went ok
 * @return the read list
 */
CommandList* XMLShortcutCommand::load(bool &ok, QString path)
{
	if (path.isEmpty()) path = this->path;

	XMLDomReader::load(path);

	ok = true;

	CommandList *list = new CommandList();

	bool allFine=true;
	bool tempFine=true;

	QDomNode shortcut = doc->documentElement().firstChildElement();

	while (!shortcut.isNull())
	{
		allFine=true;
		

		QDomElement name = shortcut.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement modifiers = icon.nextSiblingElement();
		QDomElement actionkeys = modifiers.nextSiblingElement();
		QDomElement functionkeys = actionkeys.nextSiblingElement();
		QDomElement movementkeys = functionkeys.nextSiblingElement();
		QDomElement charKey = movementkeys.nextSiblingElement();

		QString nameVal = name.text();
		QString iconVal = icon.text();
		int modifiersVal = modifiers.text().toInt(&tempFine);
		allFine = allFine && tempFine;
		int actionKeysVal = actionkeys.text().toInt(&tempFine);
		allFine = allFine && tempFine;
		int functionKeysVal = functionkeys.text().toInt(&tempFine);
		allFine = allFine && tempFine;
		int movementKeysVal = movementkeys.text().toInt(&tempFine);
		allFine = allFine && tempFine;

		QByteArray chars = charKey.text().toLatin1();
		char charKeyVal='_';
		if (chars.count()>0)
			charKeyVal = chars.constData()[0];

		if (allFine)
			list->append(new ShortcutCommand(nameVal, iconVal, new Shortcut(modifiersVal, actionKeysVal, 
						functionKeysVal, movementKeysVal, charKeyVal)));
		else ok = false;

		shortcut = shortcut.nextSiblingElement();
	}
	return list;
}


/**
 * \brief Saves the list to the given path (or the member-path if the given one is empty)
 * 
 * You might want to set the list to save with the setShortcuts(...) function first
 * 
 * @param path The path to save to
 * @return Success
 */
bool XMLShortcutCommand::save(CommandList *commands, QString path)
{
	if (path.isEmpty()) path = this->path;
	if (path.isEmpty()) return false;
	if (!commands) return false;
	
	doc->clear();
	
	QDomElement root = doc->createElement("commands");

	for (int i=0; i < commands->count(); i++)
	{
		ShortcutCommand *currentSC = dynamic_cast<ShortcutCommand*>(commands->at(i));
		if(!currentSC) continue;
		const Shortcut *currentShortcut = currentSC->getShortcut();
		if (!currentShortcut) continue;
		
		QDomElement shortcut = doc->createElement("shortcut");
		
		
		QDomNode name = doc->createElement("name");
		name.appendChild(doc->createTextNode(currentSC->getTrigger()));
		shortcut.appendChild(name);
		
		QDomNode icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(currentSC->getIconSrc()));
		shortcut.appendChild(icon);

		QDomNode modifier = doc->createElement("modifiers");
		modifier.appendChild(doc->createTextNode(QString::number(currentShortcut->getModifiers())));
		shortcut.appendChild(modifier);

		QDomNode actionkeys = doc->createElement("actionkeys");
		actionkeys.appendChild(doc->createTextNode(QString::number(currentShortcut->getActionKeys())));
		shortcut.appendChild(actionkeys);
		
		QDomNode functionkeys = doc->createElement("functionkeys");
		functionkeys.appendChild(doc->createTextNode(QString::number(currentShortcut->getFunctionKeys())));
		shortcut.appendChild(functionkeys);
		
		QDomNode movementkeys = doc->createElement("movementkeys");
		movementkeys.appendChild(doc->createTextNode(QString::number(currentShortcut->getMovementKeys())));
		shortcut.appendChild(movementkeys);
		
		QDomNode charkey = doc->createElement("charkey");
		charkey.appendChild(doc->createTextNode(QChar(currentShortcut->getCharKey())));
		shortcut.appendChild(charkey);

		root.appendChild(shortcut);
	}

	doc->appendChild(root);
	XMLDomReader::save(path);
	return true;
}

XMLShortcutCommand::~XMLShortcutCommand()
{
}
