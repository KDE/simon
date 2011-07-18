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

#include "scenariolistmodel.h"

#include <KDebug>

ScenarioListModel::ScenarioListModel()
{
  update();
}

QVariant ScenarioListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole)
    return m_scenarioIds.at(index.row());

  return QVariant();
}


int ScenarioListModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_scenarioIds.count();
  else return 0;
}


QModelIndex ScenarioListModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, (void*)&m_scenarioIds.at(row));
}


Qt::ItemFlags ScenarioListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant ScenarioListModel::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Condition");
    }
  }

  //default
  return QVariant();
}


QModelIndex ScenarioListModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int ScenarioListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


void ScenarioListModel::update()
{
  m_scenarioIds = ScenarioManager::getInstance()->getAllAvailableScenarioIds();
}


ScenarioListModel::~ScenarioListModel()
{
}

