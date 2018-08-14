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

#include "commandmodel.h"
#include <KLocalizedString>
#include <QMutexLocker>

CommandModel::CommandModel(CommandList CL)
: commands(CL)
{
}


void CommandModel::updateCommands(const CommandList& newCommands)
{
  commands = newCommands;
  reset();
}


QVariant CommandModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (! index.parent().isValid()) {
    //category

    int row = index.row();
    QStringList categories = getCategories(commands);
    if (row >= categories.count()) return QVariant();
    QString strCategory = categories[row];

    if (role == Qt::DisplayRole) {
      return strCategory;
    } else if (role == Qt::DecorationRole)
    {
      for (int i=0; i<commands.count(); i++)
        if (commands.at(i)->getCategoryText()==strCategory)
          return commands.at(i)->getCategoryIcon();
    }
  } else
          {                                       //command

            Command *command = static_cast<Command*>(index.internalPointer());
            if (!command) {
              return QVariant();
            }
            if (role == Qt::DisplayRole)
      return command->getTrigger();
    else if (role == Qt::DecorationRole)
      return command->getIcon();
  }

  return QVariant();
}


Qt::ItemFlags CommandModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) {
    return 0;
  }

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CommandModel::headerData(int, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    return i18n("Command");
  }

  //default
  return QVariant();
}


QModelIndex CommandModel::index(int row, int column,
const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  if (parent.isValid()) {
    //child
    QString catStr = parent.data(Qt::DisplayRole).toString();

    //find line
    if (row < commands.count()) {
      int i;
      int counter=-1;
      for (i=0; counter < row; i++) {
        if (commands.at(i)->getCategoryText() == catStr)
          counter++;
      }
      i--;
      return createIndex(row, column, commands.at(i));
    }
  }
  else {
    QStringList cats = getCategories(commands);
    return createIndex(row, column, 0);
  }

  return QModelIndex();
}


QModelIndex CommandModel::parent(const QModelIndex &index) const
{
  if (!index.isValid()) {
    return QModelIndex();
  }

  if (commands.contains((Command*) index.internalPointer())) {
    QStringList cats = getCategories(commands);

    Command *com = static_cast<Command*>(index.internalPointer());
    if (!com) return QModelIndex();

    QString category =  com->getCategoryText();
    int i=0;
    while ((i < cats.count()) && (cats[i] != category))
      {i++;}

      return this->index(i, 0, QModelIndex());
  }

  return QModelIndex();
}


int CommandModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid()) {
    int rowCount = getCategories(commands).count();
    return rowCount;
  }
  else {
    QString strCat = parent.data(Qt::DisplayRole).toString();
    int count=0;

    for (int i=0; i < commands.count(); i++)
      if (commands.at(i)->getCategoryText() == strCat)
        count++;

    return count;
  }
}


const QStringList CommandModel::getCategories(const CommandList &commands) const
{
  QStringList strCategories;
  for (int i=0; i < commands.count(); i++) {
    if (commands.at(i) && !strCategories.contains(commands.at(i)->getCategoryText()))
      strCategories << commands.at(i)->getCategoryText();
  }
  return strCategories;
}


int CommandModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


CommandModel::~CommandModel()
{
  //do not touch the commandlist as it is just a reference
}
