//
// C++ Interface: dbusbackend
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBUSBACKEND_H
#define DBUSBACKEND_H

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
		QStringList finalClasses; // classes which childs should not be inspected
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
