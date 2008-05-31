/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H


#include <QObject>
#include "Commands/command.h"
#include "Commands/commandmanager.h"

class EventHandler;

class ActionManager : public QObject {

Q_OBJECT

signals:
	void commandExecuted(QString trigger);
	void guiAction(QString trigger);
	void commandsChanged(CommandList *commands);

private:
	static ActionManager* instance;
	
	EventHandler *eventHandler; //to type (fallback)

	CommandManagerList *managers;
	bool askDeleteCommandByTrigger(QString trigger);



protected:
	ActionManager(QObject *parent=0);

public:

	static ActionManager* getInstance() {
		if (!instance) instance = new ActionManager();
		return instance;
	}

	void process(QString input);
	bool addCommand(Command *command);
	bool deleteCommand(Command *command);

	CommandList* getCommandList();

	void setupBackends();

	~ActionManager();

};



#endif
