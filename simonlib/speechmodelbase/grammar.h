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

#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <QStringList>

#include "speechmodelbase_export.h"
#include <simonscenariobase/scenarioobject.h>

class Scenario;

class SPEECHMODELBASE_EXPORT Grammar : public ScenarioObject
{

private:
	QStringList m_structures;

protected:
	Grammar(Scenario *parent);

public:
	static Grammar* createGrammar(Scenario *parent, const QDomElement&);
	bool deSerialize(const QDomElement&);
	QDomElement serialize(QDomDocument *doc);

	int structureCount() { return m_structures.count(); }
};

#endif
