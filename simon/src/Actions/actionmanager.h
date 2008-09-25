/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
