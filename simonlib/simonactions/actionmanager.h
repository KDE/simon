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
#include <QList>
#include <commandpluginbase/command.h>
#include "actionlib_export.h"

class CommandManager;
class CreateCommandWidget;
class CommandSettings;
class KCModule;

class SIMONACTIONS_EXPORT ActionManager : public QObject {

Q_OBJECT

signals:
	void commandExecuted(QString trigger);
	void guiAction(QString trigger);
	void commandsChanged(CommandList *commands);

private:
	static ActionManager* instance;

	CommandSettings* commandSettings;

	QList<CommandManager*> *managers;
	QStringList trigger;
	bool askDeleteCommandByTrigger(QString trigger);

	void deleteManager(CommandManager *manager);


protected:
	ActionManager(QObject *parent=0);

private slots:
	void setupBackends(const QStringList& pluginsToLoad);
	void setTrigger(const QStringList& trigger);


public:

	static ActionManager* getInstance() {
		if (!instance) instance = new ActionManager();
		return instance;
	}

	void setConfigurationDialog(KCModule*);
	KCModule* getConfigurationDialog() { return (KCModule*) commandSettings; }
	void init();

	void process(QString input);
	bool addCommand(Command *command);
	bool deleteCommand(Command *command);

	QList<CreateCommandWidget*>* getCreateCommandWidgets(QWidget *parent);

	CommandList* getCommandList();


	~ActionManager();

};



#endif
