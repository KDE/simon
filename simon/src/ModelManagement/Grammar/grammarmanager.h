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


#ifndef GRAMMARMANAGER_H
#define GRAMMARMANAGER_H

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
#include <QStringList>
class WordListManager;

class GrammarManager : public QObject {
Q_OBJECT
private:
// 	QStringList structures;
	static GrammarManager *instance;

protected:
	GrammarManager();

public slots:
	void unknownWordClass(QString name);
public:

	static GrammarManager* getInstance();

// 	bool load();
// 	bool save();

	QStringList getStructures(QString terminal);
	QStringList getTerminals();

	void renameTerminal(QString terminal, QString newName);

	QStringList getExamples(QString word, QString terminal, int count=2, bool includeShadow=false);

};

#endif
