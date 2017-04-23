/*
 *   Copyright (C) 2012 Adam Nash <adam.t.nash@gmail.com>
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


#include "samplegroupcondition.h"
#include "contextmanager.h"
#include <QTextStream>
#include <QFont>
#include <QFile>
#include <KStandardDirs>
#include <KDebug>
#include <KColorScheme>
#include <KLocalizedString>

typedef QPair<Condition*, QString> ConditionPair;

SampleGroupCondition::SampleGroupCondition(QObject *parent) :
    QAbstractItemModel(parent)
{
}
SampleGroupCondition::~SampleGroupCondition()
{
    while (!m_sampleGroupConditions.empty())
        removeSampleGroupCondition(0, true);
}

QStringList SampleGroupCondition::getSampleGroups() const
{
    QStringList allGroups;
    foreach (ConditionPair i, m_sampleGroupConditions)
        if (!allGroups.contains(i.second))
            allGroups << i.second;
    return allGroups;
}

QVariant SampleGroupCondition::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  Condition *rowCondition = m_sampleGroupConditions[index.row()].first;

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
        return m_sampleGroupConditions.at(index.row()).second;
  }

  return QVariant();
}

bool SampleGroupCondition::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || index.column() != 1) return false;
      
    changeSampleGroup(index.row(), value.toString());
    return true;
}

int SampleGroupCondition::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_sampleGroupConditions.count();
    else return 0;
}

QModelIndex SampleGroupCondition::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || parent.isValid())
        return QModelIndex();

    return createIndex(row, column, m_sampleGroupConditions[row].first);
}


Qt::ItemFlags SampleGroupCondition::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    else if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    else if (index.column() == 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return 0;
}


QVariant SampleGroupCondition::headerData(int column, Qt::Orientation orientation,
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

    return QVariant();
}

QModelIndex SampleGroupCondition::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int SampleGroupCondition::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}

void SampleGroupCondition::addSampleGroupCondition( Condition* condition, QString sampleGroup, bool holdChecks )
{
    Q_ASSERT(condition);
    
    beginInsertRows(QModelIndex(), m_sampleGroupConditions.count(), m_sampleGroupConditions.count());
    m_sampleGroupConditions.push_back(qMakePair(condition, sampleGroup));
    endInsertRows();
    
    connect(condition, SIGNAL(conditionChanged()),
            this, SLOT(checkAcousticContext()));
    if (!holdChecks)
      checkAcousticContext();
    emit changed();
}

bool SampleGroupCondition::removeSampleGroupCondition(int index, bool holdChecks)
{
    if (index >= m_sampleGroupConditions.count() || index < 0) return false;
    
    disconnect(m_sampleGroupConditions.at(index).first, SIGNAL(conditionChanged()),
            this, SLOT(checkAcousticContext()));

    ContextManager::instance()->releaseCondition(m_sampleGroupConditions.at(index).first);
    
    beginRemoveRows(QModelIndex(), index, index);
    m_sampleGroupConditions.removeAt(index);
    endRemoveRows();
    
    if (!holdChecks) checkAcousticContext();
    
    emit changed();
    return true;
}

void SampleGroupCondition::updateCondition ( int row, Condition* edit )
{
    Q_ASSERT(edit);
    Q_ASSERT(row < m_sampleGroupConditions.count() && row > 0);
    m_sampleGroupConditions[row].first = edit;
    
    emit dataChanged(index(row, 0), index(row, 0));
    checkAcousticContext();
    emit changed();
}


bool SampleGroupCondition::changeSampleGroup(int row, QString sampleGroup)
{
    if (row >= m_sampleGroupConditions.count())
        return false;
    
    m_sampleGroupConditions[row].second = sampleGroup;
    
    emit dataChanged(index(row, 1), index(row, 1));
    checkAcousticContext();
    emit changed();
    return true;
}

void SampleGroupCondition::checkAcousticContext()
{
    QStringList deactivatedList;
    for (int i=0; i<m_sampleGroupConditions.count(); i++)
    {
        if (!m_sampleGroupConditions.at(i).first->isSatisfied())
          deactivatedList.push_back(m_sampleGroupConditions[i].second);
    }
    deactivatedList.removeDuplicates();
    
    if (deactivatedList != m_lastDeactivatedList) {
      m_lastDeactivatedList = deactivatedList;
      emit sampleGroupChanged(deactivatedList);
    }
}

QStringList SampleGroupCondition::getDeactivatedSampleGroups()
{
    return m_lastDeactivatedList;
}

void SampleGroupCondition::saveSampleGroupContext()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("samplegroupcontextroot");

    foreach (ConditionPair i, m_sampleGroupConditions)
    {
        QDomElement sampleGroupContext = doc.createElement("samplegroupcontext");
        sampleGroupContext.appendChild(i.first->serialize(&doc));

        QDomElement sampleGroup = doc.createElement("samplegroup");
        sampleGroup.setAttribute("name", i.second);
        sampleGroupContext.appendChild(sampleGroup);

        root.appendChild(sampleGroupContext);
    }

    QString contextDir = KStandardDirs::locateLocal("appdata", "context/");
    QFile sampleGroupContextFile(contextDir + "sampleGroupContext");

    sampleGroupContextFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&sampleGroupContextFile);
    root.save(stream, 2);
    sampleGroupContextFile.close();
}

void SampleGroupCondition::loadSampleGroupContext()
{
    QString contextDir = KStandardDirs::locateLocal("appdata", "context/");
    QFile sampleGroupContextFile(contextDir + "sampleGroupContext");

    if (!sampleGroupContextFile.exists())
        return;

    sampleGroupContextFile.open(QIODevice::ReadOnly);
    QDomDocument doc;
    doc.setContent(&sampleGroupContextFile);
    sampleGroupContextFile.close();

    QDomElement root = doc.documentElement();

    QDomElement sampleGroupContext = root.firstChildElement("samplegroupcontext");
    while (!sampleGroupContext.isNull())
    {
        QDomElement conditionElement = sampleGroupContext.firstChildElement("condition");
        Condition* condition = ContextManager::instance()->getCondition(conditionElement);

        if (condition)
        {
            QDomElement sampleGroup = sampleGroupContext.firstChildElement("samplegroup");

            addSampleGroupCondition(condition, sampleGroup.attribute("name", "default"), true /* hold checks */);
        }

        sampleGroupContext = sampleGroupContext.nextSiblingElement("samplegroupcontext");
    }
    checkAcousticContext();
}
