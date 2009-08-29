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


#ifndef SCENARIO_H
#define SCENARIO_H
#include <QString>
#include <QList>
#include "speechmodelbase_export.h"

class ScenarioObject;
class Author;
class Action;
class VersionNumber;
class Grammar;
class Vocabulary;
class TrainingText;

class SPEECHMODELBASE_EXPORT Scenario{

private:
	QString scenarioId;

	int version;

	QString name;
	QString licence;
	QList<Author*> authors;
	VersionNumber *simonMinVersion, *simonMaxVersion;
	Vocabulary *vocabulary;
	Grammar *grammar;
	QList<Action*> actions;
	QList<TrainingText*> texts;

public:
	Scenario(const QString& scenarioId);

	bool init(QString path=QString());
	bool save(QString path=QString());

	~Scenario();

};

#endif
