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
#ifndef PLACECOMMANDMANAGER_H
#define PLACECOMMANDMANAGER_H

#include "../commandmanager.h"

class XMLPlaceCommand;
/**
 *	@class PlaceCommandManager
 *	@brief Manager for the place-commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class PlaceCommandManager : public CommandManager{
Q_OBJECT
private:
	XMLPlaceCommand* xmlPlaceCommand;
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
    PlaceCommandManager(QObject *parent=0);

    
    ~PlaceCommandManager();

};

#endif
