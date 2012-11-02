/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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
#include "action.h"
#include "commandmanager.h"

ActionModel::ActionModel(QObject *parent)
  : QAbstractItemModel(parent)
{

}


bool ActionModel::appendAction(Action* a, bool silent)
{
  if (!silent)
    beginInsertRows(QModelIndex(), m_actions.count(), m_actions.count());

  m_actions.append(a);
  connect(a, SIGNAL(changed()), this, SLOT(updateAction()));

  if (!silent)
    endInsertRows();

  return true;
}


void ActionModel::updateAction()
{
  Action *a = dynamic_cast<Action*>(sender());
  if (!a) return;
  int row = m_actions.indexOf(a);
  emit dataChanged(index(row, 0), index(row, columnCount()));
}


bool ActionModel::takeAction(Action* a)
{
  bool found = false;

  for (int i=0; i < m_actions.count(); i++) {
    if (m_actions[i] == a) {
      beginRemoveRows(QModelIndex(), i, i);
      m_actions.takeAt(i);
      endRemoveRows();
      found = true;
    }
  }

  return found;
}


bool ActionModel::clearActions()
{
  qDeleteAll(m_actions);
  m_actions.clear();
  reset();
  return true;
}


QVariant ActionModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if ((role == Qt::DisplayRole) || (role == Qt::DecorationRole)) {
    Action *a = m_actions.at(index.row());

    //kDebug() << a->source();
    Q_ASSERT(a);

    if (role == Qt::DecorationRole)
      return a->manager()->icon();
    if (role == Qt::DisplayRole) {
      QString name = a->manager()->name();
      QString trigger = a->trigger();
      if (trigger.isEmpty()) return name;
      else return name+" ("+trigger+')';
    }
  }

  return QVariant();
}


Qt::ItemFlags ActionModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant ActionModel::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Action");
    }
  }

  //default
  return QVariant();
}


QModelIndex ActionModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int ActionModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_actions.count();
  else return 0;
}


int ActionModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


QModelIndex ActionModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_actions.at(row));
}


ActionModel::~ActionModel()
{
  foreach (Action* a, m_actions)
    a->finalize();
  qDeleteAll(m_actions);
}
