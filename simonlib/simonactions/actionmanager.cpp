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

#include "commandsettings.h"
#include "listcommand.h"

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
#include <KXmlGuiWindow>
#include <KXMLGUIClient>
#include <KXMLGUIFactory>



ActionManager* ActionManager::instance;


ActionManager::ActionManager(QObject* parent) : QObject(parent),
	mainWindow(0),
	commandSettings(0),
	currentlyPromptedListOfResults(0),
	greedyReceivers(new QList<GreedyReceiver*>()),
	minimumConfidenceThreshold(0.7),
	useDYM(true)
{
	KLocale::setMainCatalog("simonlib");
}

void ActionManager::init()
{
	if (commandSettings)
	{
		setupBackends(commandSettings->getActivePlugins());
		retrieveRecognitionResultFilteringParameters();
	}
}

void ActionManager::setMainWindow(KXMLGUIClient *window)
{
	this->mainWindow = window;
}

void ActionManager::setConfigurationDialog(KCModule* commandSettings)
{
	this->commandSettings = dynamic_cast<CommandSettings*>(commandSettings);
	if (!this->commandSettings) return;
	
	connect(commandSettings, SIGNAL(actionsChanged(QList<Action::Ptr>)), this, SLOT(setupBackends(QList<Action::Ptr>)));
	connect(commandSettings, SIGNAL(recognitionResultsFilterParametersChanged()), this, SLOT(retrieveRecognitionResultFilteringParameters()));
	
	foreach (Action::Ptr action, actions)
		this->commandSettings->registerPlugIn(action->manager()->getConfigurationPage());
}

void ActionManager::deleteManager(CommandManager *manager)
{
	Q_ASSERT(manager);

	if (commandSettings)
		commandSettings->unregisterPlugIn(manager->getConfigurationPage());

	foreach (Action::Ptr action, actions)
	{
		if (action->manager() == manager)
		{
			delete action;
			actions.removeAll(action);
		}
	}
}


void ActionManager::setupBackends(QList<Action::Ptr> pluginsToLoad)
{
	bool changed=false;

	//iterate over all existing managers and find the ones we still use
	//by comparing their source()'es;
	//Change the trigger to the one given by the corresponding action in
	//pluginsToLoad and copy it to the array of new managers
	//Remove those already loaded commandmanagers from the pluginsToLoad
	//list;
	//
	//Copy the remaining commandManagers from pluginsToLoad to the array
	//retaining their positions
	
	int count = pluginsToLoad.count();
	//Action::Ptr newActionsArray[count];
	Action::Ptr *newActionsArray = new Action::Ptr[count];

	int i=0;

	while (pluginsToLoad.count() > 0)
	{
		Action *newAction = pluginsToLoad.takeAt(0);
		if (!newAction) {
			count--;
			continue;
		}
		
		QString source = newAction->source();

		bool found = false;

		//iterate over all loaded actions
		//and try to find this one
		for (int j=0; j < actions.count(); j++) {
			if (!actions[j]) {
				actions.removeAt(j--);
				continue;
			}
			if (actions[j]->source() == source) {
				//if found, move it with the appropriate spot
				//in the array and apply the (possibly new)
				//trigger
				Action *alreadyLoadedAction = actions.takeAt(j);
				if (newAction->trigger() != alreadyLoadedAction->trigger()) {
					alreadyLoadedAction->setTrigger(newAction->trigger());
					changed = true;
				}
				if (i != j) //position changed
					changed=true;
				newActionsArray[i] = alreadyLoadedAction;
				found = true;
				break;
			}
		}

		if (!found) {
			//if it was not found, copy it from pluginsToLoad
			if (newAction->manager()) {
				if (!newAction->manager()->load()) {
					KMessageBox::error(0, i18n("Couldn't initialize commandmanager \"%1\".\n\n"
							"Please check its configuration.", newAction->manager()->name()));
				} else {
					if (commandSettings) commandSettings->registerPlugIn(newAction->manager()->getConfigurationPage());
				}
				newActionsArray[i] = newAction;
			}
			changed=true;
		}
		i++;
	}

	// delete the actions remaining in the member variable as they are not
	// used any longer and copy the array there
	foreach (Action::Ptr action, actions)
	{
		changed=true;

		if (!action) {
			continue;
		}

		if (action->manager() && commandSettings)
			commandSettings->unregisterPlugIn(action->manager()->getConfigurationPage());
		delete action;
	}
	actions.clear();

	for (int i=0; i < count; i++) {
		actions << newActionsArray[i];
	}

	if (changed) {
		publishCategories();
	}

	publishGuiActions();
}


void ActionManager::publishGuiActions()
{
	mainWindow->unplugActionList("command_actionlist");

	QList<QAction*> guiActions;
	foreach (Action::Ptr a, actions) {
		if (!a) continue;
		guiActions << a->manager()->getGuiActions();
	}

	mainWindow->plugActionList("command_actionlist", guiActions);
}

void ActionManager::publishCategories()
{
	QList<KIcon> icons;
	QStringList names;
	for (int i=0; i < actions.count(); i++)
	{
		if (!actions[i] || !actions[i]->manager() || (!actions[i]->manager()->hasCommands()))
					continue;

		names << actions[i]->manager()->name();
		icons << actions[i]->manager()->icon();
	}
	emit categoriesChanged(icons, names);

}

QList<CreateCommandWidget*>* ActionManager::getCreateCommandWidgets(QWidget *parent)
{
	QList<CreateCommandWidget*> *out = new QList<CreateCommandWidget*>();
	
	foreach (Action::Ptr action, this->actions)
	{
		CreateCommandWidget* widget = (CreateCommandWidget*) action->manager()->getCreateCommandWidget(parent);
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


void ActionManager::registerGreedyReceiver(GreedyReceiver *receiver)
{
	greedyReceivers->insert(0, receiver);
}

void ActionManager::deRegisterGreedyReceiver(GreedyReceiver *receiver)
{
	greedyReceivers->removeAll(receiver);
}

bool ActionManager::addCommand(Command *command)
{
	if (!command) return false;
	
	if (!askDeleteCommandByTrigger(command->getTrigger())) 
		return false;
	
	
	int i=0;
	bool added=false;
	while (!added && (i< actions.count()))
	{
		CommandManager *man = actions.at(i)->manager();
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
	foreach (Action::Ptr action, actions)
	{
		if (action->manager()->name() == category)
		{
			CommandList *list = action->manager()->getCommands();
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
	if (!command) return false;
	
	QString trigger = command->getTrigger();
	QString cat = command->getCategoryText();
	bool deleted;
	for (int i=0; i < actions.count(); i++)
	{
		if (actions.at(i)->manager()->deleteCommand(command))
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
	CommandList *out = new CommandList();
	foreach (Action::Ptr action, this->actions)
	{
		CommandList *cur = action->manager()->getCommands();
		
		if (!cur) continue;
		
		for (int i=0; i < cur->count(); i++)
			out->append(cur->at(i));
	}
	return out;
}



bool ActionManager::triggerCommand(const QString& type, const QString& trigger)
{
	if (type == "simonrecognitionresult" && currentlyPromptedListOfResults) {
		//result from a did-you-mean popup
		QString selectedSentence = trigger;
		selectedSentence.remove(QRegExp("^[0-9][0-9]?[0-9]?%: "));
		for (int i=0; i< currentlyPromptedListOfResults->count(); i++)
		{
			QString sentence = currentlyPromptedListOfResults->at(i).sentence();
			if (sentence == selectedSentence) {
				fprintf(stderr, "Found the result!\n");
				RecognitionResultList *list = new RecognitionResultList();
				list->append(RecognitionResult(currentlyPromptedListOfResults->at(i)));
				delete currentlyPromptedListOfResults;
				currentlyPromptedListOfResults = 0;
				processRawResults(list);
				return true;
			}
		}
		return true;
	}

	Command *com = getCommand(type, trigger);
	if (com)
		return com->trigger();
	else return false;
}

CommandList* ActionManager::getCommandsOfCategory(const QString& category)
{
	foreach (Action::Ptr action, actions)
		if (action->manager()->name() == category)
			return action->manager()->getCommands();

	return NULL;
}


void ActionManager::processResult(RecognitionResult recognitionResult)
{
	fprintf(stderr, "processResult()\n");
	fprintf(stderr, "Verarbeite Ergebnis: %s\n", recognitionResult.sentence().toUtf8().data());
	int i=0;
	bool commandFound=false;
	QString currentTrigger;
	QString realCommand;
	fprintf(stderr, "recognitionResult: %s\n", recognitionResult.sentence().toUtf8().data());
	while ((i<actions.count()) && (!commandFound))
	{
		currentTrigger = actions[i]->trigger();
		fprintf(stderr, "CurrentTrigger: %s\n", currentTrigger.toUtf8().data());
		RecognitionResult tempResult = recognitionResult;
		if (tempResult.matchesTrigger(currentTrigger)) {
			tempResult.removeTrigger(currentTrigger);

			if(actions.at(i)->manager()->processResult(tempResult))
				commandFound=true;
		}
		i++;
	}

/*	if (!commandFound)
		emit guiAction(finalResult.sentence());*/
}

void ActionManager::retrieveRecognitionResultFilteringParameters()
{
	useDYM = commandSettings->useDYM();
	minimumConfidenceThreshold = commandSettings->getMinimumConfidence();
}

void ActionManager::processRawResults(RecognitionResultList* recognitionResults)
{
	if (recognitionResults->isEmpty())
		return;

	RecognitionResultList *selectedRecognitionResults = new RecognitionResultList();

	if (!currentlyPromptedListOfResults || currentlyPromptedListOfResults->isEmpty()) {
		for (int i=0; i < recognitionResults->count(); i++) {
		//foreach (const RecognitionResult& result, recognitionResults) {
			//if the recognition result has:
			//	* One word that has a score of 0
			//	* An average score of below the minimum confidence
			//it will be not be included in the list of results
			
			QList<float> confidenceScores = recognitionResults->at(i).confidenceScores();

			//calc average
			float avg= recognitionResults->at(i).averageConfidenceScore();

			if (!confidenceScores.contains(0) && (avg > minimumConfidenceThreshold))
				selectedRecognitionResults->append(recognitionResults->at(i));
		}

		fprintf(stderr, "Viable recognition results: %d\n", selectedRecognitionResults->count());

		if (selectedRecognitionResults->count() == 0) return;
	} else {
		//we are already asking...
		selectedRecognitionResults->append(recognitionResults->at(0));
	}

	fprintf(stderr, "Greedy Recievers: %d\n", greedyReceivers->count());

	if (!greedyReceivers->isEmpty()) {
		for (int i=0; i < greedyReceivers->count(); i++) {
			if (greedyReceivers->at(i)->greedyTriggerRawList(selectedRecognitionResults))
				break;
		}
		delete selectedRecognitionResults;
		return;
	}

	if (selectedRecognitionResults->count() == 1) {
		processResult(selectedRecognitionResults->at(0));
	} else {
		presentUserWithResults(selectedRecognitionResults);
	}
	delete selectedRecognitionResults;
}

void ActionManager::resultSelectionDone()
{
	fprintf(stderr, "resultSelectionDone()\n");
	delete currentlyPromptedListOfResults;
	currentlyPromptedListOfResults = 0;
}

void ActionManager::presentUserWithResults(RecognitionResultList* recognitionResults)
{
	fprintf(stderr, "More than one possible recognition result ... should display list!\n");
	if (!useDYM || (currentlyPromptedListOfResults && 
				!currentlyPromptedListOfResults->isEmpty()))
	{
		//no double did-you-means...
		processResult(recognitionResults->at(0));
		return;
	}

	delete currentlyPromptedListOfResults;
	currentlyPromptedListOfResults = new RecognitionResultList();

	QStringList sentences;
	QStringList trigger;
	QStringList iconSrcs;
	for (int i=0; i<recognitionResults->count(); i++) {

		QList<float> confidenceScores = recognitionResults->at(i).confidenceScores();

		float avg = recognitionResults->at(i).averageConfidenceScore();
		avg *= 100;


		sentences << QString("%1%: %2").arg(qRound(avg)/*, 0, 'f', 0*/).arg(recognitionResults->at(i).sentence());
		iconSrcs << "";
		trigger << "simonrecognitionresult";
		currentlyPromptedListOfResults->append(recognitionResults->at(i));

	}

	ListCommand *list = new ListCommand(i18n("Did you mean ...?"), "help-hint", sentences, iconSrcs, trigger);
	connect(list, SIGNAL(canceled()), list, SLOT(deleteLater()));
	connect(list, SIGNAL(canceled()), this, SLOT(resultSelectionDone()));
	connect(list, SIGNAL(entrySelected()), list, SLOT(deleteLater()));
	list->trigger();
}


ActionManager::~ActionManager()
{
	//...
}
