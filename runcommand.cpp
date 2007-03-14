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
	this->commandlist.append(new Command("Konqueror", exec, "konqueror"));
	this->commandlist.append(new Command("Firefox", exec, "firefox"));
	this->commandlist.append(new Command("Home", place, "/home/bedahr"));
	this->commandlist.append(new Command("Texteditor", exec, "kwrite"));
	this->commandlist.append(new Command("Google", place, "http://google.at"));
	this->commandlist.append(new Command("Geräte", place, "media:/"));
	this->commandlist.append(new Command("Drucker", place, "print:/"));
	this->commandlist.append(new Command("Suchen nach Test", place, "beagle:test"));
	this->commandlist.append(new Command("Beistrich", type, ","));
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
	while ((i < commandlist.size()) && (commandlist.at(i)->getName() != commandName))
		i++;
	
	if (i == commandlist.size()) return;
	
	if (commandlist.at(i)->getType() == exec)
	{
#ifdef linux
		RunLinuxBackend *lin = (RunLinuxBackend*) this->runner;
		lin->run(commandlist.at(i)->getValue());
#endif
#ifdef __WIN32
		RunWindowsBackend *win = (RunWindowsBackend*) this->runner;
		win->run(commandlist.at(i)->getValue());
#endif
	}
	
	if (commandlist.at(i)->getType() == place)
	{
#ifdef linux
		RunLinuxBackend *lin = (RunLinuxBackend*) this->runner;
		lin->goTo(commandlist.at(i)->getValue());
#endif
#ifdef __WIN32
		RunWindowsBackend *win = (RunWindowsBackend*) this->runner;
		win->goTo(commandlist.at(i)->getValue());
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


