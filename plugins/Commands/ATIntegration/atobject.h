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

#ifndef ATOBJECT_H
#define ATOBJECT_H

#include <QWidget>
class ATOMenu;

class ATObject;

typedef QList<ATObject*> ATObjectList;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ATObject : public QWidget
{
		Q_OBJECT
	private:
		QString className, name, description;
		QList<ATOMenu*> menuList;
	public:
		ATObject ( ATObject* parent, const QString& nam, const QString& cName, const QString& desc="" );
			
		void addMenu(ATOMenu* menu);
		QString getClassName() { return className; }
		QString getName() { return name; }
		QString getDescription() { return description; }
		QList<ATOMenu*> getMenuList() { return menuList; }
		void trigger();

		~ATObject();

};

#endif
