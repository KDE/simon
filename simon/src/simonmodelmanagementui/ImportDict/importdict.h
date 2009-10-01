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


#ifndef SIMON_IMPORTDICT_H_5866CB29C94741DC925C1177DD83CE41
#define SIMON_IMPORTDICT_H_5866CB29C94741DC925C1177DD83CE41


#include "dict.h"
#include <speechmodelbase/word.h>
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
	QString encoding; //!< the encoding of the dictionary
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
    void parseWordList(QString pathToDict, QString encoding, int type, bool deleteFileWhenDone=false);
    ~ImportDict();

};

#endif
