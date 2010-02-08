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

#define DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H \
	bool deSerializeCommandsPrivate(const QDomElement& elem);

#define DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(x, y) \
	bool x::deSerializeCommandsPrivate(const QDomElement& elem) \
	{ \
		if (elem.isNull()) return false; \
 \
		if (!commands) \
			commands = new CommandList(); \
\
		QDomElement commandElem = elem.firstChildElement("command"); \
		while(!commandElem.isNull()) \
		{ \
			Command *c = y::createInstance(commandElem); \
			if (c) \
				commands->append(c); \
 \
			commandElem = commandElem.nextSiblingElement("command"); \
		} \
		return true; \
	}

#include "simonmodelmanagement_export.h"

#include "simoncommand.h"
#include "command.h"

#include "commandlauncher.h"
#include <simonrecognitionresult/recognitionresult.h>
#include <simonscenariobase/scenarioobject.h>
#include <QList>
#include <QObject>
#include <KIcon>
#include <QAbstractItemModel>
#include <QDomElement>
#include <QHash>



class CommandManager;
class CreateCommandWidget;
class KCModule;
class QAction;
class CommandConfiguration;
class QDomDocument;
class Scenario;
class VoiceInterfaceCommandTemplate;

/**
 *	@class CommandManager
 *	@brief Provides an abstract base-class for the command-managers
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class MODELMANAGEMENT_EXPORT CommandManager : public QAbstractItemModel, public ScenarioObject {
Q_OBJECT
signals:
	void commandsFound(CommandList*);

private:
	int m_currentState;
//	QHash<QString, QString> voiceInterfaceActionNames;
	QList<VoiceInterfaceCommandTemplate*> voiceInterfaceCommandTemplates;

protected:
	CommandList *commands;
	QString m_source;
	QList<QAction*> guiActions;


	CommandConfiguration *config;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const;
	QObject* parent() { return QObject::parent(); }
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void adaptUi();
	virtual bool appendCommand(Command *com);
	virtual bool shouldAcceptCommand(Command *com)
	{ 
		Q_UNUSED(com);
		return false; 
	}

public:
	virtual const QString name() const=0;
	virtual const QString preferredTrigger() const;
	virtual const KIcon icon() const { return KIcon(); }
	bool addCommand(Command *command);
	virtual bool addCommandPrivate(Command *command);

	virtual QList<CommandLauncher*> launchers() const;

	virtual QList<QAction*> getGuiActions() const;

	virtual CommandList* getCommands() const { 
		return commands; 
	}

	bool hasCommands() const {
		return (commands && !commands->isEmpty());
	}

	virtual void setFont(const QFont& font);

	virtual CommandConfiguration* getConfigurationPage();
	virtual CreateCommandWidget* getCreateCommandWidget(QWidget *parent);
	CreateCommandWidget* getCreateVoiceInterfaceCommandWidget(QWidget *parent);

	virtual bool processResult(const RecognitionResult& recognitionResult);

	virtual bool deleteCommand(Command *command);

	virtual bool deSerialize(const QDomElement&);
	virtual QDomElement serialize(QDomDocument *doc);

	virtual bool deSerializeConfig(const QDomElement& elem);
	virtual QDomElement serializeConfig(QDomDocument *doc);

	bool deSerializeCommands(const QDomElement& elem);
	virtual bool deSerializeCommandsPrivate(const QDomElement& elem);

	virtual QDomElement serializeCommands(QDomDocument *doc);

	virtual bool trigger(const QString& triggerName);

	virtual bool installInterfaceCommand(QObject* object, const QString& slot, 
			const QString& actionName, const QString& iconSrc,
			const QString& description, bool announce, bool showIcon=false,
			int state=SimonCommand::DefaultState, 
			int newState=SimonCommand::DefaultState, 
			const QString& defaultVisibleTrigger=QString(), 
			QString id=QString());

	QList<VoiceInterfaceCommandTemplate*> getVoiceInterfaceCommandTemplates()
	{ return voiceInterfaceCommandTemplates; }

	void switchToState(int newState)
	{ m_currentState = newState; }

	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CommandManager(Scenario *parentScenario, const QVariantList& args) : QAbstractItemModel((QObject*) parentScenario),
		ScenarioObject(parentScenario),
		m_currentState(SimonCommand::DefaultState), commands(0), config(0)
	{
		Q_UNUSED(args);
	}

	void setGreedyStatus(bool isGreedy);

	virtual ~CommandManager();
	

};

#endif
