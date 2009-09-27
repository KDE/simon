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
#include "scenario.h"
#include <KColorScheme>

/**
 * Empty, private constructor
 */
Vocabulary::Vocabulary(Scenario *parent) : ScenarioObject(parent)
{
}

/**
 * Factory function
 * \author Peter Grasch
 */
Vocabulary* Vocabulary::createVocabulary(Scenario *parent, const QDomElement& elem)
{
	Vocabulary *v = new Vocabulary(parent);
	if (!v->deSerialize(elem)) {
		delete v;
		v=NULL;
	} 
	return v;
}

bool Vocabulary::deSerialize(const QDomElement& vocabularyElem)
{
	if (vocabularyElem.isNull())
		return false;
	
	//clean member
	qDeleteAll(m_words);

	QDomElement wordElem = vocabularyElem.firstChildElement();

	while (!wordElem.isNull()) {
		QDomElement nameElem = wordElem.firstChildElement();
		QDomElement pronunciationElem = nameElem.nextSiblingElement();
		QDomElement terminalElem = pronunciationElem.nextSiblingElement();

		QString name = nameElem.text();
		QString pronunciation = pronunciationElem.text();
		QString terminal = terminalElem.text();
		int probability = TrainingManager::getInstance()->getProbability(name);

		m_words << new Word(name, pronunciation, terminal, probability);
		//m_words << new Word(QString("test"), QString("test"), QString("test"));

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
	for (int i=0; i < m_words.count(); i++) {
		if (m_words.at(i) == w) {
			beginRemoveRows(QModelIndex(), i, i);
			m_words.removeAt(i);
			endRemoveRows();
			
			//make changes permanent
			parentScenario->save();
			return true;
		}
	}
	return false;
}


QVariant Vocabulary::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	Word *word = m_words.at(index.row());

	if (!word) {
		return QVariant();
	}
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



