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
#ifdef linux
#include "runlinuxbackend.h"
#endif
#ifdef __WIN32
#include "runwindowsbackend.h"
#endif
#include "xmlcommand.h"

RunCommand* RunCommand::instance;
/**
 *	@brief Constructor
 *	
 *	Calls readCommands to initialize the CommandList
 *
 *	@author Peter Grasch
 */
RunCommand::RunCommand()
{
	#ifdef linux
	this->runner = new RunLinuxBackend();
	#endif
	
	#ifdef __WIN32
	this->runner = new RunWindowsBackend();
	#endif
}

/**
 * \brief Reads the config file if, and ONLY if, there are no commands yet (kind of a read-once)
 * @return 
 */
bool RunCommand::init(QString path)
{
	if (commandlist.isEmpty())
		return readCommands(path);
	else return true;
}

bool RunCommand::save(QString path)
{
	if (path.isEmpty()) path = filename;
	Logger::log(QObject::tr("[INF] Schreibe Befehle nach ")+path);
	XMLCommand *saver = new XMLCommand(path);
	saver->setCommands(commandlist);
	if (!saver->save(path))
	{
		Logger::log(QObject::tr("[ERR] Konnte Befehle nicht schreiben"));
		QMessageBox::critical(0, QObject::tr("Konnte Kommandos nicht speichern"), 
				QObject::tr("Konnte Kommandos nicht nach %1 speichern").arg(path));
		delete saver;
		return false;
	}
	delete saver;
	return true;
}

/**
 *	@brief Reads the Commands from the given file
 *
 *	@param QString path
 *	The path of the file which contains the commands
 *	@author Peter Grasch
 */
bool RunCommand::readCommands(QString path)
{
	Logger::log(QObject::tr("[INF] Lese Befehle von ")+path);
	this->filename = path;
	XMLCommand *reader = new XMLCommand(path);
	if (!reader->load(path))
	{
		Logger::log(QObject::tr("[ERR] Konnte Befehle nicht öffnen"));
		QMessageBox::critical(0, QObject::tr("Konnte Kommandos nicht laden"), QObject::tr("Konnte Kommandos vom konfigurierten Pfad %1 nicht einlesen").arg(path));
		delete reader;
		return false;
	}
	this->commandlist = reader->getCommands();

	delete reader;
	return true;
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
 *	@return True if we executed something
 */
bool RunCommand::run(QString commandName)
{
	//execute the command
	int i=0;
	while ((i < commandlist.size()) && (commandlist.at(i)->getName() != commandName.trimmed()))
		i++;
	
	if (i == commandlist.size()) return false;
	
	QString command = commandlist.at(i)->getValue();
	QString workingDir = commandlist.at(i)->getWorkingDirectory();
	
	if (commandlist.at(i)->getType() == exec)
	{
		Logger::log(QObject::tr("[INF] Ausführen des Befehls: ")+command);
#ifdef linux
		RunLinuxBackend *lin = (RunLinuxBackend*) this->runner;
		lin->run(command, workingDir);
#endif
#ifdef __WIN32
		RunWindowsBackend *win = (RunWindowsBackend*) this->runner;
		win->run(command, workingDir);
#endif
	}
	
	if (commandlist.at(i)->getType() == place)
	{
		Logger::log(QObject::tr("[INF] öffne Ort: ")+command);
#ifdef linux
		RunLinuxBackend *lin = (RunLinuxBackend*) this->runner;
		lin->goTo(command);
#endif
#ifdef __WIN32
		RunWindowsBackend *win = (RunWindowsBackend*) this->runner;
		win->goTo(command);
#endif
	}
	return true;
}





/**
 * \brief Replaces a command of the commandlist (member) identified by <commandName> with the new command supplied by <newCommand>
 * \author Susanne Tschernegg
 * @param QString commandName
 *      holds the name of the command
 * @param Command *newCommand
 *      is an object of a command
 */
void RunCommand::replaceCommand(QString commandName, Command *newCommand)
{
	for (int i=0; i < commandlist.size(); i++)
	{
		if(commandlist.at(i)->getName()==commandName)
		{
			commandlist.replace(i, newCommand);
		}
	}
}

/**
 * \brief Adds a command to the commandlist (member)
 * \author Susanne Tschernegg
 * @param Command *newCommand
 *      is an object of a command
 */
void RunCommand::addCommand(Command *newCommand)
{
	commandlist.append(newCommand);
}

/**
 * \brief deletes a command from the commandlist.
 * \author Susanne Tschernegg
 * @param QString commandName
 *      is an object of a command
 */
void RunCommand::deleteCommand(QString commandName)//, QString commandValue)
{
	for (int i=0; i < commandlist.size(); i++)
	{
		if(commandlist.at(i)->getName()==commandName)
		{
			    commandlist.removeAt(i);
		}
	}
}

/**
 * \brief returns wheater the command exists or not
 * \author Susanne Tschernegg
 * @param QString commandName
 *      holds the name of the command
 * @return bool
 *      if the command exists, this method will return true, otherwise it returns false
 */
bool RunCommand::commandExists(QString commandName)
{
	for (int i=0; i < commandlist.size(); i++)
	{
		if(commandlist.at(i)->getName()==commandName)
		{
			return true;
		}
	}
	return false;
}



/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunCommand::~RunCommand()
{
    delete runner;
}


