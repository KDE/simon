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
#include <QObject>
#include <KIcon>
#include "speechmodel.h"
#include "vocabulary.h"
#include "scenariomanager.h"
#include "simonmodelmanagement_export.h"

class ScenarioObject;
class Author;
class Action;
class Word;
class VersionNumber;
class Grammar;
class ActiveVocabulary;
class TrainingText;

class MODELMANAGEMENT_EXPORT Scenario : public QObject {
	Q_OBJECT

signals:
	void changed(Scenario*);

private:
	QString m_scenarioId;

	int m_version;

	QString m_name;
	QString m_iconSrc;
	QString m_licence;
	QList<Author*> m_authors;
	VersionNumber *m_simonMinVersion, *m_simonMaxVersion;
	ActiveVocabulary *m_vocabulary;
	Grammar *m_grammar;
	QList<Action*> m_actions;
	QList<TrainingText*> m_texts;

	bool emitChangedIfTrue(bool input);

public:
	Scenario(const QString& scenarioId);

	KIcon icon() { return KIcon(m_iconSrc); }
	QString name() { return m_name; }
	QString id() { return m_scenarioId; }

	bool addWords(QList<Word*>* w);
	bool addWord(Word* w);
	bool removeWord(Word* w);

	QList<Word*> findWords(const QString& name, Vocabulary::MatchType type);
	QStringList getExampleSentences(const QString& name, const QString& terminal, int count);
	QString fillGrammarSentenceWithExamples(const QString& terminalSentence, bool &ok, const QString& toDemonstrate=QString(), 
			const QString& toDemonstrateTerminal=QString());

	QStringList getTerminals(SpeechModel::ModelElements elements);
	bool renameTerminal(const QString& terminal, const QString& newName, SpeechModel::ModelElements affect);

	QString getRandomWord(const QString& terminal);
	bool containsWord(const QString& word);
	bool containsWord(const QString& word, const QString& terminal, const QString& pronunciation);

	bool init(QString path=QString());
	bool save(QString path=QString());

	ActiveVocabulary* vocabulary() { return m_vocabulary; }
	Grammar* grammar() { return m_grammar; }

	~Scenario();

};

#endif
