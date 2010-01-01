/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "actioncollection.h"
#include <KDebug>
#include <simonscenarios/action.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/createcommandwidget.h>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/actioncommandmodel.h>
#include "actioncommandmodel.h"

ActionCollection::ActionCollection(Scenario *parent) : ScenarioObject(parent)
{
	proxy = new ActionCommandModel(this);

}

/**
 * Factory function
 * \author Peter Grasch
 */
ActionCollection* ActionCollection::createActionCollection(Scenario *parent, const QDomElement& actionCollectionElem)
{
	ActionCollection *ac = new ActionCollection(parent);
	if (!ac->deSerialize(actionCollectionElem)) {
		delete ac;
		ac=NULL;
	} 
	return ac;
}

bool ActionCollection::deSerialize(const QDomElement& actionCollectionElem)
{
	if (actionCollectionElem.isNull())
		return false;
	
	//clean member
	qDeleteAll(m_actions);
	m_actions.clear();

	QDomElement pluginElem = actionCollectionElem.firstChildElement();
	while (!pluginElem.isNull()) {
		Action *a = Action::createAction(parentScenario, pluginElem);
		if (!a) {
			kDebug() << "Couldn't load action";
		} else {
			//m_actions << a;
			appendAction(a, true /*silent*/);
		}

		pluginElem = pluginElem.nextSiblingElement();
	}
	proxy->update();
	reset();
	return true;
}

QDomElement ActionCollection::createEmpty(QDomDocument *doc)
{
	return doc->createElement("actions");
}

QDomElement ActionCollection::serialize(QDomDocument *doc)
{
	QDomElement actionsElem = createEmpty(doc);
	foreach (Action *a, m_actions) {
		actionsElem.appendChild(a->serialize(doc));
	}
	proxy->update();
	return actionsElem;
}


QList<CreateCommandWidget*>* ActionCollection::getCreateCommandWidgets(QWidget *parent)
{
	QList<CreateCommandWidget*> *out = new QList<CreateCommandWidget*>();
	
	foreach (Action* action, m_actions)
	{
		CreateCommandWidget* widget = (CreateCommandWidget*) action->manager()->getCreateCommandWidget(parent);
		if (widget)
			*out << widget;
	}
	return out;
}

QList<CommandConfiguration*>* ActionCollection::getConfigurationPages()
{
	QList<CommandConfiguration*>* configs = new QList<CommandConfiguration*>();
	foreach (Action* a, m_actions) {
		CommandConfiguration *cm = a->getConfigurationPage();
		if (cm)
			configs->append(cm);
	}
	return configs;
}

QList<QAction*> ActionCollection::getGuiActions()
{
	QList<QAction*> guiActions;
	foreach (Action* a, m_actions) {
		guiActions << a->manager()->getGuiActions();
	}
	return guiActions;
}


/*bool ActionCollection::addCommand(Command *command)
{
	if (!command) return false;
	
	int i=0;
	bool added=false;
	while (!added && (i< m_actions.count())) {
		CommandManager *man = m_actions.at(i)->manager();
		added = man->addCommand(command);
		i++;
	}

	proxy->update();
	return added;
}*/

bool ActionCollection::removeCommand(Command *command)
{
	bool removed=false;
	foreach (Action *a, m_actions) {
		if (a->removeCommand(command)) {
			removed = true;
			break;
		}
	}
	proxy->update();
	return removed;
}

bool ActionCollection::addAction(Action *action, bool silent, bool save)
{
	action->assignParent(parentScenario);
	action->deSerialize(QDomElement());

	appendAction(action, silent);
	proxy->update();

	return (save) ? parentScenario->save() : true;
}

bool ActionCollection::deleteAction(Action *action)
{
	for (int i=0; i <m_actions.count(); i++) {
		if (m_actions[i] == action) {
			beginRemoveRows(QModelIndex(), i, i);
			m_actions.takeAt(i);
			endRemoveRows();
		}
	}
	delete action;

	proxy->update();
	return parentScenario->save();
}

bool ActionCollection::moveActionUp(Action *action)
{
	bool moved = false;

	for (int i=1; i <m_actions.count(); i++) {
		if (m_actions[i] == action) {
			m_actions.takeAt(i);
			m_actions.insert(i-1, action);
			emit dataChanged(index(i-1, 0), 
					  index(i, columnCount()));
			moved = true;
			break;
		}
	}
	proxy->update();

	return moved;
}

bool ActionCollection::moveActionDown(Action *action)
{
	bool moved = false;

	for (int i=0; i <m_actions.count()-1; i++) {
		if (m_actions[i] == action) {
			m_actions.takeAt(i);
			m_actions.insert(i+1, action);
			emit dataChanged(index(i, 0), 
					  index(i+1, columnCount()));
			moved = true;
			break;
		}
	}
	proxy->update();

	return moved;
}

bool ActionCollection::processResult(RecognitionResult recognitionResult)
{
	int i=0;
	bool commandFound=false;
	QString currentTrigger;
	QString realCommand;


	while ((i<m_actions.count()) && (!commandFound))
	{
		currentTrigger = m_actions[i]->trigger();
		RecognitionResult tempResult = recognitionResult;
		if (tempResult.matchesTrigger(currentTrigger)) {
			tempResult.removeTrigger(currentTrigger);

			if(m_actions.at(i)->manager()->processResult(tempResult))
				commandFound=true;
		}
		i++;
	}

	return commandFound;
}

bool ActionCollection::triggerCommand(const QString& type, const QString& trigger)
{
	foreach (Action *a, m_actions)
		if (a->manager()->name() == type)
			return a->manager()->trigger(trigger);
	
	return false;
}

bool ActionCollection::setTrigger(const QString& trigger)
{
	bool success = true;
	parentScenario->startGroup();
	foreach (Action *a, m_actions)
		success = a->setTrigger(trigger) && success;
	parentScenario->commitGroup();
	
	return success;
}

CommandList* ActionCollection::getCommandList()
{
	CommandList *commandList = new CommandList();
	foreach (Action *a, m_actions) {
		CommandList *list = a->manager()->getCommands();

		if (list)
			commandList->append(*list);
	}
	
	return commandList;
}

void ActionCollection::setPluginFont(const QFont& font)
{
	foreach (Action *a, m_actions)
		a->setPluginFont(font);
}



ActionCollection::~ActionCollection()
{
	qDeleteAll(m_actions);
}

