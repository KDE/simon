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
#include "scenario.h"
#include <QMutexLocker>
#include <QtGlobal>

/**
 * Empty, private constructor
 */
Grammar::Grammar(Scenario *parent) : ScenarioObject(parent)
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
		delete g;
		g=NULL;
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

QDomElement Grammar::serialize(QDomDocument *doc)
{
	QMutexLocker lock(&structuresLock);
	QDomElement elem = doc->createElement("grammar");

	foreach (const QString& structure, m_structures) {
		QDomElement structElem = doc->createElement("structure");
		structElem.appendChild(doc->createTextNode(structure));
		elem.appendChild(structElem);
	}

	return elem;
}

bool Grammar::renameTerminal(QString terminal, const QString& newName)
{
	QMutexLocker lock(&structuresLock);

	//make the terminal regex-able :)
	terminal.replace(".", "\\.");
	terminal.replace("-", "\\-");
	terminal.replace("!", "\\!");
	terminal.replace("?", "\\?");
	terminal.replace("*", "\\*");
	terminal.replace("\\", "\\\\");
	terminal.replace("^", "\\^");
	terminal.replace("$", "\\$");

	QStringList newStructures;
	//replace using regex patterns
	for (int j=0; j < m_structures.count(); j++)
	{
		QStringList currentStructure = m_structures.at(j).split(" ");
		for (int i=0; i < currentStructure.count(); i++)
		{
			if (currentStructure[i] == terminal)
				currentStructure.replace(i, newName);
		}
		m_structures.replace(j, currentStructure.join(" "));
	}

	return parentScenario->save();
}

QStringList Grammar::getTerminals()
{
	QMutexLocker lock(&structuresLock);
	QStringList out;
	QStringList terminalsInStruct;
	foreach (const QString& structure, m_structures)
	{
		terminalsInStruct.clear();
		terminalsInStruct = structure.split(" ");
		for (int j=0; j < terminalsInStruct.count(); j++)
			if (!out.contains(terminalsInStruct[j]))
				out << terminalsInStruct[j];
	}
	return out;
}

QString Grammar::getExampleSentence(const QString& terminal)
{
	int start = qrand() % m_structures.count();

	//start at this random position
	for (int i=start; i < m_structures.count(); i++)
		if (m_structures.at(i).contains(terminal))
			return m_structures.at(i);

	//not found? ok start backwards
	for (int i=start-1; i > 0; i--)
		if (m_structures.at(i).contains(terminal))
			return m_structures.at(i);

	return QString(); //no sentence found
}

