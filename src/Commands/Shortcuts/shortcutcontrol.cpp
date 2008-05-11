//
// C++ Implementation: shortcutcontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shortcutcontrol.h"
#include "shortcut.h"
#include <QMessageBox>
#include "shortcutxmlreader.h"
#include "settings.h"

ShortcutControl* ShortcutControl::instance;

/**
 * \brief Constructor
 * \author Peter Grasch
 */
ShortcutControl::ShortcutControl()
{
	shortcutXMLReader=new ShortcutXMLReader(Settings::getS("PathToShortcuts"));
	if (!readShortcuts())
		QMessageBox::critical(0, "Dateifehler", 
			"Beim auslesen der Tastenkürzel ist ein Fehler aufgetreten.");
}

/**
 * \brief Reads the shortcuts from the xml file utilizing the shortcutxmlreader
 * \author Peter Grasch
 * @return success
 */
bool ShortcutControl::readShortcuts()
{
	bool ok = shortcutXMLReader->load();
	if (!ok) return false;
	
	shortcuts = shortcutXMLReader->getShortcuts();
	return true;
}

/**
 * \brief Saves the list of shortcuts to the XML file
 * \author Peter Grasch
 */
void ShortcutControl::save()
{
	shortcutXMLReader->setShortcuts(this->shortcuts);
	shortcutXMLReader->save();
}

/**
 * \brief Clears the list of shortcuts
 * \author Peter Grasch
 * \author Peter Grasch
 */
void ShortcutControl::clearShortcutList()
{
	if (!shortcuts) return;
	
	shortcuts->clear();
}

/**
 * \brief Sets the shortcuts (member) to the given list of shortcuts
 * \author Peter Grasch
 * @param newShortcuts The shortcuts to point to
 */
void ShortcutControl::setShortcuts(ShortcutList *newShortcuts)
{
	this->shortcuts = newShortcuts;
}

/**
 * \brief Returns the shortcuts
 * \author Peter Grasch
 * @return Returns a pointer to the list of shortcuts
 */
ShortcutList* ShortcutControl::getShortcuts()
{
	return shortcuts;
}

/**
 * \brief Returns a pointer to the shortcut mathing the given trigger
 * \author Peter Grasch
 * @param trigger The trigger to look for
 * @return The shortcut if found - else NULL
 */
Shortcut* ShortcutControl::getShortcut(QString trigger)
{
	if (!shortcuts) return  NULL;
	
	int i=0;
	while ((i < shortcuts->count()) && (shortcuts->at(i)->getName() != trigger))
	{ i++; }

	if (i!= shortcuts->count())
		return shortcuts->at(i);
	else return NULL;
}

/**
 * \brief Checks if the given name exists and returns true if it does
 * \author Peter Grasch
 * @param name The name to look for
 * @return True if we found the shortcut
 */
bool ShortcutControl::nameExists(QString name)
{
	int i=0;
	if (!shortcuts) return false;

	while ((i<shortcuts->count()) && (shortcuts->at(i)->getName() != name)) {i++;}
	
	if (i != shortcuts->count())
		return true;
	
	return false;
}

/**
 * \brief Inserts the given shortcut
 * \author Peter Grasch
 * @param shortcut The shortcut to insert
 * @return Success
 */
bool ShortcutControl::insertShortcut(Shortcut *shortcut)
{
	if (!shortcuts) return false;

	if (nameExists(shortcut->getName())) return false;
	
	shortcuts->append(shortcut);
	return true;
}

/**
 * \brief Removes the given shortcut from the list
 * \author Peter Grasch
 * @param shortcut The shortcut to remove
 * @return The position from where we removed the shortcut (or -1 if not found)
 */
int ShortcutControl::removeShortcut(Shortcut shortcut)
{
	if (!shortcuts) return -1;
	
	for (int i=0; i < shortcuts->count(); i++)
	{
		if ((*(shortcuts->at(i)))== shortcut) 
		{
			shortcuts->removeAt(i);
			return i;
		}
	}
	return -1;
}

/**
 * \brief Replaces the given oldShortcut with the given newShortcut
 * @param oldShortcut The old shortcut to replace
 * @param newShortcut The shortcut we want to insert
 * @return Success
 */
bool ShortcutControl::replaceShortcut(Shortcut oldShortcut, Shortcut newShortcut)
{
	int removedPos = removeShortcut(oldShortcut);
	if (removedPos ==-1) return false;
	
	if (nameExists(newShortcut.getName())) return false;
	
	shortcuts->insert(removedPos, &newShortcut);
	return true;
}

ShortcutControl::~ShortcutControl()
{
    shortcutXMLReader->deleteLater();
    delete shortcuts;
}

