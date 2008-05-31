//
// C++ Interface: shortcutcommandmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHORTCUTCOMMANDMANAGER_H
#define SHORTCUTCOMMANDMANAGER_H

#include "../commandmanager.h"


class XMLShortcutCommand;
/**
 *	@class ShortcutCommandManager 
 *	@brief Manager for the executable commands
 *
 *	@version 0.1
 *	@date 22.05.2008
 *	@author Peter Grasch
 */
class ShortcutCommandManager : public CommandManager {

private:
	XMLShortcutCommand* xmlShortcutCommand;
public:
	const QString name() const;
	bool load();
	bool save();
	bool addCommand(Command *command);

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ShortcutCommandManager (QObject *parent=0);

    
    ~ShortcutCommandManager ();

};

#endif
