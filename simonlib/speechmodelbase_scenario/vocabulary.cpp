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

		m_words << new Word(name, pronunciation, terminal);

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

