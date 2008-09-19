//
// C++ Interface: command
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QList>
#include <QObject>
#include "command.h"

class CommandManager;

typedef QList<CommandManager*> CommandManagerList;

/**
 *	@class CommandManager
 *	@brief Provides an abstract base-class for the command-managers
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class CommandManager : public QObject{
Q_OBJECT
signals:
	void commandsFound(CommandList*);
protected:
	CommandList *commands;
public:
	virtual const QString name() const=0;
	virtual bool load()=0;
	virtual bool save()=0;
	virtual bool addCommand(Command *command)=0;

	virtual CommandList* getCommands() const { return commands; }

	virtual bool deleteCommand(Command *command);

	virtual bool trigger(const QString& triggerName);
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    CommandManager(QObject *parent=0) : QObject(parent)
    { this->commands = 0;  }

    
    virtual ~CommandManager() {
		if (commands)
			qDeleteAll(*commands);
	}

};

#endif
