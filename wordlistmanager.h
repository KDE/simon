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
#include "word.h"
#include "modelmanager.h"

/**
 *	@struct vocab
 *	@brief Defines one entry in the vocabulary list
 *
 *	@version 0.1
 *	@date 10.03.2007
 *	@author Peter Grasch
 */

struct vocab {
	QString terminal;
	QString wordname;
	QString pronunciaton;
};

typedef QList<vocab> VocabList;

/**
 *	@class WordListManager
 *	@brief Manages the Wordlist, the grammar definitions and the vocabulary data
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */

class WordListManager{
private:
	WordList *wordlist;	//!< Holds all the Vocabulary
	VocabList *vocablist;
	
public:
	WordListManager(QString path="model/lexicon");
	WordList* readWordList(QString lexiconpath="model/lexicon", QString vocabpath="model/model.voca");
	WordList* getWordList() { return wordlist; }

	~WordListManager();

};

#endif
