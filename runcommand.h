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

#define WINDOWS

#include "command.h"
#include "runbackend.h"

#ifdef LINUX
#include "runlinuxbackend.h"
#else
#include "runwindowsbackend.h"
#endif
#include <QString>
#include <QMessageBox>
#include <iostream>
#include "simoninfo.h"


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
 *	@todo Implementing
 */

class RunCommand{

private:
	CommandList commandlist;
	RunBackend* runner;
	bool desprate;
public:
    RunCommand(QString path="commands.xml");

     /**
     *	@brief Returns the commandlist (member)
     *
     *	@author Peter Grasch
     *	@return CommandList
     */
    CommandList getCommands() { return this->commandlist; }
    void readCommands(QString path);
    void run(QString commandName);

    ~RunCommand();

};

#endif
