/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "trainingtextcollection.h"
#include "scenario.h"
#include <KLocalizedString>

/**
 * Empty, private constructor
 */
TrainingTextCollection::TrainingTextCollection(Scenario *parent) : ScenarioObject(parent)
{
}


/**
 * Factory function
 * \author Peter Grasch
 */
TrainingTextCollection* TrainingTextCollection::createTrainingTextCollection(Scenario *parent, const QDomElement& elem)
{
  TrainingTextCollection *g = new TrainingTextCollection(parent);
  if (!g->deSerialize(elem)) {
    delete g;
    g=0;
  }
  return g;
}


bool TrainingTextCollection::deSerialize(const QDomElement& textsElem)
{
  QDomElement textElem = textsElem.firstChildElement();
  while (!textElem.isNull()) {
    TrainingText *t = TrainingText::createTrainingText(parentScenario, textElem);
    if (!t) {
      kDebug() << "Could not load trainingtext";
    }
    else {
      m_texts << t;
      kDebug() << "Trainingtext loaded: " << t->getName();
    }
    textElem = textElem.nextSiblingElement();
  }

  return true;
}


QDomElement TrainingTextCollection::createEmpty(QDomDocument *doc)
{
  return doc->createElement("trainingtexts");
}


QDomElement TrainingTextCollection::serialize(QDomDocument *doc)
{
  QDomElement textsElem = createEmpty(doc);
  foreach (TrainingText *t, m_texts) {
    textsElem.appendChild(t->serialize(doc));
  }
  return textsElem;
}


QVariant TrainingTextCollection::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0:
        return m_texts.at(index.row())->getName();
      case 1:
        return m_texts.at(index.row())->getPageCount();
      case 2:
        TrainingText* t = m_texts.at(index.row());
        return t->getRelevance();
    }
  }

  return QVariant();
}


Qt::ItemFlags TrainingTextCollection::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant TrainingTextCollection::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18nc("Name of the trainings text", "Name");
      case 1:
        return i18nc("Number of pages in the trainings text", "Pages");
      case 2:
        return i18nc("Relevance of the trainings text", "Relevance");
    }
  }

  //default
  return QVariant();
}


QModelIndex TrainingTextCollection::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int TrainingTextCollection::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_texts.count();
  else return 0;
}


int TrainingTextCollection::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 3;
}


QModelIndex TrainingTextCollection::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_texts.at(row));
}


bool TrainingTextCollection::removeText(TrainingText* text)
{
  for (int i=0; i < m_texts.count(); i++) {
    if (m_texts.at(i) == text) {
      beginRemoveRows(QModelIndex(), i, i);
      m_texts.removeAt(i);
      endRemoveRows();
      i--;
    }
  }

  return parentScenario->save();
}


bool TrainingTextCollection::addTrainingText(TrainingText* text)
{
  int count = m_texts.count();
  beginInsertRows(QModelIndex(), count, count);
  m_texts << text;
  endInsertRows();
  return parentScenario->save();
}
