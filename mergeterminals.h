//
// C++ Interface: mergeterminals
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MERGETERMINALS_H
#define MERGETERMINALS_H

#include <QThread>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class WordListManager;
class GrammarManager;

class MergeTerminals : public QThread
{
		Q_OBJECT
	signals:
		void status ( QString );
		void progress ( int cur, int max );
		void done();
	private:
		QString newName, terminalA, terminalB;
		WordListManager *wordListManager;
		GrammarManager *grammarManager;
		bool includeShadow, includeGrammar;
	public:
		void run();

		void setNewName ( QString newName ) { this->newName = newName.trimmed(); }
		void setTerminalA ( QString terminalA ) { this->terminalA = terminalA.trimmed(); }
		void setTerminalB ( QString terminalB ) { this->terminalB = terminalB.trimmed(); }
		void setIncludeShadow ( bool includeShadow ) { this->includeShadow= includeShadow; }
		void setIncludeGrammar ( bool includeGrammar) { this->includeGrammar= includeGrammar; }

		MergeTerminals ( WordListManager *wordListManager, GrammarManager *grammarManager, QObject* parent );

		~MergeTerminals();

};

#endif
