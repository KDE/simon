/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef IMPORTDICT_H
#define IMPORTDICT_H

#define HADIFIXBOMP 1
#define WIKTIONARY 2
#define LEXICON 3

#include "../word.h"
#include <QThread>

class Dict;

/**
 *	\class ImportDict
 *	\brief The ImportDict class provides the functions to import e.g. Wiktionary dicts
 *	\author Peter Grasch
 *	\version 0.1
 */
class ImportDict : public QThread{
Q_OBJECT

private:
	QString pathToDict; //!< the path to the dictionary
	int type;
	Dict *dict;
	bool deleteFileWhenDone;
signals:
	void status(QString);
	void progress(int);
	void finished(WordList*);
	void opened();
private slots:
	void loadProgress(int prog);
	void openingFinished();
public slots:
	void deleteDict();
public:
    ImportDict(QObject *parent=0);
    void run();
    void parseWordList(QString pathToDict, int type, bool deleteFileWhenDone=false);
    ~ImportDict();

};

#endif
