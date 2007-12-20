//
// C++ Interface: grammarmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAMMARMANAGER_H
#define GRAMMARMANAGER_H

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
#include <QStringList>
class WordListManager;

class GrammarManager{
private:
	QStringList structures;
	WordListManager *wordlistManager;
public:
    GrammarManager(WordListManager*);

	bool load();
	bool save();

	QStringList getStructures(QString terminal);
	QStringList getTerminals();
	QStringList getExamples(QString word, QString terminal, int count=2);

    ~GrammarManager();

};

#endif
