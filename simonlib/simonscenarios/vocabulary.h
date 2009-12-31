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
#include <QAbstractItemModel>

#include "simonmodelmanagement_export.h"

#include <simonscenarios/word.h>

class QDomElement;
class QDomDocument;

class MODELMANAGEMENT_EXPORT Vocabulary : public QAbstractItemModel
{

private:
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;


protected:
	bool m_resolveProbability;
	QStringList terminals; //terminal cache
	QList<Word*> m_words;

	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void sortWords();
	bool appendWordRaw(Word* w);
	bool insertWordRaw(int pos, Word* w);


public:
	enum MatchType {
	    ExactMatch=1,
	    SimilarMatch=2,
	    ContainsMatch=4
    };

	Vocabulary();
	bool deSerialize(const QDomElement&);
	QDomElement serialize(QDomDocument *doc);
	static QDomElement createEmpty(QDomDocument *doc);


	/**
	 * Adds the specified words to the vocabulary
	 */
	virtual bool addWords(QList<Word*> *w);
	virtual bool addWord(Word* w);

	bool removeWord(Word* w, bool deleteWord=true);

	QString getRandomWord(const QString& terminal);
	bool containsWord(const QString& word);
	bool containsWord(const QString& word, const QString& terminal, const QString& pronunciation);

	int wordCount() { return m_words.count(); }
	QList<Word*> getWords() { return m_words; }
	virtual ~Vocabulary();

	bool renameTerminal(const QString& from, const QString& to);

	QStringList getTerminals();

	QList<Word*> findWords(const QString& name, Vocabulary::MatchType type);
	QList<Word*> findWordsByTerminal(const QString& terminal);

	void clear();
	void deleteAll();

enum VocabularyType {
	ShadowVocabulary = 0,
	ActiveVocabulary = 1
};


};

#endif
