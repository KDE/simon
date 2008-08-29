//
// C++ Interface: shortcutcommand
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 20078
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHORTCUTCOMMAND_H
#define SHORTCUTCOMMAND_H

#include <QKeySequence>
#include "../command.h"
#include "../../../SimonLib/EventSimulation/shortcut.h"


/**
 *	@class ShortcutCommand
 *	@brief Ressembles one shortcut-command
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class ShortcutCommand : public Command{
Q_OBJECT

private:
	QKeySequence shortcut;

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;

	bool triggerPrivate();

public:
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;
	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ShortcutCommand(const QString& name, const QString& iconSrc, const QKeySequence &shortcut) : Command(name, iconSrc)
    {
        this->shortcut = shortcut;
    }


	void change(const QString& newName, const QString& newIconSrc, const QKeySequence& newShortcut) { 
		this->shortcut = newShortcut;
		Command::change(newName, newIconSrc);
	}

    /**
    * @brief Returns the shortcut
    * 
    *	@author Peter Grasch
    */
    const QKeySequence getShortcut() const { return this->shortcut; }
    
    
    ~ShortcutCommand() {
    }

};

#endif
