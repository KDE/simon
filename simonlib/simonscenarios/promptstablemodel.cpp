/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#include "promptstablemodel.h"
#include "promptstable.h"
#include <KLocalizedString>
#include <QDebug>

PromptsTableModel::PromptsTableModel(PromptsTable* table, QObject *parent) : QAbstractItemModel(parent), m_table(table)
{
}

void PromptsTableModel::reset()
{
  QAbstractItemModel::reset();
}

Qt::ItemFlags PromptsTableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PromptsTableModel::headerData(int idx, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole) return QVariant();
  
  if (orientation == Qt::Vertical)
    return QString::number(idx + 1);
  
  switch (idx) {
    case 0:
      return i18n("Prompt");
    case 1:
      return i18n("Sample Group");
  }
  
  return QVariant();
}

QModelIndex PromptsTableModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int PromptsTableModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return m_table->m_samples.count();
}

QModelIndex PromptsTableModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return createIndex(row, column);
}

QVariant PromptsTableModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    QString key = m_table->m_samples[index.row()];
    switch (index.column()) {
      case 0:
        return m_table->m_wordBySample[key];
      case 1:
        return m_table->m_groupBySample[key];
    }
  }
  return QVariant();
}

int PromptsTableModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}
