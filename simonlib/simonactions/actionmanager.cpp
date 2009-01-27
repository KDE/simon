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
#include <simoninfo/simoninfo.h>
#include <commandpluginbase/commandconfiguration.h>
#include <commandpluginbase/commandmanager.h>
#include <commandpluginbase/createcommandwidget.h>

#include <QFile>
#include <QMetaObject>

#include <KMessageBox>
#include <KLocalizedString>
#include <KService>
#include <KServiceTypeTrader>
#include <KStandardDirs>
#include <KDesktopFile>
#include <KDebug>
#include <KLocale>

#include "commandsettings.h"


ActionManager* ActionManager::instance;


ActionManager::ActionManager(QObject *parent) : QObject(parent)
{
	KLocale::setMainCatalog("simonlib");
	managers = new QList<CommandManager*>();
	commandSettings=0;
}

void ActionManager::init()
{
	if (commandSettings)
	{
		setupBackends(commandSettings->getPluginsToLoad());
		setTrigger(commandSettings->getTrigger());
	}
}

void ActionManager::setConfigurationDialog(KCModule* commandSettings)
{
	this->commandSettings = dynamic_cast<CommandSettings*>(commandSettings);
	if (!this->commandSettings) return;
	
	connect(commandSettings, SIGNAL(pluginSelectionChanged(const QStringList&)), this, SLOT(setupBackends(const QStringList&)));
	connect(commandSettings, SIGNAL(triggerChanged(const QStringList&)), this, SLOT(setTrigger(const QStringList&)));
	
	foreach (CommandManager *man, *managers)
		this->commandSettings->registerPlugIn(man->getConfigurationPage());
}

void ActionManager::setTrigger(const QStringList& trigger)
{
	this->trigger = trigger;
}

void ActionManager::deleteManager(CommandManager *manager)
{
	if (commandSettings)
		commandSettings->unregisterPlugIn(manager->getConfigurationPage());
	manager->deleteLater();
	managers->removeAll(manager);
}


void ActionManager::setupBackends(const QStringList& pluginsToLoad)
{
	Q_ASSERT(managers);
    kWarning() << pluginsToLoad;
	bool changed=false;
	
	CommandManager* newManagerArray[pluginsToLoad.count()];
	int i=0;
	foreach (QString pluginToLoad, pluginsToLoad)
	{
		bool found=false;
		foreach (CommandManager *man, *managers)
		{
			if (man->name() == pluginToLoad)
			{
				if (managers->indexOf(man) != i)
					changed=true;
				
				newManagerArray[i] = man;
				managers->removeAll(man);
				found=true;
			}
		}
		if (!found)
			changed=true;
		i++;
	}
	
	KService::List services;
	KServiceTypeTrader* trader = KServiceTypeTrader::self();
	services = trader->query("simon/CommandPlugin");
	
	i=0;
	foreach (KService::Ptr service, services) {
		KPluginFactory *factory = KPluginLoader(service->library()).factory();
 
		if (!factory)
			continue;
		
		CommandManager *man = factory->create<CommandManager>(this);
	
		if (man)
		{
			QString currentManagerName = man->name();
			if (pluginsToLoad.contains(currentManagerName))
			{
				newManagerArray[pluginsToLoad.indexOf(currentManagerName)] = man;
				
				if (!man->load())
					KMessageBox::error(0, i18n("Couldn't initialize commandmanager \"%1\".\n\n"
							"Please check its configuration.", currentManagerName));
				else
				{
					if (commandSettings) commandSettings->registerPlugIn(man->getConfigurationPage());
					changed=true;
				}
			} else
				man->deleteLater();
		}
	}
	
	foreach (CommandManager *manager, *managers)
	{
		kWarning() << "Deleting " << manager;
		deleteManager(manager);
		changed=true;
	}
	for (i=0; i < pluginsToLoad.count(); i++)
	{
		kWarning() << newManagerArray[i];
		*(this->managers) << newManagerArray[i];
	}
	
	//if (changed) emit commandsChanged(getCommandList());
	if (changed) 
		publishCategories();
}

void ActionManager::publishCategories()
{
	QList<KIcon> icons;
	QStringList names;
	foreach (CommandManager* man, *managers)
	{
		if ((!man->getCommands())
				|| (man->getCommands()->count() == 0))
			continue;

		names << man->name();
		icons << man->icon();
	}
	emit categoriesChanged(icons, names);

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
			if (KMessageBox::questionYesNoCancel(0, i18n("The trigger \"%1\" is already assigned to another command.\n\nDo you want to delete the other command that uses the Trigger \"%1\"?", com->getTrigger())) == KMessageBox::Yes)
			{
				deleteCommand(com);
			} else allDeleted = false;
		}
	}
	
	delete list;

	return allDeleted;
}


void ActionManager::registerPrompt(QObject *receiver, const char* slot)
{
	greedyReceivers << GreedyReceiver(receiver, slot);
}

void ActionManager::deRegisterPrompt(QObject *d_receiver, const char* d_slot)
{
	for (int i=0; i < greedyReceivers.count(); i++)
	{
		if ((greedyReceivers[i].receiver() == d_receiver)
				&& (greedyReceivers[i].slot() == d_slot))
			greedyReceivers.removeAt(i--);
	}

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
		KMessageBox::error(0, i18n("Couldn't add Command \"%1\".", command->getTrigger()));
	else 
		emit commandAdded(command);

	return added;
}

Command* ActionManager::getCommand(const QString& category, const QString& trigger)
{
	foreach (CommandManager* manager, *managers)
	{
		if (manager->name() == category)
		{
			CommandList *list = manager->getCommands();
			foreach (Command* com, *list)
			{
				if (com->getTrigger() == trigger)
					return com;
			}
			break;
		}
	}
	return NULL;

}

bool ActionManager::deleteCommand(Command *command)
{
	Q_ASSERT(managers);
	
	if (!command) return false;
	
	QString trigger = command->getTrigger();
	QString cat = command->getCategoryText();
	bool deleted;
	for (int i=0; i < managers->count(); i++)
	{
		if (managers->at(i)->deleteCommand(command))
			deleted = true;
	}
	
	if (!deleted)
		KMessageBox::error(0, i18n("Command could not be deleted."));
	else {
		//emit commandsChanged(getCommandList());
		emit commandRemoved(trigger, cat);
	}

	//command will be deleted
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



bool ActionManager::triggerCommand(const QString& type, const QString& trigger)
{
	Q_ASSERT(managers);
	
	Command *com = getCommand(type, trigger);
	if (com)
		return com->trigger();
	else return false;
}

CommandList* ActionManager::getCommandsOfCategory(const QString& category)
{
	foreach (CommandManager *man, *managers)
		if (man->name() == category)
			return man->getCommands();

	return NULL;
}

void ActionManager::process(QString input)
{
	if (!greedyReceivers.isEmpty())
	{
		foreach (const GreedyReceiver& rec, greedyReceivers)
		{
			bool accepted;
			QMetaObject::invokeMethod(rec.receiver(), rec.slot(), 
					Qt::DirectConnection, Q_RETURN_ARG(bool, accepted), Q_ARG(QString, input));
			if (accepted) return;
		}
		return;
	}

	Q_ASSERT(managers);
	Q_ASSERT(commandSettings);

	if (input.isEmpty()) return;
	if (trigger.count() != managers->count()) return;

	int i=0;
	bool commandFound=false;
	QString currentTrigger;
	QString realCommand;
	while ((i<trigger.count()) && (!commandFound))
	{
		currentTrigger = trigger[i];
		if (input.startsWith(currentTrigger))
		{
			if (!currentTrigger.isEmpty())
				realCommand = input.mid(currentTrigger.count()+1);
			else realCommand = input;

			if(managers->at(i)->trigger(realCommand))
				commandFound=true;
		}
		i++;
	}
	if (!commandFound)
		emit guiAction(input);
}


ActionManager::~ActionManager()
{
	//...
}
