//
// C++ Implementation: wordlistmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wordlistmanager.h"

WordListManager::WordListManager(QString path)
{
	this->wordlist = new WordList();
	readVocab(path);
}

WordList* WordListManager::readVocab(QString path)
{
	wordlist->clear();
	//read the vocab
	//replace this dummy with the xml code
	
	wordlist->append(new Word("Birne", "b I R n @", "Nomen", 6));
	wordlist->append(new Word("Eins", "aI n @", "Nomen", 1));
	wordlist->append(new Word("Zwei", "t s v aI", "Nomen", 2));
	wordlist->append(new Word("Magarita", "m a: g a: R i: t a:", "Nomen", 4));
	wordlist->append(new Word("Tonno", "t o: n o:", "Nomen", 2));
	wordlist->append(new Word("Drei", "d R aI", "Nomen", 3));
	wordlist->append(new Word("Vier", "f i: 6", "Nomen", 4));
	wordlist->append(new Word("Fünf", "f Y n f", "Nomen", 5));
	wordlist->append(new Word("Sechs", "z E k s", "Nomen", 6));
	wordlist->append(new Word("Sieben", "z i: b @ n", "Nomen", 7));
	wordlist->append(new Word("Acht", "a x t", "Nomen", 8));
	wordlist->append(new Word("Neun", "n OY n", "Nomen", 9));
	
	//end dummy
	return this->wordlist;
}

WordListManager::~WordListManager()
{
}


