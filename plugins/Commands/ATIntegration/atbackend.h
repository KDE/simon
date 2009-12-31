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

#ifndef SIMON_ATBACKEND_H_5CC22287EB184828A6542992D82EAA02
#define SIMON_ATBACKEND_H_5CC22287EB184828A6542992D82EAA02

#include <QObject>

class ATObject;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ATBackend : public QObject
{
Q_OBJECT

signals:
	/**
	 * \brief Emits the found object in a tree-like structure
	 *
	 * Once we found something useful on the AT-Bus we are using, we emit it using this signal.
	 * It is defined as follows:
	 * 	* QList of widgets
	 * 	* Each widget has a parent which builds sort of a "tree" of widgets
	 * 	* The highest widget has a pseudo element as his parent, which is also the first in the list;
	 * 	  It represents the application itself (because there could be more than one "main window" (-> e.g. Dolphin)
	 * 	  Its trigger is the applicationname
	 *
	 * @param name 
	 * @param widgets 
	 */
	void objectFound(ATObject* widgets);
	void objectDeleted(ATObject *widget);
	void objectFocused(ATObject *widget);
public:
	ATBackend(QObject* parent) : QObject(parent) {}
	virtual void startMonitoring()=0;
	virtual void stopMonitoring()=0;

    virtual ~ATBackend() {}

};

#endif
