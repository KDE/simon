//
// C++ Interface: renameterminal
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENAMETERMINAL_H
#define RENAMETERMINAL_H

#include <QThread>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class GrammarManager;
class WordListManager;
class RenameTerminal : public QThread
{
Q_OBJECT
signals:
	void progress(int);
	void done();
private:
	WordListManager *wordListManager;
	GrammarManager *grammarManager;
	QString oldName, newName;
	bool includeShadow, includeGrammar;
public:
    RenameTerminal(QObject* parent, WordListManager*, GrammarManager*);
	void setOldName(QString old) { oldName = old; }
	void setNewName(QString newName) { this->newName = newName; }
	void setIncludeShadow (bool includeShadow) 
	{ this->includeShadow = includeShadow; }
	void setIncludeGrammar (bool includeGrammar) 
	{ this->includeGrammar = includeGrammar; }
    ~RenameTerminal();

    void run();

};

#endif
