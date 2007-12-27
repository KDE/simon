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
private:
	bool isTemp;
	WordList *wordlist;	//!< Holds the wordlist
	ModelManager *modelManager; //!< Manages the language- and acoustic model
	WordList *shadowList;	//!< this holds the word that are not in the vocabulary (unused)
	QStringList activeTerminals; //!< Cache for the terminals
	QStringList shadowTerminals; //!< Cache for the terminals
	TrainingManager *trainManager;

	bool mainDirty, shadowDirty;
	
	WordList* removeDoubles(WordList *in);
	WordList* readWordList(QString lexiconpath, QString vocabpath, QString promptspath, QStringList &terminals);
	WordList* readVocab(QString vocabpath);
	QString* getTerminal(QString name, QString pronunciation, WordList *wlist);

public:
	void safelyInit();
	void run();

	WordListManager(TrainingManager *trainManager);

	Word* getWord(QString word, QString pronunciation, QString terminal, bool &isShadowed);

	bool saveWordList(WordList *list, QString lexiconFilename, QString vocabFilename);

	WordList* getWordList() { return wordlist; }
	WordList* getShadowList();
	QStringList getTerminals(bool includeShadow=true);

	QString getRandomWord(QString terminal);

	WordList* getWords(QString word, bool includeShadow=true);
	void addWords(WordList *list, bool isSorted=false, bool shadow=false);
	bool save( QString lexiconFilename="", QString vocabFilename="", QString shadowLexiconFilename="", QString shadowVocabFilename="" );
	bool moveToShadow(Word *w);
	bool deleteCompletely(Word *w, bool shadowed);
	WordList* sortList(WordList* list);

	bool compileModel();
	~WordListManager();

};

#endif
