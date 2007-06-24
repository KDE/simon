//
// C++ Implementation: runcommand
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "runcommand.h"
#include "logger.h"

/**
 *	@brief Constructor
 *	
 *	Calls readCommands to initialize the CommandList
 *
 *	@author Peter Grasch
 */
RunCommand::RunCommand(QString path)
{
	#ifdef linux
	this->runner = new RunLinuxBackend();
	#endif
	
	#ifdef __WIN32
	this->runner = new RunWindowsBackend();
	#endif
	
	readCommands(path);
}

/**
 *	@brief Reads the Commands from the given file
 *
 *	@param QString path
 *	The path of the file which contains the commands
 *	@author Peter Grasch
 *	@note Waiting for the XML Reader before implementing any function
 */
void RunCommand::readCommands(QString path)
{
	Logger::log("Reading commands from "+path);
	XMLCommand *reader = new XMLCommand();
	reader->load();
	
	this->commandlist = reader->getCommands();
}

/**
 *	@brief Executes the given Commando
 *	
 *	Expects the name of a commando.
 *	This commando is then looked up in the commandlist (member)
 *
 *	@param QString name
 *	Name of the command
 *	@author Peter Grasch
 */
void RunCommand::run(QString commandName)
{
	//execute the command
	int i=0;
	while ((i < commandlist.size()) && (commandlist.at(i)->getName() != commandName.trimmed()))
		i++;
	
	if (i == commandlist.size()) return;
	
	QString command = commandlist.at(i)->getValue();
	
	if (commandlist.at(i)->getType() == exec)
	{
		Logger::log("Executing command: "+command);
#ifdef linux
		RunLinuxBackend *lin = (RunLinuxBackend*) this->runner;
		lin->run(command);
#endif
#ifdef __WIN32
		RunWindowsBackend *win = (RunWindowsBackend*) this->runner;
		win->run(command);
#endif
	}
	
	if (commandlist.at(i)->getType() == place)
	{
		Logger::log("Opening place: "+command);
#ifdef linux
		RunLinuxBackend *lin = (RunLinuxBackend*) this->runner;
		lin->goTo(command);
#endif
#ifdef __WIN32
		RunWindowsBackend *win = (RunWindowsBackend*) this->runner;
		win->goTo(command);
#endif
	}
}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunCommand::~RunCommand()
{
}


