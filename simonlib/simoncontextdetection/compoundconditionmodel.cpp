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

#include "compoundcondition.h"
#include "compoundconditionmodel.h"
#include <KDebug>

CompoundConditionModel::CompoundConditionModel(CompoundCondition *base) : m_ref(base)
{
  update();
}


QVariant CompoundConditionModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (!m_conditions.at(index.row()))
    return QVariant();

  if (role == Qt::DisplayRole)
    return m_conditions.at(index.row())->name();

  //if (role == Qt::DecorationRole) {
  //  return m_conditions.at(index.row())->manager()->icon();
  //}

  return QVariant();
}


int CompoundConditionModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_conditions.count();
  else return 0;
}


QModelIndex CompoundConditionModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_conditions.at(row));
}


Qt::ItemFlags CompoundConditionModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CompoundConditionModel::headerData(int column, Qt::Orientation orientation,
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


QModelIndex CompoundConditionModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int CompoundConditionModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


void CompoundConditionModel::update()
{
  //update list
  int currentIndex=0;

  QList<Condition*> allConditions = m_ref->getConditions();

  for (int i=0; i < allConditions.count(); i++)
  {
    Condition *c = allConditions.at(i);

    if ((currentIndex >= m_conditions.count()) || (m_conditions.at(currentIndex) != c))
    {
        beginInsertRows(QModelIndex(), currentIndex, currentIndex);
        m_conditions.insert(currentIndex, c);
        endInsertRows();
    }
    currentIndex++;
  }

  if (currentIndex == m_conditions.count())
    return;

  beginRemoveRows(QModelIndex(), currentIndex, m_conditions.count());
  for ( ; currentIndex < m_conditions.count(); currentIndex++)
    m_conditions.removeAt(currentIndex);
  endRemoveRows();

}


CompoundConditionModel::~CompoundConditionModel()
{
}
