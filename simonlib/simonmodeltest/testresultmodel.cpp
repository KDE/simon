/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "testresultmodel.h"
#include "testresult.h"
#include <KLocale>

TestResultModel::TestResultModel(QObject *parent) : QAbstractItemModel(parent)
{
}

QVariant TestResultModel::headerData(int section, Qt::Orientation orientation,
  int role) const
{
  if ((orientation == Qt::Vertical) || (role != Qt::DisplayRole))
    return QVariant();

  switch (section)
  {
    case 0:
      return i18nc("Text that is being said in this sample according to the prompts file", "Label");

  }
  return QVariant();
}

QModelIndex TestResultModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int TestResultModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return m_results.count();
}

QModelIndex TestResultModel::index(int row, int column,const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_results.at(row));
}

QVariant TestResultModel::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DisplayRole) return QVariant();

  TestResult *r = m_results[index.row()];
  switch (index.column())
  {
    case 0:
      return r->prettyLabel();
  }

  return QVariant();
}

int TestResultModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


void TestResultModel::setResults(QList<TestResult*>& results)
{
  m_results = results;
  reset();
}

void TestResultModel::clear()
{
  m_results.clear();
  reset();
}

