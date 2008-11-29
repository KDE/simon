/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#ifndef WORDLISTMANAGER_H
#define WORDLISTMANAGER_H


#include <speechmodelbase/word.h>
#include "simonmodelmanagement_export.h"
#include <QThread>
#include <QMutex>
#include <QStringList>
/**
 *	@class WordListManager
 *	@brief Manages the Wordlist, the grammar definitions and the vocabulary data
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class QTextStream;
class QString;

class MODELMANAGEMENT_EXPORT WordListManager : public QThread {

Q_OBJECT
signals:
	void wordlistChanged();
	void shadowListChanged();
	void tempWarning();
	void wordListCouldntBeLoaded();
	void shadowListCouldntBeLoaded();
// 	void status(const QString&);
// 	void progress(int cur, int max);
private:
	static WordListManager *instance;

	bool isTemp;
	QMutex wordListLock;
	QMutex shadowLock;

	WordList *wordlist;	//!< Holds the wordlist
	WordList *shadowList;	//!< this holds the word that are not in the vocabulary (unused)
	QStringList activeTerminals; //!< Cache for the terminals
	QStringList shadowTerminals; //!< Cache for the terminals

	bool mainDirty, shadowDirty;
	
	WordList* removeDoubles(WordList *in);
	WordList* readWordList(const QString& lexiconpath, const QString& vocabpath, QStringList &terminals, bool isShadowlist=false);
	QString* getTerminal(const QString& name, const QString& pronunciation, WordList *wlist);

private slots:
	void complainAboutPaths();
	void warnAboutTempWordList();
	void updateWordProbability();

protected:
	WordListManager();

public:
	static WordListManager* getInstance();

	void safelyInit();
	void run();

	WordList* getSimpleVocab();

	bool saveWordList(WordList *list, const QString& vocabFilename, const QString& lexiconFilename="");

	WordList* getWordList() { return this->wordlist; }
	inline WordList* getShadowList();
	QStringList getTerminals(bool includeShadow=true);

	QString getRandomWord(const QString& terminal, bool includeShadow=true);
	
	bool mainWordListContains(Word *word);
	bool mainWordListContainsStr(const QString& word);
	bool extraListContains(Word *word);
	bool extraListContainsStr(const QString& word);
	bool wordListContains(WordList *list, Word *word);
	bool wordListContainsStr(WordList *list, const QString& word);

	WordList* getShadowedWords(const QString& word, bool fuzzy=false);
	WordList* getMainstreamWords(const QString& word, bool fuzzy=false);
	WordList* searchForWords(WordList *list, const QString& word, bool fuzzy=false);
	
	WordList* getWords(const QString& word, bool includeShadow, bool fuzzy=false, bool keepDoubles=false);

	WordList* mergeLists(WordList *a, WordList *b, bool keepDoubles=true);

	Word* getWord(const QString& word, const QString& pronunciation, const QString& terminal, bool &isShadowed);
	int getWordIndex(WordList *list, bool &found, const QString& word, const QString& pronunciation="", const QString& terminal="");

	WordList* getWordsByTerminal(const QString& terminal, bool includeShadow=false);
	void addWords(WordList *list, bool isSorted=false, bool shadow=false);
	bool save();
	bool moveToShadow(Word *w);
	bool deleteCompletely(Word *w, bool shadowed);

	WordList* sortList(WordList* list);

	void renameTerminal(const QString& from, const QString& to, bool includeShadow);

	bool refreshWordListFiles(const QByteArray& simpleVocab,
			const QByteArray& activeVocab, const QByteArray& activeLexicon);
	bool refreshShadowListFiles(const QByteArray& vocab);

	bool initShadowList();
	bool initWordList();

	~WordListManager();

};

#endif
