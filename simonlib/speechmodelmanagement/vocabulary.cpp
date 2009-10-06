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

/**
 * Empty, private constructor
 */
Vocabulary::Vocabulary()
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
		int probability = TrainingManager::getInstance()->getProbability(name);

		if (!terminals.contains(terminal)) terminals << terminal;

		m_words << new Word(name, pronunciation, terminal, probability);

		wordElem = wordElem.nextSiblingElement();
	}

	return true;
}

QDomElement Vocabulary::serialize(QDomDocument *doc)
{
	QDomElement elem = doc->createElement("vocabulary");

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

bool Vocabulary::removeWord(Word* w)
{
	//not updating terminal cache...
	for (int i=0; i < m_words.count(); i++) {
		if (m_words.at(i) == w) {
			beginRemoveRows(QModelIndex(), i, i);
			m_words.removeAt(i);
			endRemoveRows();
			return true;
		}
	}
	return false;
}


QVariant Vocabulary::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	Word *word = m_words.at(index.row());

	if (role == Qt::DisplayRole) 
	{
		switch (index.column())
		{
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

	return createIndex(row, column, (void*) m_words.at(row));
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
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (column)
		{
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
	//qSort will sort based on pointer addresses so this is obviously bogus
//	qSort(m_words.begin(), m_words.end());
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

/**
 * The list of words has to be sorted
 * The input list will be destroyed!
 */
bool Vocabulary::addWords(QList<Word*> *w)
{
	if (!w) return false;

	kDebug() << this;

	//insertion
	for (int i=0; i < m_words.count(); i++) {
		if (!( *(m_words[i]) < *(w->at(0)) )) {
		//	kDebug() << m_words[i]->getWord() << " !< " << w->at(0)->getWord();
			if (!terminals.contains(w->at(0)->getTerminal()))
				terminals << w->at(0)->getTerminal();
			m_words.insert(i, w->takeAt(0));
			if (w->isEmpty()) break;
		} //else
		//	kDebug() << m_words[i]->getWord() << " < " << w->at(0)->getWord();
	}

	if (!w->isEmpty()) {
		foreach (Word *word, *w) {
		//	kDebug() << "Appending: " << word->getWord();
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
	Q_UNUSED(word);
	//TODO: implement
	return false;
}

bool Vocabulary::containsWord(const QString& word, const QString& terminal, const QString& pronunciation)
{
	Q_UNUSED(word);
	//TODO: implement
	return false;
}

QString Vocabulary::getRandomWord(const QString& terminal)
{
	Q_UNUSED(terminal);
	//TODO: implement
	return QString();
}

bool Vocabulary::renameTerminal(const QString& from, const QString& to)
{
	foreach (Word *w, m_words)
		if (w->getTerminal() == from)
			w->setTerminal(to);
	return true;
}

QStringList Vocabulary::getTerminals()
{
	return terminals;
}

/*
 * @warning:	This returns a list containing shallow copies of the words of the vocabulary
 * 		Don't delete its contents!
 */
QList<Word*>* Vocabulary::findWords(const QString& name)
{
	//TODO: implement
	return new QList<Word*>();
}

Vocabulary::~Vocabulary()
{
	qDeleteAll(m_words);
}

