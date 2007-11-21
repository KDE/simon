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


class WordListManager{

private:
	WordList *wordlist;	//!< Holds the wordlist
	ModelManager *modelManager; //!< Manages the language- and acoustic model
//	WordList *extralist;	//!< this holds the word that are not in the vocabulary (unused)
	QString lexiconPath, vocabPath;
	
public:
// 	WordList* getExtraWords() { return extralist; }
	
	WordList* removeDoubles(WordList *in);
	WordListManager(QString lexiconPath="model/lexicon", QString vocabPath="model/model.voca");
	WordList* readWordList(QString lexiconpath="model/lexicon", QString vocabpath="model/model.voca", QString promptspath="model/prompts");
	WordList* readVocab(QString vocabpath="model/model.voca");
	PromptsTable* readPrompts(QString promptspath="model/prompts");
	QString* getTerminal(QString name, QString pronunciation, WordList *wlist);
	int getProbability(QString name, PromptsTable *promptsTable);
	void trainList();
	WordList* getWordList() { return wordlist; }
	void addWords(WordList *list, bool isSorted=false);
	bool save( QString lexiconFilename="", QString vocabFilename="" );
	WordList* sortList(WordList* list);

	bool compileModel();
	~WordListManager();

};

#endif
