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

class GrammarManager : public QObject {
Q_OBJECT
private:
	QStringList structures;
	WordListManager *wordlistManager;
	static GrammarManager *instance;

protected:
	GrammarManager();

public slots:
	void unknownWordClass(QString name);
public:

	static GrammarManager* getInstance();

	bool load();
	bool save();

	QStringList getStructures(QString terminal);
	QStringList getTerminals();

	void setStructures(QStringList structures);

	void renameTerminal(QString terminal, QString newName);


	QStringList getAllStructures() { return structures; }
	QStringList getExamples(QString word, QString terminal, int count=2, bool includeShadow=false);

};

#endif
