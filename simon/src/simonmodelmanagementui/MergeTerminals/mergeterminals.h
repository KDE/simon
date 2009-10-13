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


#ifndef MERGETERMINALS_H
#define MERGETERMINALS_H

#include <QThread>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/

class MergeTerminals : public QThread
{
		Q_OBJECT
	signals:
		void status ( QString );
		void progress ( int cur, int max );
		void done();
	private:
		QString newName, terminalA, terminalB;
		bool includeShadow, includeGrammar;
	public:
		void run();

		void setNewName ( QString newName ) { this->newName = newName.trimmed(); }
		void setTerminalA ( QString terminalA ) { this->terminalA = terminalA.trimmed(); }
		void setTerminalB ( QString terminalB ) { this->terminalB = terminalB.trimmed(); }
		void setIncludeShadow ( bool includeShadow ) { this->includeShadow= includeShadow; }
		void setIncludeGrammar ( bool includeGrammar) { this->includeGrammar= includeGrammar; }

		MergeTerminals ( QObject* parent );

		~MergeTerminals();

};

#endif
