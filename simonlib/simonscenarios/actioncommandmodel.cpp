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

#include "actioncommandmodel.h"
#include <KDebug>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/action.h>

ActionCommandModel::ActionCommandModel(ActionCollection *base) : ref(base)
{
	update();
}

QVariant ActionCommandModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	if (role == Qt::DisplayRole) 
		return m_actions.at(index.row())->manager()->name();

	if (role == Qt::DecorationRole) {
		return m_actions.at(index.row())->manager()->icon();
	}

	return QVariant();
}

int ActionCommandModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return m_actions.count();
	else return 0;
}


QModelIndex ActionCommandModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent) || parent.isValid())
		return QModelIndex();

	return createIndex(row, column, m_actions.at(row));
}


Qt::ItemFlags ActionCommandModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ActionCommandModel::headerData(int column, Qt::Orientation orientation,
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


QModelIndex ActionCommandModel::parent(const QModelIndex &index) const
{
	Q_UNUSED(index);
	return QModelIndex();
}

int ActionCommandModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

void ActionCommandModel::update()
{
	//update list
	int currentIndex=0;

	QList<Action*> allActions = ref->actions();
	for (int i=0; i < allActions.count(); i++) {
		Action *a = allActions.at(i);
		if (a->hasCommands()) {
			if ((currentIndex >= m_actions.count()) || (m_actions.at(currentIndex) != a)) {
				beginInsertRows(QModelIndex(), currentIndex, currentIndex);
				m_actions.insert(currentIndex, a);
				endInsertRows();
			}
			currentIndex++;
		}
	}

	if (currentIndex == m_actions.count())
		return;

	beginRemoveRows(QModelIndex(), currentIndex, m_actions.count());
	for ( ; currentIndex < m_actions.count(); currentIndex++) 
		m_actions.removeAt(currentIndex);
	endRemoveRows();

}

ActionCommandModel::~ActionCommandModel()
{
}


