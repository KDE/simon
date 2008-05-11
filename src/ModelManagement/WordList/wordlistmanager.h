//
// C++ Interface: wordlistmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WORDLISTMANAGER_H
#define WORDLISTMANAGER_H

#include <QThread>
#include <QMutex>
#include "word.h"
#include "trainingmanager.h"
/**
 *	@class WordListManager
 *	@brief Manages the Wordlist, the grammar definitions and the vocabulary data
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class QMessageBox;
class QTextStream;
class QString;
class ModelManager;

class WordListManager : public QThread {

Q_OBJECT
signals:
	void wordlistChanged();
	void shadowListChanged();
	void tempWarning();
	void wordListCouldntBeLoaded();
	void shadowListCouldntBeLoaded();
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
	WordList* readWordList(QString lexiconpath, QString vocabpath, QString promptspath, QStringList &terminals, bool isShadowlist=false);
	WordList* readVocab(QString vocabpath);
	QString* getTerminal(QString name, QString pronunciation, WordList *wlist);

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


	bool saveWordList(WordList *list, QString lexiconFilename, QString vocabFilename);

	WordList* getWordList() { return this->wordlist; }
	WordList* getShadowList();
	QStringList getTerminals(bool includeShadow=true);

	QString getRandomWord(QString terminal, bool includeShadow=true);

	WordList* getShadowedWords(QString word, bool fuzzy=false);
	WordList* getMainstreamWords(QString word, bool fuzzy=false);
	WordList* searchForWords(WordList *list, QString word, bool fuzzy=false);
	
	WordList* getWords(QString word, bool includeShadow, bool fuzzy=false, bool keepDoubles=false);

	WordList* mergeLists(WordList *a, WordList *b, bool keepDoubles=true);

	Word* getWord(QString word, QString pronunciation, QString terminal, bool &isShadowed);
	int getWordIndex(WordList *list, bool &found, QString word, QString pronunciation="", QString terminal="");

	WordList* getWordsByTerminal(QString terminal, bool includeShadow=false);
	void addWords(WordList *list, bool isSorted=false, bool shadow=false);
	bool save( QString lexiconFilename="", QString vocabFilename="", QString shadowLexiconFilename="",
			 QString shadowVocabFilename="" );
	bool moveToShadow(Word *w);
	bool deleteCompletely(Word *w, bool shadowed);

	WordList* sortList(WordList* list);
    void writePrompts(QString line, QString promptspath="model/prompts");

    PromptsTable* readPrompts(QString promptspath="model/prompts");

	void renameTerminal(QString from, QString to, bool includeShadow);
	~WordListManager();

};

#endif
