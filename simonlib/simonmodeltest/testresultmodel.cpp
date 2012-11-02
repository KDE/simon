/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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
#include <QColor>
#include <KLocale>
#include <KColorScheme>

TestResultModel::TestResultModel(QObject *parent) : QAbstractItemModel(parent)
{
  KColorScheme colorScheme(QPalette::Active);
  QColor negative = colorScheme.background(KColorScheme::NegativeBackground).color();
  recogWrong = KColorScheme::shade(negative, KColorScheme::MidShade);
}

QVariant TestResultModel::headerData(int section, Qt::Orientation orientation,
  int role) const
{
  if ((orientation == Qt::Vertical) || ((role != Qt::DisplayRole)))
    return QVariant();

  switch (section)
  {
    case 0:
      return i18nc("Text that is being said in this sample according to the prompts file", "Label");
    case 1: return i18n("Count");
    case 2: return i18n("Correct");
    case 3: return i18n("Incorrect");
    case 4: return i18n("Confidence");
    case 5: return i18n("Accuracy");
    case 6: return i18n("Word error rate");
    case 7: return i18n("Insertion errors");
    case 8: return i18n("Deletion errors");
    case 9: return i18n("Substitution errors");

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

QString TestResultModel::printPercentage(float in) const
{
  return i18nc("Printing nice percentage display", "%1 %", qRound(in*10000.0f)/100.0f);
}

QVariant TestResultModel::data(const QModelIndex &index, int role) const
{
  TestResult *r = m_results[index.row()];

  if (role == Qt::BackgroundRole) {
    if (r->incorrectCount())
      return recogWrong;
  }

  if (role == Qt::ToolTipRole) {
    return i18n("Recognition results:\n%1", r->recognitionResults());
  }

  if (role == Qt::DisplayRole)
  {
    switch (index.column())
    {
      case 0: return r->prettyLabel();
      case 1: return r->count();
      case 2: return r->correctCount();
      case 3: return r->incorrectCount();
      case 4: return printPercentage(r->confidence());
      case 5: return printPercentage(r->accuracy());
      case 6: return printPercentage(r->wordErrorRate());
      case 7: return r->insertionErrors();
      case 8: return r->deletionErrors();
      case 9: return r->substitutionErrors();
    }
  }

  return QVariant();
}

int TestResultModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 10;
}


void TestResultModel::setResults(QList<TestResult*>& results)
{
  m_results = results;
  reset();
}

QVariant TestResultModel::data(int row, int column) const
{
  return data(index(row, column), Qt::DisplayRole);
}

void TestResultModel::clear()
{
  m_results.clear();
  reset();
}

