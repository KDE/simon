//
// C++ Interface: importgrammar
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTGRAMMAR_H
#define IMPORTGRAMMAR_H

#include <QThread>
#include <QStringList>
#include "word.h"
class WordListManager;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportGrammar : public QThread
{
Q_OBJECT
signals:
	void status(QString);
	void fileProgress(int /*this file progress*/,
			int /*this file max*/);
	void allProgress(int /*all progress*/,
			int /*all max*/);

	void grammarCreated(QStringList grammar);
private:
	QStringList files;
	bool includeUnknown;
	QStringList importFile(QString path);
	QStringList terminals(WordList *in);
	QStringList readFile(QString path);

public:
    ImportGrammar(QObject* parent);

	void run();
	void setFiles(QStringList files) { this->files = files; }
	void setIncludeUnknown(bool include) { this->includeUnknown = include; }

    ~ImportGrammar();

};

#endif
