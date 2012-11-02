
/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "commandtablemodel.h"
#include <KLocalizedString>
#include <KLocale>
#include <QDebug>
#include <QMutexLocker>

CommandTableModel::CommandTableModel(const CommandList &newCommands)
{
  commands = newCommands;
}


QVariant CommandTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (index.row() > commands.count())
    return QVariant();

  int pos = index.row();
  Command *com = dynamic_cast<Command*>(commands.at(pos));

  switch (index.column()) {
    case 0:
      if (role == Qt::DisplayRole) {
        if (com)
          return commands.at(pos)->getTrigger();
        else
          return QString::number(5);
      }
      else if (role == Qt::DecorationRole) {
        if (com)
          return commands.at(pos)->getIcon();
        else
          return KIcon("player-time");
      }
    case 1:
      if (role == Qt::DisplayRole) {
        if (com)
          return commands.at(pos)->getCategoryText();
        else
          return i18n("Delay");
      }
      else if (role == Qt::DecorationRole) {
        if (com)
          return commands.at(pos)->getCategoryIcon();
        else
          return KIcon("chronometer");
      }

  }

  //default
  return QVariant();
}


Qt::ItemFlags CommandTableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CommandTableModel::headerData(int section, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return i18n("Trigger");
      case 1:
        return i18n("Category");
    };
  }

  //default
  return QVariant();
}


QModelIndex CommandTableModel::index(int row, int column,
const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  if (parent.isValid())                           //no such thing
    return QModelIndex();
  return createIndex(row, column);
}


void CommandTableModel::moveUp(const QModelIndex& index)
{
  if (!index.isValid()) return;
  int i = index.row();

  if (i == 0) return;
  if (i>= commands.count()) return;

  Command *com = commands.takeAt(i);
  commands.insert(i-1, com);
  emit dataChanged(this->index(i-1, 0), this->index(i, 1));
}


void CommandTableModel::moveDown(const QModelIndex& index)
{
  if (!index.isValid()) return;
  int i = index.row();

  if (i>= commands.count()) return;

  Command *com = commands.takeAt(i);
  commands.insert(i+1, com);
  emit dataChanged(this->index(i, 0), this->index(i+1, 1));
}


QModelIndex CommandTableModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}


int CommandTableModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return commands.count();
}


int CommandTableModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;                                       // Name, Category
}


void CommandTableModel::selectCommand(Command* com)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  commands << com;
  endInsertRows();
  //	reset();
}


void CommandTableModel::removeCommand(int i)
{
  beginRemoveRows(QModelIndex(), i, i);
  commands.removeAt(i);
  endRemoveRows();
  //	reset();
}


CommandTableModel::~CommandTableModel()
{
  //do not touch the commandlist as it is just a reference
}
