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

#include "actionmanager.h"
#include <QMessageBox>
#include "../SimonLib/Logging/logger.h"
#include "../SimonLib/Settings/settings.h"
#include "../SimonLib/SimonInfo/simoninfo.h"
#include "../SimonLib/EventSimulation/eventhandler.h"
#include "Commands/commandmanager.h"
// #include "Commands/ATIntegration/"
#include "Commands/DesktopGrid/desktopgridcommandmanager.h"
#include "Commands/Executable/executablecommandmanager.h"
#include "Commands/Place/placecommandmanager.h"
#include "Commands/Shortcut/shortcutcommandmanager.h"
#include "Commands/TextMacro/textmacrocommandmanager.h"

ActionManager* ActionManager::instance;

ActionManager::ActionManager(QObject *parent) : QObject(parent)
{
	managers = new CommandManagerList();
	this->eventHandler = EventHandler::getInstance();
	setupBackends();
}


void ActionManager::setupBackends()
{
	Q_ASSERT(managers);

	qDeleteAll(*managers);
	managers->clear();



	if (Settings::getB("Commands/DesktopGrid/Enabled"))
		managers->append(new DesktopGridCommandManager());
	if (Settings::getB("Commands/Executable/Enabled"))
		managers->append(new ExecutableCommandManager());
	if (Settings::getB("Commands/Place/Enabled"))
		managers->append(new PlaceCommandManager());
	if (Settings::getB("Commands/Shortcut/Enabled"))
		managers->append(new ShortcutCommandManager());
	if (Settings::getB("Commands/TextMacro/Enabled"))
		managers->append(new TextMacroCommandManager());

// 	if (Settings::getB("Commands/ATIntegration/Enabled"))
//		managers->append(new ATIntegrationCommandManager());

	for (int i=0; i< managers->count(); i++)
	{
		CommandManager *man = managers->at(i);
		if (!man->load())
			QMessageBox::warning(0, tr("Initialisieren fehlgeschlagen"), tr("Konnte Kommandomanager \"%1\" nicht initialisieren.\n\nBitte überprüfen Sie seine Konfiguration.").arg(man->name()));
	}
}

/**
* \brief Goes through all the command-lists and asks to delete every command with the given trigger
* \param trigger The trigger to match
* \return True, if all matching commands where deleted.
*/
bool ActionManager::askDeleteCommandByTrigger(QString trigger)
{
	CommandList *list = getCommandList();
	
	Q_ASSERT(list);
	bool allDeleted = true;
	
	for (int i=0; i < list->count(); i++)
	{
		Command *com = list->at(i);
		if (!com) continue;
		
		if (com->getTrigger() == trigger) {
			if (QMessageBox::question(0, tr("Trigger vergeben"), tr("Der Trigger \"%1\" ist bereits vergeben.\n\nWollen Sie das Kommando, welches den Trigger %1 verwendet, löschen?").arg(com->getTrigger()), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
			{
				deleteCommand(com);
			} else allDeleted = false;
		}
	}
	
	return allDeleted;
}

bool ActionManager::addCommand(Command *command)
{
	Q_ASSERT(managers);
	
	if (!command) return false;
	
	if (!askDeleteCommandByTrigger(command->getTrigger())) 
		return false;
	
	
	int i=0;
	bool added=false;
	while (!added && (i< managers->count()))
	{
		CommandManager *man = managers->at(i);
		added = man->addCommand(command);
		i++;
	}
	if (!added)
		QMessageBox::critical(0, tr("Nicht hinzugefügt"), tr("Konnte das Kommando \"%1\" nicht hinzufügen.").arg(command->getTrigger()));
	else emit commandsChanged(getCommandList());

	return added;
}

bool ActionManager::deleteCommand(Command *command)
{
	Q_ASSERT(managers);
	
	if (!command) return false;
	
	bool deleted;
	for (int i=0; i < managers->count(); i++)
	{
		if (managers->at(i)->deleteCommand(command))
			deleted = true;
	}
	
	if (!deleted)
		QMessageBox::warning(0, tr("Kommando löschen fehlgeschlagne"), tr("Kommando konnte nicht gelöscht werden."));
	else emit commandsChanged(getCommandList());

	command->deleteLater();
	
	return deleted;
}

CommandList* ActionManager::getCommandList()
{
	CommandManager *manager;
	CommandList *out = new CommandList();
	foreach (manager, * this->managers)
	{
		CommandList *cur = manager->getCommands();
		
		if (!cur) continue;
		
		for (int i=0; i < cur->count(); i++)
			out->append(cur->at(i));
	}
	return out;
}


void ActionManager::process(QString input)
{
	Q_ASSERT(managers);
	Q_ASSERT(eventHandler);


	QString keyword = Settings::getS("Commands/Trigger");
	if (input.startsWith(keyword))
	{
		input = input.remove(0, QString(keyword).length()).trimmed();
		
		int i=0;
		while ((i < managers->count()) && (!managers->at(i)->trigger(input)))
		{ i++; }
		if (i == managers->count()) // didn't find anything
			emit guiAction(input);

	} else {
		if (Settings::getB("Commands/Dictation")) //is dictation activated?
			eventHandler->sendWord(input);
	}
}


ActionManager::~ActionManager()
{
	//...
}
