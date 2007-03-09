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
#ifndef COMMAND_H
#define COMMAND_H
#include <QList>
#include <QString>

/**
 *	@class Command
 *	@brief Ressamles one command with all the attributes
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class Command;

/**
 *	@typedef CommandList
 *	QList of Commands
 */
typedef QList<Command*> CommandList;


/**
 *	@enum CommandType
 *	Defines the Commandtypes: exec, place, type
 */
enum CommandType {
	exec=0,
	place=1,
	type=2
};

class Command{

private:
	QString name; //!< The name of the command - this is used to call the command and is unique
	CommandType type; //!< the type of command
	QString value; //!< The command itself

public:
	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    *	@param QString name
    *	@param CommandType type
    *	@param QString value
    */
    Command(QString name, CommandType type, QString value)
    {
	this->name = name;
	this->type = type;
	this->value = value;
    }

    /**
    * @brief Returns the name
    * 
    *	@author Peter Grasch
    *	@return QString
    *	Returns the name of the command (member)
    */
    QString getName() { return this->name; }
    
    /**
    * @brief Returns the Type
    * 
    *	@author Peter Grasch
    *	@return CommandType
    *	Returns the type of the command (member)
    */
    CommandType getType() { return this->type; }
    
    /**
    * @brief Returns the Value
    * 
    *	@author Peter Grasch
    *	@return QString
    *	This returns the actual commandstring (this may be a executable, a specal character or a url)
    */
    QString getValue() {return this->value; }

    ~Command();

};

#endif
