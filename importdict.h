//
// C++ Interface: importdict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICT_H
#define IMPORTDICT_H

#include "word.h"

#include <QThread>
#include <iostream>

#include "wiktionarydict.h"
/**
	\class ImportDict
	\brief The ImportDict class provides the functions to import e.g. Wiktionary dicts
	\author Peter Grasch
*/
class ImportDict : public QThread{
Q_OBJECT

private:
	QString pathToDict; //!< the path to the dictionary
	WordList *wordList; //!< The WordList we are creating
signals:
	void status(QString);
	void progress(int);
	void finished();
	void opened();
private slots:
	void loadProgress(int prog);
	void openingFinished();
public:
    ImportDict(QObject *parent=0);
	WordList* getWordList() { return wordList; }
    void run();
    void parseWordList(QString pathToDict);
    ~ImportDict();

};

#endif
