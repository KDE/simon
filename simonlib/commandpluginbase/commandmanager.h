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
#include <simonrecognitionresult/recognitionresult.h>
#include <QList>
#include <QObject>
#include <KIcon>
#include <KDebug>

class CommandManager;
class CreateCommandWidget;
class KCModule;
class QAction;
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
	QList<QAction*> guiActions;
	CommandList *commands;
	virtual bool trigger(const QString& triggerName);

public:
	virtual const QString name() const=0;
	virtual const QString preferredTrigger() const;
	virtual const KIcon icon() const { return KIcon(); }
	virtual bool load()=0;
	virtual bool save()=0;
	virtual bool addCommand(Command *command)=0;

	virtual QList<QAction*> getGuiActions();

	virtual CommandList* getCommands() const { 
		kWarning() << "Getting commands...";
		return commands; 
	}

	virtual CommandConfiguration* getConfigurationPage();
	virtual CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

	virtual bool processResult(const RecognitionResult& recognitionResult);

	virtual bool deleteCommand(Command *command);

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
