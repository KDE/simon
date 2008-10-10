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

#include "actionmanager.h"

#include <simonlogging/logger.h>
#include <simoninfo/simoninfo.h>
#include "Commands/commandpluginbase/commandconfiguration.h"
#include "Commands/commandpluginbase/commandmanager.h"
#include "Commands/commandpluginbase/createcommandwidget.h"

#include <QFile>

#include <KMessageBox>
#include <KLocalizedString>
#include <KService>
#include <KServiceTypeTrader>

#include "eventsimulation/eventhandler.h"

#include "../Configuration/configurationdialog.h"
#include "commandsettings.h"


ActionManager* ActionManager::instance;


ActionManager::ActionManager(QObject *parent) : QObject(parent)
{
	managers = new QList<CommandManager*>();
}

void ActionManager::init()
{
	this->commandSettings = new CommandSettings();
	connect (commandSettings, SIGNAL(pluginSelectionChanged(const QStringList&)),
		  this, SLOT(setupBackends(QStringList)));
		  
	ConfigurationDialog::getInstance()->registerModule( commandSettings );
	
	setupBackends(commandSettings->getPluginsToLoad());
}

void ActionManager::deleteManager(CommandManager *manager)
{
	ConfigurationDialog::getInstance()->unregisterModule(manager->getConfigurationPage());
	manager->deleteLater();
	managers->removeAll(manager);
}


void ActionManager::setupBackends(QStringList pluginsToLoad)
{
	Q_ASSERT(managers);

	bool changed=false;
	
	foreach (CommandManager *man, *managers)
	{
		QString currentName = man->name();
		
		if (pluginsToLoad.contains(currentName))
			pluginsToLoad.removeAll(currentName);
		else {
			deleteManager(man);
			changed=true;
		}
	}
	if (pluginsToLoad.count() > 0)
		changed=true;

	KService::List services;
	KServiceTypeTrader* trader = KServiceTypeTrader::self();

	services = trader->query("simon/CommandPlugin");
	
	ConfigurationDialog *configDialog = ConfigurationDialog::getInstance();
	
	foreach (KService::Ptr service, services) {
		KPluginFactory *factory = KPluginLoader(service->library()).factory();
 
		if (!factory)
			continue;
		
		CommandManager *man = factory->create<CommandManager>(this);
	
		if (man)
		{
			if (pluginsToLoad.contains(man->name()))
			{
				managers->append(man);
				configDialog->registerModule(man->getConfigurationPage());
				
				if (!man->load())
					KMessageBox::error(0, i18n("Konnte Kommandomanager \"%1\" nicht initialisieren.\n\n"
							"Bitte überprüfen Sie seine Konfiguration.", man->name()));
			} else
				man->deleteLater();
		}
	}
	
	if (changed) emit commandsChanged(getCommandList());
}

QList<CreateCommandWidget*>* ActionManager::getCreateCommandWidgets(QWidget *parent)
{
	QList<CreateCommandWidget*> *out = new QList<CreateCommandWidget*>();
	
	foreach (CommandManager* manager, * this->managers)
	{
		CreateCommandWidget* widget = (CreateCommandWidget*) manager->getCreateCommandWidget(parent);
		if (widget)
			*out << widget;
	}
	return out;
}

QStringList ActionManager::availableCommandManagers()
{
	QStringList commandManagers;
	
	KService::List services;
	KServiceTypeTrader* trader = KServiceTypeTrader::self();

	services = trader->query("simon/CommandPlugin");
	
	foreach (KService::Ptr service, services) {
		KPluginFactory *factory = KPluginLoader(service->library()).factory();
		if (!factory) continue;
		
		CommandManager *man = factory->create<CommandManager>(this);
		if (man) {
			commandManagers << man->name();
			man->deleteLater();
		}
	}
	return commandManagers;
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
			if (KMessageBox::questionYesNoCancel(0, i18n("Der Trigger \"%1\" ist bereits vergeben.\n\nWollen Sie das Kommando, welches den Trigger %1 verwendet, löschen?", com->getTrigger())) == KMessageBox::Yes)
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
		KMessageBox::error(0, i18n("Konnte das Kommando \"%1\" nicht hinzufügen.", command->getTrigger()));
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
		KMessageBox::error(0, i18n("Kommando konnte nicht gelöscht werden."));
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
	Q_ASSERT(commandSettings);

	QString keyword = commandSettings->globalTrigger();
	
	bool commandFound=false;
	if (input.startsWith(keyword) || (!commandSettings->useGlobalTrigger()))
	{
		input = input.remove(0, QString(keyword).length()).trimmed();
		
		int i=0;
		while ((i < managers->count()) && (!managers->at(i)->trigger(input)))
		{ i++; }
		if (i == managers->count()) // didn't find anything
			emit guiAction(input);
		else commandFound=true;
	} 
	if ((!input.startsWith(keyword) || (!commandSettings->useGlobalTrigger() && !commandFound)) &&
		commandSettings->dictation()) //is dictation activated?
			EventHandler::getInstance()->sendWord(input);
}


ActionManager::~ActionManager()
{
	//...
}
