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

#include <QHash>
#include <QThread>
#include "word.h"
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


typedef QHash<QString, QString> PromptsTable;


class WordListManager : public QThread {

Q_OBJECT
signals:
	void wordlistChanged();
private:
	WordList *wordlist;	//!< Holds the wordlist
	ModelManager *modelManager; //!< Manages the language- and acoustic model
	WordList *shadowList;	//!< this holds the word that are not in the vocabulary (unused)
	
	WordList* removeDoubles(WordList *in);
	WordList* readWordList(QString lexiconpath, QString vocabpath, QString promptspath);
	WordList* readVocab(QString vocabpath);
	QString* getTerminal(QString name, QString pronunciation, WordList *wlist);

public:
	void run();

	WordListManager();

	Word getWord(int id) { return wordlist->at(id); }

	PromptsTable* readPrompts(QString pathToPrompts);
	int getProbability(QString name, PromptsTable *promptsTable);
	WordList* getWordList() { return wordlist; }
	WordList* getShadowList() { return shadowList; }
	void addWords(WordList *list, bool isSorted=false, bool shadow=false);
	bool save( QString lexiconFilename="", QString vocabFilename="" );
	WordList* sortList(WordList* list);

	bool compileModel();
	~WordListManager();

};

#endif
