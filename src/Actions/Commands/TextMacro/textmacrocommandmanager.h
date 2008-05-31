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
#ifndef TEXTMACROCOMMANDMANAGER_H
#define TEXTMACROCOMMANDMANAGER_H

#include "../commandmanager.h"

class XMLTextMacroCommand;
/**
 *	@class PlaceCommandManager
 *	@brief Manager for the place-commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class TextMacroCommandManager : public CommandManager{

private:
	XMLTextMacroCommand* xmlTextMacroCommand;
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
    TextMacroCommandManager(QObject *parent=0);

    
    ~TextMacroCommandManager();

};

#endif
