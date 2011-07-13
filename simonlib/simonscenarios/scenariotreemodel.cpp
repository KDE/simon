/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "scenariotreemodel.h"
#include "scenario.h"
#include <KDebug>

ScenarioTreeModel::ScenarioTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

ScenarioTreeModel::~ScenarioTreeModel()
{
}

Qt::ItemFlags ScenarioTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ScenarioTreeModel::headerData(int column, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (column) {
        case 0:
            return QString("Child Scenarios");
        }
    }

    //default
    return QVariant();
}

QModelIndex ScenarioTreeModel::index (int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Scenario *parentItem;
    if (!parent.isValid())
        parentItem = m_rootScenario;
    else
        parentItem = static_cast<Scenario*>(parent.internalPointer());

    Scenario *childItem = parentItem->childScenario(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ScenarioTreeModel::parent (const QModelIndex & index) const
{
    if (!index.isValid())
        return QModelIndex();

    Scenario *childItem = static_cast<Scenario*>(index.internalPointer());
    Scenario *parentItem = childItem->parentScenario();

    if (parentItem == m_rootScenario || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childIndex(), 0, parentItem);
}

int ScenarioTreeModel::rowCount (const QModelIndex & parent) const
{
    Scenario *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootScenario;

    else
        parentItem = static_cast<Scenario*>(parent.internalPointer());

    return parentItem->childScenarios().count();
}

int ScenarioTreeModel::columnCount (const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ScenarioTreeModel::data ( const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Scenario *item = static_cast<Scenario*>(index.internalPointer());

    return item->id();
}

void ScenarioTreeModel::setRootScenario(Scenario *root)
{
    beginResetModel();
    m_rootScenario = root;
    endResetModel();
}
