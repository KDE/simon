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
#include "contextmanager.h"

#include <QFont>
#include <KColorScheme>
#include <KDebug>

CompoundCondition::CompoundCondition(QObject *parent) :
  QAbstractItemModel(parent)
{
  m_satisfied = true;
}

QDomElement CompoundCondition::createEmpty(QDomDocument *doc)
{
  return doc->createElement("compoundcondition");
}

CompoundCondition::CompoundCondition(const CompoundCondition& other) : QAbstractItemModel(((const QObject&) other).parent())
{
  foreach (Condition * condition, m_conditions)
  ContextManager::instance()->refCondition(condition);
}

CompoundCondition& CompoundCondition::operator=(const CompoundCondition& other)
{
  Q_UNUSED(other);
  foreach (Condition * condition, m_conditions)
  ContextManager::instance()->refCondition(condition);
  return *this;
}

CompoundCondition::~CompoundCondition()
{
  foreach (Condition * condition, m_conditions)
  ContextManager::instance()->releaseCondition(condition);
}

CompoundCondition* CompoundCondition::createInstance(const QDomElement &elem)
{
  //create the instance
  CompoundCondition* instance = new CompoundCondition();

  //deserialize the compound condition dom element
  if (!instance->deSerialize(elem)) {
    kDebug() << "Error: failed to create compound condition!";
    return 0;
  }

  //initial evaluation of the condition
  instance->evaluateConditions();

  return instance;
}

bool CompoundCondition::addCondition(Condition *condition)
{
  if (!condition)
    return false;

  beginInsertRows(QModelIndex(), m_conditions.count(), m_conditions.count());
  m_conditions.push_back(condition);
  endInsertRows();
  connect(condition, SIGNAL(conditionChanged()),
          this, SLOT(evaluateConditions()));

  this->evaluateConditions();

  emit modified();

  return true;
}

bool CompoundCondition::removeCondition(Condition *condition)
{
  if (!condition)
    return false;

  int row = m_conditions.indexOf(condition);
  beginRemoveRows(QModelIndex(), row, row);
  bool succ = m_conditions.removeOne(condition);
  endRemoveRows();

  if (!succ) {
    kDebug() << "Condition is not part of the compound condition!";
    reset();
    return false;
  }

  disconnect(condition, SIGNAL(conditionChanged()),
             this, SLOT(evaluateConditions()));

  ContextManager::instance()->releaseCondition(condition);
  this->evaluateConditions();

  emit modified();

  return true;
}

bool CompoundCondition::deSerialize(const QDomElement &elem)
{
  QDomElement conditionElem;
  Condition* condition;
  ContextManager* manager = ContextManager::instance();

  beginResetModel();

  QList<Condition*> oldConditions = m_conditions;
  m_conditions.clear();
  conditionElem = elem.firstChildElement("condition");
  bool allOkay = true;

  while(!conditionElem.isNull()) {
    //get condition from the condition dom element
    condition = manager->getCondition(conditionElem);

    //error checking
    if (condition == NULL) {
      kDebug() << "Error: Invalid Condition within CompoundCondition!";
      allOkay = false;
    } else {
      //connect condition to the evaluateConditions slot
      connect(condition, SIGNAL(conditionChanged()),
              this, SLOT(evaluateConditions()));

      //add the condition to the conditions list
      m_conditions.push_back(condition);
    }

    //get next condition element
    conditionElem = conditionElem.nextSiblingElement("condition");
  }

  foreach (Condition * condition, oldConditions)
  manager->releaseCondition(condition);

  endResetModel();

  return allOkay;
}

QDomElement CompoundCondition::serialize(QDomDocument *doc)
{
  QDomElement elem = doc->createElement("compoundcondition");

  foreach (Condition * condition, m_conditions)
  elem.appendChild(condition->serialize(doc));

  return elem;
}

void CompoundCondition::evaluateConditions()
{
  Condition *condition;

  if (m_satisfied == true) {
    foreach (condition, m_conditions) { // krazy:exclude=foreach
      if (!condition->isSatisfied()) {
        m_satisfied = false;
        kDebug() << "CompoundCondition is not satisfied!";
        emit conditionChanged(m_satisfied);

        return;
      }
    }
    kDebug() << "CompoundCondition is still satisfied!";
  } else {
    foreach (condition, m_conditions) { // krazy:exclude=foreach
      if (!condition->isSatisfied()) {
        kDebug() << "CompoundCondition is still not satisfied!";
        return;
      }
    }

    m_satisfied = true;
    kDebug() << "CompoundCondition is satisfied!";
    emit conditionChanged(m_satisfied);
  }
}

QVariant CompoundCondition::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (!m_conditions.at(index.row()))
    return QVariant();

  if (index.column() == 0) {
    if (role == Qt::DisplayRole)
      return  m_conditions.at(index.row())->name();
    else if (role == Qt::FontRole && !m_conditions.at(index.row())->isSatisfied()) {
      QFont font;
      font.setItalic(true);
      return font;
    } else if (role == Qt::ForegroundRole && !m_conditions.at(index.row())->isSatisfied())
      return KColorScheme(QPalette::Active).foreground(KColorScheme::InactiveText);
  }

  return QVariant();
}


int CompoundCondition::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_conditions.count();
  else return 0;
}


QModelIndex CompoundCondition::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_conditions.at(row));
}


Qt::ItemFlags CompoundCondition::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CompoundCondition::headerData(int column, Qt::Orientation orientation,
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


QModelIndex CompoundCondition::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int CompoundCondition::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}

