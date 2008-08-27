//
// C++ Interface: atobject
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
