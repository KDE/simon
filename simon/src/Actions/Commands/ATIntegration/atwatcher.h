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

#ifndef ATWATCHER_H
#define ATWATCHER_H

#include <QObject>

class ATBackend;
class ATObject;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/


/// \warning Was wenn von einer untest�tzten Applikation zu einer nicht unterst�tzten umgewechselt wird? was wird emmitted?
class ATWatcher : public QObject
{
		Q_OBJECT
	private:
		ATBackend* backend;
		ATObject *focusedApplication; //!< holds a pointer to the currently selected app.
		ATObject *focusedWindow; //!< holds a pointer to the currently opened window
		QList<ATObject*> applications;
		static ATWatcher *instance;
		
	private slots:
		void addObject(ATObject *newObject);
		void deleteObject(ATObject *oldObject);
		void translateFocusToWindow(ATObject* selectedObject);
	public slots:
		bool trigger(const QString &word);

	protected:
		ATWatcher ( QObject* parent=0 );

	public:
		void applySettings();
		static ATWatcher* getInstance() {
			if (!instance) instance = new ATWatcher();
			return instance;
		}

		~ATWatcher();

};

#endif
