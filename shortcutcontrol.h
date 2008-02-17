//
// C++ Interface: shortcutcontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHORTCUTCONTROL_H
#define SHORTCUTCONTROL_H

#include "shortcut.h"

class ShortcutXMLReader;
/**
 * \class ShortcutControl
 * \brief Manages the shortcuts
 * \author Peter Grasch
 * \version 0.1
 * \date 28.8.2007
 */
class ShortcutControl{
private:
	ShortcutList *shortcuts;
	ShortcutXMLReader *shortcutXMLReader;
	static ShortcutControl *instance;
public:
	ShortcutControl();
	static ShortcutControl* getInstance()
	{
		if (!instance) instance = new ShortcutControl();
		return instance;
	}

	bool readShortcuts();
	void clearShortcutList();
	void save();
	
	void setShortcuts(ShortcutList *newShortcuts);
	
	ShortcutList* getShortcuts();
	
	Shortcut* getShortcut(QString trigger);
	
	bool nameExists(QString name);
	bool insertShortcut(Shortcut *shortcut);
	int removeShortcut(Shortcut shortcut);
	bool replaceShortcut(Shortcut oldShortcut, Shortcut newShortcut);
	
	~ShortcutControl() { }

};

#endif
