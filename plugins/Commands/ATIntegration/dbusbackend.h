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

#ifndef SIMON_DBUSBACKEND_H_CE8C354073E44D66B8AE75099BE48B70
#define SIMON_DBUSBACKEND_H_CE8C354073E44D66B8AE75099BE48B70

#include "atbackend.h"
#include "ato.h"
#include "atobject.h"
#include <QStringList>

class QDBusConnectionInterface;
/**
	@author Peter Grasch <bedahr@gmx.net>
 */
class DBusBackend : public ATBackend
{
		Q_OBJECT
	
	
	signals:
		void status(QString);
		void servicesFound(QStringList apps, QStringList names);
		void removeService(QString);

	private slots:
		void serviceRegistered(QString service);
		void serviceOwnerChanged(const QString &name, const QString &oldOwner,
					 const QString &newOwner);
	private:
		ATOMenu *currentMenu;
		QDBusConnectionInterface *iface;
		int currentActionIndex;
		int ignoredSubmenue;
		ATObject *currentWindow, *currentRoot;
		QStringList finalClasses; // classes which children should not be inspected
					  // -> Quirks-Modus
		
		QStringList getAccessibleWindowNames(QString service);
		QString getName(QString service, QString pathpath="/org/freedesktop/accessibility/MainWindow", bool descriptive=false);
		QString getDescription(QString service, QString path);
		QString getClassName(QString service, QString path);
		
		QStringList buildFinalClasses();
		
		//menu
		void handleMenuBar(QString service, QString path);
		void handleMenuEntry(QString service, QString path);
		QString getMenuName(QString service, QString path, int index);
		QStringList getMenuNames(QString service, QString path);
		QList<ATObject*>* parseObject(QString service, QString path, ATObject *parent=0);


	public:
		QStringList fetchActions(QString serviceName, QString windowName);
		QStringList getWindows(QString service);
		QString getWindowName(QString service, QString window);

		void execute(QString service, QString window, QString action);


		ATOLocation getLocation(QString service, QString path, bool absolute=false);
	
	
	
	
	
		DBusBackend ( QObject* parent );

		void startMonitoring();
		void stopMonitoring();
		bool processServices(QStringList services);
		ATObject* readGui(QString service);

		~DBusBackend();

};

#endif
