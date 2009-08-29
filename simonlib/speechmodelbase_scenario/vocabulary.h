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


#ifndef VOCABULARY_H
#define VOCABULARY_H
#include <QString>
#include <QList>

#include "word.h"

#include "speechmodelbase_export.h"
#include <simonscenariobase/scenarioobject.h>

class SPEECHMODELBASE_EXPORT Vocabulary : public ScenarioObject
{

private:
	QList<Word*> m_words;

protected:
	Vocabulary(const QString& scenarioId);

public:
	static Vocabulary* createVocabulary(const QString& scenarioId, const QDomElement&);
	bool deSerialize(const QDomElement&);
	QDomElement serialize(QDomDocument *doc);

	int wordCount() { return m_words.count(); }
};

#endif
