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
#include "commandconfiguration.h"
#include <KLocalizedString>
#include <simonscenarios/scenario.h>
#include <QAction>
#include <QDomElement>
#include <QDomDocument>

bool CommandManager::trigger(const QString& triggerName)
{
	if (!commands) return false;

	bool done=false;
	for (int i=0; i < commands->count(); i++)
	{
		if (!commands->at(i)) continue;

		if (commands->at(i)->getTrigger() == triggerName)
		{
			if (commands->at(i)->trigger())
				done=true;
			break;
		}
	}
	return done;
}
	
bool CommandManager::deSerializeConfig(const QDomElement& elem, Scenario *parent)
{
	Q_UNUSED(elem);
	Q_UNUSED(parent);
	return true;
}

QDomElement CommandManager::serializeConfig(QDomDocument *doc, Scenario *parent)
{
	Q_UNUSED(parent);
	return doc->createElement("config");
}

bool CommandManager::deSerializeCommands(const QDomElement& elem, Scenario *parent)
{
	Q_UNUSED(elem);
	Q_UNUSED(parent);
	return true;
}

QDomElement CommandManager::serializeCommands(QDomDocument *doc, Scenario *parent)
{
	Q_UNUSED(parent);
	QDomElement commandsElem = doc->createElement("commands");
	if (commands) {
		foreach (Command *c, *commands)
			commandsElem.appendChild(c->serialize(doc));
	}

	return commandsElem;
}

QList<QAction*> CommandManager::getGuiActions()
{
	return guiActions;
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
 * 
 * If you want your Plugin to be configurable you will have to reimplement this.
 */
CommandConfiguration* CommandManager::getConfigurationPage()
{
	return 0;
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
			delete command;
			return parentScenario->save();
		}
	}

	return false;
}

bool CommandManager::deSerialize(const QDomElement& elem)
{
	QDomElement configElem = elem.firstChildElement("config");
	if (!deSerializeConfig(configElem, parentScenario)) {
		kDebug() << "Couldn't load config of plugin";
		return false;
	}
	QDomElement commandsElem = elem.firstChildElement("commands");
	if (!deSerializeCommands(commandsElem, parentScenario)) {
		kDebug() << "Couldn't load commands of plugin";
		return false;
	}
	return true;
}

QDomElement CommandManager::serialize(QDomDocument *doc)
{
	QDomElement pluginElem = doc->createElement("plugin");

	pluginElem.appendChild(serializeConfig(doc, parentScenario));
	pluginElem.appendChild(serializeCommands(doc, parentScenario));

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



CommandManager::~CommandManager()
{
	if (commands)
		qDeleteAll(*commands);

	foreach (QAction* action, guiActions) {
		action->deleteLater();
	}

//	if (getConfigurationPage())
//		getConfigurationPage()->destroy();
}
