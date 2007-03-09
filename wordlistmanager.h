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
#include "word.h"

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
public:
	WordListManager(QString path="words.xml");
	WordList* readVocab(QString path="words.xml");
	WordList* getWordList() { return wordlist; }

	~WordListManager();

};

#endif
