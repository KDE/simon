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

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "simoncommandpluginbase_export.h"

#include "command.h"
#include <QList>
#include <QObject>

class CommandManager;
class CreateCommandWidget;
class KCModule;
class CommandConfiguration;

/**
 *	@class CommandManager
 *	@brief Provides an abstract base-class for the command-managers
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class SIMONCOMMANDPLUGINBASE_EXPORT CommandManager : public QObject{
Q_OBJECT
signals:
	void commandsFound(CommandList*);

protected:
	CommandList *commands;

public:
	virtual const QString name() const=0;
	virtual bool load()=0;
	virtual bool save()=0;
	virtual bool addCommand(Command *command)=0;

	virtual CommandList* getCommands() const { return commands; }

	virtual bool deleteCommand(Command *command);

	virtual CommandConfiguration* getConfigurationPage();
	virtual CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

	virtual bool trigger(const QString& triggerName);
	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CommandManager(QObject *parent, const QVariantList& args) : QObject(parent)
	{
		Q_UNUSED(args);
		commands=0;
	}


	virtual ~CommandManager();

};

#endif
