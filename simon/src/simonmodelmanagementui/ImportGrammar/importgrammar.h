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


#ifndef SIMON_IMPORTGRAMMAR_H_18BB1C74117E44A995D8DB8DA54E0988
#define SIMON_IMPORTGRAMMAR_H_18BB1C74117E44A995D8DB8DA54E0988

#include <QThread>
#include <QStringList>
#include <speechmodelbase/word.h>

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
	QString encoding;
	bool includeUnknown;
	QStringList importFile(QString path);
	QStringList terminals(WordList *in);
	QStringList readFile(QString path);

public:
    ImportGrammar(QObject* parent);

	void run();
	void setFiles(QStringList files) { this->files = files; }
	void setEncoding(const QString& encoding) { this->encoding = encoding; }
	void setIncludeUnknown(bool include) { this->includeUnknown = include; }

    ~ImportGrammar();

};

#endif
