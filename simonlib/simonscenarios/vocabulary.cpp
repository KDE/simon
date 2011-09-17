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

#include "vocabulary.h"
#include "trainingmanager.h"
#include <KColorScheme>
#include <QDomDocument>
#include <QDomElement>

//sonnet speller
#include <sonnet/speller.h>
#include <QFile>

/**
 * Empty, private constructor
 */
Vocabulary::Vocabulary()                          /*: m_resolveProbability(false)*/
{
}


bool Vocabulary::deSerialize(const QDomElement& vocabularyElem)
{
  if (vocabularyElem.isNull())
    return false;

  //clean member
  qDeleteAll(m_words);
  m_words.clear();
  terminals.clear();

  QDomElement wordElem = vocabularyElem.firstChildElement();

  while (!wordElem.isNull()) {
    QDomElement nameElem = wordElem.firstChildElement();
    QDomElement pronunciationElem = nameElem.nextSiblingElement();
    QDomElement terminalElem = pronunciationElem.nextSiblingElement();

    QString name = nameElem.text();
    QString pronunciation = pronunciationElem.text();
    QString terminal = terminalElem.text();

    if (!terminals.contains(terminal)) terminals << terminal;

    m_words << new Word(name, pronunciation, terminal);

    wordElem = wordElem.nextSiblingElement();
  }
  kDebug() << "Loaded " << m_words.count() << "words";

  return true;
}


QDomElement Vocabulary::createEmpty(QDomDocument *doc)
{
  return doc->createElement("vocabulary");
}


QDomElement Vocabulary::serialize(QDomDocument *doc)
{
  QDomElement elem = createEmpty(doc);

  foreach (Word *w, m_words) {
    QDomElement wordElem = doc->createElement("word");

    QDomElement nameElem = doc->createElement("name");
    nameElem.appendChild(doc->createTextNode(w->getWord()));
    wordElem.appendChild(nameElem);

    QDomElement pronunciationElem = doc->createElement("pronunciation");
    pronunciationElem.appendChild(doc->createTextNode(w->getPronunciation()));
    wordElem.appendChild(pronunciationElem);

    QDomElement terminalElem = doc->createElement("terminal");
    terminalElem.appendChild(doc->createTextNode(w->getTerminal()));
    wordElem.appendChild(terminalElem);

    elem.appendChild(wordElem);
  }

  return elem;
}


bool Vocabulary::removeWord(Word* w, bool deleteWord)
{
  //not updating terminal cache...
  for (int i=0; i < m_words.count(); i++) {
    if (m_words.at(i) == w) {
      beginRemoveRows(QModelIndex(), i, i);
      m_words.removeAt(i);
      endRemoveRows();
      if (deleteWord) delete w;
      return true;
    }
  }
  return false;
}


QVariant Vocabulary::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  Word *word = m_words.at(index.row());

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0:
        return word->getWord();
      case 1:
        return word->getPronunciation();
      case 2:
        return word->getTerminal();
      case 3:
        return word->getPropability();
    }
  }

  return QVariant();
}


QModelIndex Vocabulary::index(int row, int column,
const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column,  m_words.at(row));
}


Qt::ItemFlags Vocabulary::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant Vocabulary::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Word");
      case 1:
        return i18n("Pronunciation");
      case 2:
        return i18n("Category");
      case 3:
        return i18n("Recognition Rate");
    }
  }

  //default
  return QVariant();
}


void Vocabulary::sortWords()
{
  qSort(m_words.begin(), m_words.end(), isWordLessThan);
}


bool Vocabulary::appendWordRaw(Word* w)
{
  return insertWordRaw(wordCount(), w);
}


bool Vocabulary::insertWordRaw(int pos, Word* w)
{
  m_words.insert(pos, w);
  return true;
}


bool Vocabulary::addWord(Word *w)
{
  QList<Word*> *wList = new QList<Word*>();
  wList->append(w);
  return addWords(wList);
}


bool Vocabulary::reOrder(Word* w)
{
  if (!removeWord(w, false /* do not delete this */))
    return false;
  return addWord(w);
}


/**
 * The list of words has to be sorted
 * The input list will be destroyed!
 */
bool Vocabulary::addWords(QList<Word*> *w)
{
  if (!w) return false;
  if (w->isEmpty()) {
    delete w;
    return true;
  }

  //insertion
  for (int i=0; i < m_words.count(); i++) {
    if (!( *(m_words[i]) < *(w->at(0)) )) {
      if (*(m_words[i]) != *(w->at(0))) {
        if (!terminals.contains(w->at(0)->getTerminal()))
          terminals << w->at(0)->getTerminal();
        m_words.insert(i, w->takeAt(0));
        if (w->isEmpty()) break;
      }
      else {
        //word already in the list
        delete w->takeAt(0);
        if (w->isEmpty()) break;
      }
    }
  }

  if (!w->isEmpty()) {
    foreach (Word *word, *w) {
      if (!terminals.contains(word->getTerminal()))
        terminals << word->getTerminal();
      m_words.append(word);
    }
  }

  delete w;

  reset();

  return true;
}


QModelIndex Vocabulary::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int Vocabulary::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_words.count();
  else return 0;
}


int Vocabulary::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 4;
}


bool Vocabulary::containsWord(const QString& word)
{
  Word *w = new Word(word, "", "");
  QList<Word*>::iterator i = qLowerBound(m_words.begin(), m_words.end(), w, isWordLessThan);
  delete w;

  while (i != m_words.end()) {
    int c = QString::compare((*i)->getWord().toUpper(), word.toUpper());
    if (c == 0)
      return true;
    else if (c > 0)
      return false;
  }
  return false;

  /*
  foreach (Word *w, m_words)
    //to upper vs. case insensitive because of german sz
    if (QString::compare(w->getWord().toUpper(), word.toUpper())==0)
      return true;
  return false;
  */
}


bool Vocabulary::containsWord(const QString& word, const QString& terminal, const QString& pronunciation)
{
  Word searchWord = Word(word, terminal, pronunciation);
  foreach (Word *w, m_words)
    if ((*w) == searchWord)
    return true;
  return false;
}


QString Vocabulary::getRandomWord(const QString& terminal)
{
  if (m_words.isEmpty()) return QString();

  int starting = qrand() % m_words.count();

  //start at this random position
  for (int i=starting; i < m_words.count(); i++)
    if (m_words[i]->getTerminal() == terminal)
      return m_words[i]->getWord();

  //no matching word found? go backwards
  for (int i=starting-1; i > 0; i--)
    if (m_words[i]->getTerminal() == terminal)
      return m_words[i]->getWord();

  //still none? ok, there is nothing to see here...
  return QString();
}


bool Vocabulary::renameTerminal(const QString& from, const QString& to)
{
  foreach (Word *w, m_words)
    if (w->getTerminal() == from)
    w->setTerminal(to);
  terminals.removeAll(from);
  if (!terminals.contains(to))
    terminals << to;
  return true;
}


QStringList Vocabulary::getTerminals()
{
  return terminals;
}


/*
 * @warning:	This returns a list containing shallow copies of the words of the vocabulary
 * 		Do not delete its contents!
 */
QList<Word*> Vocabulary::findWords(const QString& name, Vocabulary::MatchType type)
{
  QList<Word*> out;

  if (type & Vocabulary::ExactMatch) {
    Word *search = new Word(name, QString(), QString());
    QList<Word*>::iterator begin = qLowerBound(m_words.begin(), m_words.end(),
      search, isWordLessThan);
    delete search;

    while (begin != m_words.end()) {
      if ((*begin)->getWord().compare(name, Qt::CaseInsensitive)== 0)
        out << *begin;
      else break;
      begin++;
    }
  }
  if (type & Vocabulary::ContainsMatch) {
    foreach (Word *w, m_words) {
      if (w->getWord().contains(name, Qt::CaseInsensitive)) {
        kDebug() << "Adding " << w->getWord() << " matching " << name;

        out << w;
      }
    }
  }

  if (type & Vocabulary::SimilarMatch) {
    Sonnet::Speller spell;
    QStringList suggestions = spell.suggest(name);
    foreach (const QString& suggestion, suggestions)
      out.append(findWords(suggestion, Vocabulary::ExactMatch));
  }

  //sort
  qSort(out.begin(), out.end(), isWordLessThan);

  if (out.count() <= 1)
    return out;                                   //no doubles

  //remove doubles
  QList<Word*> uniqueOut;
  uniqueOut.append(out.takeAt(0));

  //optimized remove doubles that takes into account that the list is sorted
  while (!out.isEmpty()) {
    if ((uniqueOut.at(uniqueOut.count()-1)) != out.at(0))
      uniqueOut.append(out.takeAt(0));
    else out.removeAt(0);
  }

  return uniqueOut;
}


/*
 * @warning:	This returns a list containing shallow copies of the words of the vocabulary
 * 		Do not delete its contents!
 */
QList<Word*> Vocabulary::findWordsByTerminal(const QString& terminal)
{
  QList<Word*> out;

  foreach (Word *w, m_words)
    if (w->getTerminal() == terminal)
    out.append(w);

  return out;
}


void Vocabulary::deleteAll()
{
  qDeleteAll(m_words);
  clear();
}


/*
 * \brief this will remove all the words - but doesn't delete them! Use deleteAll for that!
 */
void Vocabulary::clear()
{
  m_words.clear();
  reset();
}

bool Vocabulary::exportToFile(const QString& path, Vocabulary::ExportFormat format)
{
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly))
    return false;
  foreach (Word *w, m_words) {
    switch (format) {
      case SPHINX:
	f.write(w->getWord().toUtf8()+"\t"+w->getPronunciation().toUtf8()+"\n");
	break;
      default:
	kWarning() << "Export format not implemented: " << format;
	return false;
    }
  }
  f.close();
  return true;
}


Vocabulary::~Vocabulary()
{
  qDeleteAll(m_words);
}
