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
#include <QAction>

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

			//uncomment the following line if you don't want to allow multiple commands with the
			//same name. (this is just commented out for testing)
			//break;
		}
	}
	return done;
}
	
bool CommandManager::deSerializeConfig(const QDomElement& elem, const QString& scenarioId)
{
	Q_UNUSED(elem);
	Q_UNUSED(scenarioId);
	return true;
}

QDomElement CommandManager::serializeConfig(QDomDocument *doc, const QString& scenarioId)
{
	Q_UNUSED(scenarioId);
	return doc->createElement("config");
}

bool CommandManager::deSerializeCommands(const QDomElement& elem, const QString& scenarioId)
{
	Q_UNUSED(elem);
	Q_UNUSED(scenarioId);
	return true;
}

QDomElement CommandManager::serializeCommands(QDomDocument *doc, const QString& scenarioId)
{
	Q_UNUSED(scenarioId);
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

	if (commands->removeAll(command) > 0)
		return save();

	return false;
}


CommandManager::~CommandManager()
{
	kDebug() << "DELETING COMMAND MANAGER";

	if (commands)
		qDeleteAll(*commands);

	foreach (QAction* action, guiActions) {
		action->deleteLater();
	}

//	if (getConfigurationPage())
//		getConfigurationPage()->destroy();
}
