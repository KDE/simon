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
#include <QMutex>
#include "simonmodelmanagement_export.h"

class WordListManager;

class MODELMANAGEMENT_EXPORT GrammarManager : public QObject {
Q_OBJECT

signals:
	void structuresChanged();

private:
	static GrammarManager *instance;
	QMutex structuresLock;
	QStringList structures;

protected:
	GrammarManager();

public:
	QStringList getStructures();
	void setStructures(const QStringList& structures);

	static GrammarManager* getInstance();

	bool load();
	bool save();

	QStringList getStructures(const QString& terminal);
	QStringList getTerminals();

	void renameTerminal(QString terminal, const QString& newName);

	QStringList getExamples(const QString& word, const QString& terminal, int count=2, bool includeShadow=false);

	bool refreshFiles(const QByteArray& grammarStructures);

};

#endif
