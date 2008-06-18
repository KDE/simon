//
// C++ Interface: ExecutableCommandManager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EXECUTABLECOMMANDMANAGER_H
#define EXECUTABLECOMMANDMANAGER_H

#include "../commandmanager.h"


class XMLExecutableCommand;
/**
 *	@class ExecutableCommandManager
 *	@brief Manager for the executable commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class ExecutableCommandManager : public CommandManager {
Q_OBJECT
private:
	XMLExecutableCommand* xmlExecutableCommand;
public:
	bool addCommand(Command *command);
	const QString name() const;
	bool load();
	bool save();

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    ExecutableCommandManager(QObject *parent=0);

    
    ~ExecutableCommandManager();

};

#endif
