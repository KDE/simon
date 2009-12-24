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
	if (m_structures.count() == 0) return QString();

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


QVariant Grammar::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	if (role == Qt::DisplayRole) 
	{
		switch (index.column())
		{
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
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (column)
		{
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
		if (!addStructure(structure, false /* don't save */))
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

	return parentScenario->save();
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

