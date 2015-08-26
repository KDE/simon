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

#include "grammar.h"
#include "scenario.h"
#include <QMutexLocker>

/**
 * Empty, private constructor
 */
Grammar::Grammar(Scenario *parent) : ScenarioObject(parent), structuresLock(QMutex::Recursive)
{
}


/**
 * Plain constructor for use as storage model
 */
Grammar::Grammar() : ScenarioObject(0), structuresLock(QMutex::Recursive)
{
}


/**
 * Factory function
 * \author Peter Grasch
 */
Grammar* Grammar::createGrammar(Scenario *parent, const QDomElement& elem)
{
  Grammar *g = new Grammar(parent);
  if (!g->deSerialize(elem)) {
    g->deleteLater();
    g=0;
  }
  return g;
}


bool Grammar::deSerialize(const QDomElement& grammarElem)
{
  QMutexLocker lock(&structuresLock);
  if (grammarElem.isNull())
    return false;

  //clean member
  m_structures.clear();

  QDomElement structElem = grammarElem.firstChildElement();

  while (!structElem.isNull()) {
    m_structures << structElem.text();

    structElem = structElem.nextSiblingElement();
  }

  return true;
}


QDomElement Grammar::createEmpty(QDomDocument *doc)
{
  return doc->createElement("grammar");
}


QDomElement Grammar::serialize(QDomDocument *doc)
{
  QMutexLocker lock(&structuresLock);
  QDomElement elem = createEmpty(doc);

  foreach (const QString& structure, m_structures) {
    QDomElement structElem = doc->createElement("structure");
    structElem.appendChild(doc->createTextNode(structure));
    elem.appendChild(structElem);
  }

  return elem;
}


bool Grammar::renameCategory(QString category, const QString& newName)
{
  qDebug() << "Renaming category: " << category << newName;
  QMutexLocker lock(&structuresLock);

  //make the category regex-able :)
  /*category.replace('.', "\\.");
  category.replace('-', "\\-");
  category.replace('+', "\\+");
  category.replace('!', "\\!");
  category.replace('?', "\\?");
  category.replace('*', "\\*");
  category.replace('\\', "\\\\");
  category.replace('^', "\\^");
  category.replace('$', "\\$");*/

  QStringList newStructures;
  //replace using regex patterns
  for (int j=0; j < m_structures.count(); j++) {
    QStringList currentStructure = m_structures.at(j).split(' ');
    for (int i=0; i < currentStructure.count(); i++) {
      if (currentStructure[i] == category)
        currentStructure.replace(i, newName);
    }
    m_structures.replace(j, currentStructure.join(" "));
  }

  return parentScenario->save();
}


QStringList Grammar::getCategories()
{
  QMutexLocker lock(&structuresLock);
  QStringList out;
  QStringList categoriesInStruct;
  foreach (const QString& structure, m_structures)
  {
    categoriesInStruct = getCategoriesForStructure(structure);
    for (int j=0; j < categoriesInStruct.count(); j++)
      if (!out.contains(categoriesInStruct[j]))
        out << categoriesInStruct[j];
  }
  return out;
}

QStringList Grammar::getCategoriesForStructure(const QString &structure)
{
  return structure.split(' ');
}


QString Grammar::getExampleSentence(const QString& category)
{
  if (m_structures.count() == 0) return QString();

  int start = qrand() % m_structures.count();

  //start at this random position
  for (int i=start; i < m_structures.count(); i++)
    if (m_structures.at(i).contains(category))
      return m_structures.at(i);

  //not found? ok start backwards
  for (int i=start-1; i > 0; i--)
    if (m_structures.at(i).contains(category))
      return m_structures.at(i);

  return QString();                               //no sentence found
}


QVariant Grammar::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0:
        return m_structures.at(index.row());
    }
  }

  return QVariant();
}


QString Grammar::getStructure(int index)
{
  if (index < m_structures.count())
    return m_structures[index];

  return QString();
}


Qt::ItemFlags Grammar::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant Grammar::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Sentence");
    }
  }

  //default
  return QVariant();
}


bool Grammar::addStructures(const QStringList& newStructures, bool save)
{
  foreach (const QString& structure, newStructures)
    if (!addStructure(structure, false /* do not save */))
    return false;

  return (!save || parentScenario->save());
}


bool Grammar::addStructure(const QString& newStructure, bool save)
{
  //avoid doubles
  if (m_structures.contains(newStructure)) return true;

  int count = m_structures.count();
  beginInsertRows(QModelIndex(), count, count);
  m_structures << newStructure;
  endInsertRows();

  if (!save) return true;

  return parentScenario->save();
}


bool Grammar::deleteStructure(int index)
{
  if (index > m_structures.count()) return false;

  beginRemoveRows(QModelIndex(), index, index);
  m_structures.removeAt(index);
  endRemoveRows();

  return parentScenario ? parentScenario->save() : true;
}

bool Grammar::deleteStructure(const QString& structure)
{
  int index = m_structures.indexOf(structure);
  return (index == -1) ? false : deleteStructure(index);
}

QModelIndex Grammar::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int Grammar::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_structures.count();
  else return 0;
}


int Grammar::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


QModelIndex Grammar::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column);
}
