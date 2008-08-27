//
// C++ Interface: desktopgridcommandmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESKTOPGRIDCOMMANDMANAGER_H
#define DESKTOPGRIDCOMMANDMANAGER_H

#include "../commandmanager.h"


/**
 *	@class DesktopGridCommandManager
 *	@brief Manager of the ScreenGrid
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class DesktopGridCommandManager : public CommandManager{
Q_OBJECT
public:
	const QString name() const;
	bool load();
	bool save();
	bool trigger(const QString& triggerName);
	bool addCommand(Command *) { return false; }

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    DesktopGridCommandManager(QObject *parent=0);

    
    ~DesktopGridCommandManager() {}

};

#endif
