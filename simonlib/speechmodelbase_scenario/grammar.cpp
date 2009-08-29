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

#include "grammar.h"

/**
 * Empty, private constructor
 */
Grammar::Grammar(const QString& scenarioId) : ScenarioObject(scenarioId)
{
}

/**
 * Factory function
 * \author Peter Grasch
 */
Grammar* Grammar::createGrammar(const QString& scenarioId, const QDomElement& elem)
{
	Grammar *g = new Grammar(scenarioId);
	if (!g->deSerialize(elem)) {
		delete g;
		g=NULL;
	} 
	return g;
}

bool Grammar::deSerialize(const QDomElement& grammarElem)
{
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

QDomElement Grammar::serialize(QDomDocument *doc)
{
	QDomElement elem = doc->createElement("grammar");

	foreach (const QString& structure, m_structures) {
		QDomElement structElem = doc->createElement("structure");
		structElem.appendChild(doc->createTextNode(structure));
		elem.appendChild(structElem);
	}

	return elem;
}

