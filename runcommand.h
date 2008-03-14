//
// C++ Interface: runcommand
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RUNCOMMAND_H
#define RUNCOMMAND_H

#include "command.h"

class RunBackend;

/**
 *	@class RunCommand
 *	@brief Runs commands
 *
 *	Uses the defined set of commands (defined in the commands.xml) to determine the
 *	reaction to a definite command string
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */

class RunCommand{

private:
	CommandList commandlist;
	RunBackend* runner;
	QString filename;
	bool desprate;
	static RunCommand *instance;
public:
    RunCommand();

	static RunCommand* getInstance()
	{
		if (!instance) instance = new RunCommand();
		return instance;
	}
     /**
     *	@brief Returns the commandlist (member)
     *
     *	@author Peter Grasch
     *	@return CommandList
     */
    CommandList getCommands() { return this->commandlist; }
    bool readCommands(QString path);

	void replaceCommand(QString commandName, Command *newCommand);
	void addCommand(Command *newCommand);
	void deleteCommand(QString commandName);
	bool commandExists(QString commandName);

	bool init(QString path);
	bool reload(QString path="");
	bool save(QString path="");

    bool run(QString commandName);

    ~RunCommand();

};

#endif
