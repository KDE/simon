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
#include <simonactions/action.h>
#include <commandpluginbase/commandmanager.h>

ActionCollection::ActionCollection(Scenario *parent) : ScenarioObject(parent)
{

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
			m_actions << a;
			if (a->hasCommands())
				m_actionsWithCommands << a;
		}

		pluginElem = pluginElem.nextSiblingElement();
	}
	return true;
}

QDomElement ActionCollection::serialize(QDomDocument *doc)
{
	QDomElement actionsElem = doc->createElement("actions");
	foreach (Action *a, m_actions) {
		actionsElem.appendChild(a->serialize(doc));
	}
	return actionsElem;
}


QVariant ActionCollection::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	if (role == Qt::DisplayRole) 
		return m_actionsWithCommands.at(index.row())->manager()->name();

	if (role == Qt::DecorationRole)
		return m_actionsWithCommands.at(index.row())->manager()->icon();

	return QVariant();
}

Qt::ItemFlags ActionCollection::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ActionCollection::headerData(int column, Qt::Orientation orientation,
			int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (column)
		{
			case 0:
				return i18n("Action");
		}
	}
	
	//default
	return QVariant();
}


QModelIndex ActionCollection::parent(const QModelIndex &index) const
{
	Q_UNUSED(index);
	return QModelIndex();
}

int ActionCollection::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return m_actionsWithCommands.count();
	else return 0;
}

int ActionCollection::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

QModelIndex ActionCollection::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent) || parent.isValid())
		return QModelIndex();

	return createIndex(row, column, m_actionsWithCommands.at(row));
}




ActionCollection::~ActionCollection()
{
	qDeleteAll(m_actions);
}

