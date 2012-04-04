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

#include "samplegroupconditionmodel.h"
#include <KColorScheme>
#include <QFont>

SampleGroupConditionModel::SampleGroupConditionModel(SampleGroupCondition *sampleGroupCondition, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_sampleGroupCondition = sampleGroupCondition;
    update();
}

QVariant SampleGroupConditionModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  Condition *rowCondition = m_sampleGroupCondition->getSampleGroupCondition(index.row());

  if (!rowCondition)
  {
    return QVariant();
  }
  else if (index.column() == 0)
  {
      if (role == Qt::DisplayRole)
          return  rowCondition->name();
      else if (role == Qt::FontRole && !rowCondition->isSatisfied())
      {
          QFont font;
          font.setItalic(true);
          return font;
      }
      else if (role == Qt::ForegroundRole && !rowCondition->isSatisfied())
          return KColorScheme(QPalette::Active).foreground(KColorScheme::InactiveText);
  }
  else if (index.column() == 1)
  {
      if (role == Qt::DisplayRole)
        return m_sampleGroupCondition->getSampleGroup(index.row());
  }

  return QVariant();
}

bool SampleGroupConditionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && index.column() == 1)
    {
        m_sampleGroupCondition->changeSampleGroup(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int SampleGroupConditionModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_sampleGroupCondition->getSampleGroupConditionCount();
  else return 0;
}

QModelIndex SampleGroupConditionModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || parent.isValid())
        return QModelIndex();

//    if (column == 0)
        return createIndex(row, column, m_sampleGroupCondition->getSampleGroupCondition(row));

//    else if (column == 1)
//        return createIndex(row, column, m_sampleGroupCondition->getSampleGroup(row));

//    else
//        return QModelIndex();
}


Qt::ItemFlags SampleGroupConditionModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  else if (index.column() == 0)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  else if (index.column() == 1)
      return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

  return 0;
}


QVariant SampleGroupConditionModel::headerData(int column, Qt::Orientation orientation,
                                            int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return i18n("Condition");
        case 1:
            return i18n("Sample Group");
        }
    }
    else if (orientation == Qt::Horizontal && role == Qt::SizeHintRole)
    {
        switch (column)
        {
        case 0:
            return QSize(300, 30);
        case 1:
            return QSize(200, 30);
        }
    }

  //default
  return QVariant();
}


QModelIndex SampleGroupConditionModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int SampleGroupConditionModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}


void SampleGroupConditionModel::update()
{
    reset();
}
