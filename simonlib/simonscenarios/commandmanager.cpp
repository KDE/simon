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

#include "commandmanager.h"
#include "voiceinterfacecommand.h"
#include "voiceinterfacecommandtemplate.h"
#include "createvoiceinterfacecommandwidget.h"
#include "commandconfiguration.h"
#include "voiceinterfacecommand.h"
#include <KLocalizedString>
#include <simonscenarios/scenario.h>
#include <QAction>
#include <QDomElement>
#include <QDomDocument>

bool CommandManager::trigger(const QString& triggerName)
{
	if (!commands) return false;

	kDebug() << "Matching trigger: " << triggerName;
	foreach (Command* c, *commands)
	{
		if (c->matches(m_currentState, triggerName))
			return c->trigger();
	}

	return false;
}

bool CommandManager::addCommand(Command *command)
{
	VoiceInterfaceCommand *c = dynamic_cast<VoiceInterfaceCommand*>(command);
	if (c) {
		if (!commands)
			commands = new CommandList();
		beginInsertRows(QModelIndex(), commands->count(), commands->count());
		*commands << c;
		endInsertRows();
		adaptUi();
		return parentScenario->save();
	}

	return addCommandPrivate(command);
} 

bool CommandManager::addCommandPrivate(Command *command)
{
	Q_UNUSED(command);
	return false;
}

bool CommandManager::installInterfaceCommand(QObject* object, const QString& slot, 
		const QString& actionName, const QString& iconSrc,
		const QString& description, int state, QString id)
{
	Q_ASSERT(object);

	if (id.isEmpty())
		id = object->objectName();

	if (id.isEmpty())
		return false;
	
	//make id unique
	bool unique;
	{
		unique = true;
		foreach (VoiceInterfaceCommandTemplate *t, voiceInterfaceCommandTemplates) 
		{
			if (t->id() == id)
			{
				unique = false;
				break;
			}
		}
		if (!unique)
			id += "_";
	} while (!unique);

	VoiceInterfaceCommandTemplate *templ = new VoiceInterfaceCommandTemplate(id, actionName, iconSrc, description, state);
	templ->assignAction(object, slot);
	voiceInterfaceCommandTemplates.append(templ);
				
	if (commands)
	{
		foreach (Command *c, *commands)
		{
			VoiceInterfaceCommand *iC = dynamic_cast<VoiceInterfaceCommand*>(c);
			if (!iC) continue;
			if (iC->id() == id)
				iC->assignAction(this, object, slot);
		}
	}
	return true;
}

CreateCommandWidget* CommandManager::getCreateVoiceInterfaceCommandWidget(QWidget *parent)
{
	if (voiceInterfaceCommandTemplates.isEmpty())
		return NULL; //no voice interface actions

	return new CreateVoiceInterfaceCommandWidget(this, parent);
}

void CommandManager::setFont(const QFont& font)
{
	//reimplement this when you use graphical widgets in your
	//command manager
	Q_UNUSED(font);
}
	
bool CommandManager::deSerializeConfig(const QDomElement& elem)
{
	Q_UNUSED(elem);
	return true;
}

QDomElement CommandManager::serializeConfig(QDomDocument *doc)
{
	if (config) 
		return config->serialize(doc);
	
	return doc->createElement("config");
}

QDomElement CommandManager::serializeCommands(QDomDocument *doc)
{
	QDomElement commandsElem = doc->createElement("commands");
	if (commands) {
		foreach (Command *c, *commands)
		{
			QDomElement commandElem = c->serialize(doc);
			if (dynamic_cast<VoiceInterfaceCommand*>(c))
				commandElem.setTagName("voiceInterfaceCommand");
			commandsElem.appendChild(commandElem);
		}
	}

	return commandsElem;
}

bool CommandManager::deSerializeCommands(const QDomElement& elem)
{
	if (elem.isNull())
	{
		//load defaults
		bool childSucc = deSerializeCommandsPrivate(elem);

		if (!voiceInterfaceCommandTemplates.isEmpty())
			if (!commands)
				commands = new CommandList();

		foreach (VoiceInterfaceCommandTemplate *tem, voiceInterfaceCommandTemplates)
		{
			VoiceInterfaceCommand *com = new VoiceInterfaceCommand(this, tem);
			com->assignAction(this, tem->receiver(), tem->slot());
			*commands << com;
		}

		return childSucc;
	}

	///////////// end defaults

	QDomElement command = elem.firstChildElement("voiceInterfaceCommand");

	if (commands)
		qDeleteAll(*commands);

	commands = NULL;

	if (!command.isNull()) //we need commands
		commands = new CommandList();

	while (!command.isNull())
	{
		VoiceInterfaceCommand *com = VoiceInterfaceCommand::createInstance(command);
		command = command.nextSiblingElement("voiceInterfaceCommand");

		if (!com) continue;

		foreach (VoiceInterfaceCommandTemplate *tem, voiceInterfaceCommandTemplates)
		{
			if (tem->id() == com->id())
			{
				com->assignAction(this, tem->receiver(), tem->slot());
				break;
			}
		}
		*commands << com;
	}

	return deSerializeCommandsPrivate(elem);
}

bool CommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{
	Q_UNUSED(elem);
	return true;
}

QList<QAction*> CommandManager::getGuiActions() const
{
	return guiActions;
}

QList<CommandLauncher*> CommandManager::launchers() const
{
	return QList<CommandLauncher*>();
}

bool CommandManager::processResult(const RecognitionResult& recognitionResult)
{
	return trigger(recognitionResult.sentence());
}

/**
 * \brief Returns the CreateCommandWidget used for configuring a new/existing command
 * \author Peter Grasch
 * 
 * If you want your command to be add-able (the user can add a new command of the the type of your plugin)
 * you must override this method and provide your own CreateCommandWidget.
 * 
 * See the CreateCommandWidget documentation for details.
 * 
 * The default implementation returns NULL.
 */
CreateCommandWidget* CommandManager::getCreateCommandWidget(QWidget *parent)
{
	Q_UNUSED(parent);

	return 0;
}


/**
 * \brief Returns the configuration page of the plugin
 * \author Peter Grasch
 */
CommandConfiguration* CommandManager::getConfigurationPage()
{
	return config;
}

const QString CommandManager::preferredTrigger() const
{
	return i18n("Computer");
}

bool CommandManager::deleteCommand(Command *command)
{
	if (!commands) return false;

	for (int i=0; i < commands->count(); i++) {
		if (commands->at(i) == command) {
			beginRemoveRows(QModelIndex(), i, i);
			commands->removeAt(i);
			endRemoveRows();

			if (dynamic_cast<VoiceInterfaceCommand*>(command))
				adaptUi();

			delete command;
			return parentScenario->save();
		}
	}

	return false;
}

void CommandManager::adaptUi()
{
	if (!commands) return;

	QHash<QObject* /*receiver*/, QStringList /*triggers*/> voiceCommands;

	foreach (Command *c, *commands)
	{
		VoiceInterfaceCommand *com = dynamic_cast<VoiceInterfaceCommand*>(c);
		if (!com) continue;

		if (!voiceCommands.contains(com->receiver()))
		{
			if (com->showIcon())
				com->receiver()->setProperty("icon",  com->getIcon());
			else 
				com->receiver()->setProperty("icon", QIcon()); 
		}

		QStringList currentCommands = voiceCommands.value(com->receiver());
		currentCommands.append(com->visibleTrigger());
		voiceCommands.insert(com->receiver(), currentCommands);

	}

	foreach (QObject *object, voiceCommands.keys())
	{
		QStringList visibleTriggers = voiceCommands.value(object);
		object->setProperty("toolTip", visibleTriggers.join(", "));
		// if it didn't have at least one entry it wouldn't be here
		object->setProperty("text", visibleTriggers.at(0)); 
	}

	//hide unused widgets
	foreach (VoiceInterfaceCommandTemplate *tem, voiceInterfaceCommandTemplates)
	{
		QObject *receiver = tem->receiver();
		bool shouldBeShown = voiceCommands.contains(receiver);

		QWidget *widget = dynamic_cast<QWidget*>(receiver);
		if (!widget) continue;

		widget->setVisible(shouldBeShown);
	}
}

bool CommandManager::deSerialize(const QDomElement& elem)
{
	QDomElement configElem = elem.firstChildElement("config");
	if (!deSerializeConfig(configElem)) {
		kDebug() << "Couldn't load config of plugin";
		return false;
	}
	QDomElement commandsElem = elem.firstChildElement("commands");
	if (!deSerializeCommands(commandsElem)) {
		kDebug() << "Couldn't load commands of plugin";
		return false;
	}

	adaptUi();
	return true;
}

QDomElement CommandManager::serialize(QDomDocument *doc)
{
	QDomElement pluginElem = doc->createElement("plugin");

	pluginElem.appendChild(serializeConfig(doc));
	pluginElem.appendChild(serializeCommands(doc));

	return pluginElem;
}

QVariant CommandManager::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || !commands) return QVariant();

	if (role == Qt::DisplayRole) 
		return commands->at(index.row())->getTrigger();

	if (role == Qt::DecorationRole)
		return commands->at(index.row())->getIcon();

	return QVariant();
}

Qt::ItemFlags CommandManager::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant CommandManager::headerData(int column, Qt::Orientation orientation,
			int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (column)
		{
			case 0:
				return i18n("Trigger");
		}
	}
	
	//default
	return QVariant();
}


QModelIndex CommandManager::parent(const QModelIndex &index) const
{
	Q_UNUSED(index);
	return QModelIndex();
}

int CommandManager::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid() && commands)
		return commands->count();
	else return 0;
}

int CommandManager::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

QModelIndex CommandManager::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent) || parent.isValid() || !commands)
		return QModelIndex();

	return createIndex(row, column, commands->at(row));
}

void CommandManager::setGreedyStatus(bool isGreedy)
{
	if (isGreedy) 
		m_currentState |= SimonCommand::GreedyState;
	else
		m_currentState &= ~(SimonCommand::GreedyState);
}
	


CommandManager::~CommandManager()
{
	if (commands)
		qDeleteAll(*commands);

	if (config) 
		config->deleteLater();

	foreach (QAction* action, guiActions) {
		action->deleteLater();
	}

	qDeleteAll(voiceInterfaceCommandTemplates);
}
