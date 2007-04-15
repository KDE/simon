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
#include <QList>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QtGlobal>
#include <QHash>
#include "word.h"
#include "modelmanager.h"

/**
 *	@class WordListManager
 *	@brief Manages the Wordlist, the grammar definitions and the vocabulary data
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */

typedef QHash<QString, QString> PromptsTable;

class WordListManager{

private:
	WordList *wordlist;	//!< Holds the wordlist
	
public:
	WordListManager(QString path="model/lexicon");
	WordList* readWordList(QString lexiconpath="model/lexicon", QString vocabpath="model/model.voca", QString promptspath="model/prompts");
	WordList* readVocab(QString vocabpath="model/model.voca");
	PromptsTable* readPrompts(QString promptspath="model/prompts");
	QString getTerminal(QString name, QString pronunciation, WordList *wlist);
	int getProbability(QString name, PromptsTable *promptsTable);
	void trainList();
	WordList* getWordList() { return wordlist; }

	~WordListManager();

};

#endif
