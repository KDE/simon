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

#include "activevocabulary.h"
#include "trainingmanager.h"
#include "scenario.h"
#include <KColorScheme>

/**
 * Constructor
 */
ActiveVocabulary::ActiveVocabulary(Scenario *parent) : Vocabulary(), ScenarioObject(parent)
{
  buildBrushes();
}


/**
 * Factory function
 * \author Peter Grasch
 */
ActiveVocabulary* ActiveVocabulary::createVocabulary(Scenario *parent, const QDomElement& elem)
{
  ActiveVocabulary *v = new ActiveVocabulary(parent);
  if (!v->deSerialize(elem)) {
    v->deleteLater();
    v=0;
  }
  return v;
}


void ActiveVocabulary::buildBrushes()
{
  KColorScheme colorScheme(QPalette::Active);
  QColor negative = colorScheme.background(KColorScheme::NegativeBackground).color();

  recogNone = KColorScheme::shade(negative, KColorScheme::MidShade);
}


QVariant ActiveVocabulary::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  Word *word = m_words.at(index.row());

  if (role == Qt::BackgroundRole) {
    if (word->getBlacklisted()) return recogNone;
  }

  return Vocabulary::data(index, role);
}


bool ActiveVocabulary::addWords(QList<Word*> w)
{
  if (!Vocabulary::addWords(w)) return false;
  return parentScenario->save();
}


bool ActiveVocabulary::addWord(Word *w)
{
  if (!Vocabulary::addWord(w)) return false;
  return parentScenario->save();
}


bool ActiveVocabulary::reOrder(Word* w)
{
  parentScenario->startGroup();
  if (!Vocabulary::reOrder(w)) return false;
  return parentScenario->commitGroup();
}


bool ActiveVocabulary::removeWord(Word* w, bool deleteWord)
{
  if (Vocabulary::removeWord(w, deleteWord)) {
    return parentScenario->save();
  }
  return false;
}


bool ActiveVocabulary::deSerialize(const QDomElement& elem)
{
  return Vocabulary::deSerialize(elem);
}


QDomElement ActiveVocabulary::serialize(QDomDocument *doc)
{
  return Vocabulary::serialize(doc);
}


bool ActiveVocabulary::empty()
{
  deleteAll();
  return parentScenario->save();
}


bool ActiveVocabulary::renameCategory(const QString& from, const QString& to)
{
  if (Vocabulary::renameCategory(from, to))
    return parentScenario->save();
  return false;
}
