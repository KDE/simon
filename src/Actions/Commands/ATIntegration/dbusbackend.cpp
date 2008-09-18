//
// C++ Implementation: dbusbackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "dbusbackend.h"

#include <QtDBus> //FIXME: where the hell resides qDBusRegisterMetaType?
#include <QDBusConnection>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QDBusArgument>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusInterface>
#include <QDBusConnectionInterface>


#include <QMetaType>
#include <KMessageBox>
#include <KLocalizedString>
#include "atobject.h"
#include "../../../SimonLib/Settings/settings.h"

DBusBackend::DBusBackend(QObject* parent): ATBackend(parent)
{
	qDBusRegisterMetaType<ATOLocation>();
	qDBusRegisterMetaType<ATOPosition>();

	iface =0;
	currentWindow=0;
	
	this->finalClasses = buildFinalClasses();
}

void DBusBackend::startMonitoring()
{
	if (iface) return; //already running
	stopMonitoring();
	
	iface = QDBusConnection::sessionBus().interface();
	connect(iface, SIGNAL(serviceRegistered(QString)),
		this, SLOT(serviceRegistered(QString)));
	connect(iface, SIGNAL(serviceUnregistered(QString)),
		this, SIGNAL(removeService(QString)));
	connect(iface, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
		this, SLOT(serviceOwnerChanged(QString,QString,QString)));
	
	
	
	//reading currently running applications
	QDBusReply<QStringList> list = iface->registeredServiceNames();
	
	if (!list.isValid())
	{
		KMessageBox::error(0, i18n("Konnte Liste der verfügbaren DBus Services nicht empfangen"));
		return;
	}
	
	processServices(list.value());
}

void DBusBackend::stopMonitoring()
{
	if (iface)
		iface->deleteLater();
}

bool DBusBackend::processServices(QStringList services)
{
	for (int i=0; i < services.count(); i++)
	{
		QString service = services[i];
		ATObject *root = readGui(service);
		if (root)
			emit objectFound(root);
	}
	return true;
}


/**
 * \brief Reads the gui
 * @param service The service to parse
 * @return The root element (the app. itself)
 */
ATObject* DBusBackend::readGui(QString service)
{
	ignoredSubmenue=0;

	ATObject *root = new ATObject(0, service, "Application");
	this->currentRoot=root;
	this->currentWindow=0;
	
	QStringList windows = getAccessibleWindowNames(service);
	for (int i=0; i < windows.count(); i++)
		parseObject(service, "/org/freedesktop/accessibility/"+windows[i], root);
	


	//TODO: submenues werden ignoriert
// 	if (ignoredSubmenue != 0) {
// 		KMessageBox::error(0, i18n("%1 Submenüs wurden ignoriert;\n\n(Submenüs sind derzeit nicht unterstützt)", ignoredSubmenue));
// 	}
	if (root->children().count()==0) return 0;
	return root;
}






void DBusBackend::serviceRegistered(QString service)
{
	if (service == QDBusConnection::sessionBus().baseService())
		return;
	
	//TODO: hardcoded mainwindow
	QString name = getName(service, "/org/freedesktop/accessibility/MainWindow", true);
	if (!name.isEmpty() && !(name == "\"\" ()"))
	{
		QStringList applications = QStringList() << service;
		QStringList applicationNames = QStringList() << name;
		emit servicesFound(applications, applicationNames);
	} else emit status(i18n("Konnte %1 nicht auflösen...", service));
}



void DBusBackend::serviceOwnerChanged(const QString &name, const QString &oldOwner,
			const QString &newOwner)
{
	if (!oldOwner.isEmpty())
		emit removeService(oldOwner);
	if (!newOwner.isEmpty())
		serviceRegistered(name);
}



// <omfg>
QDBusArgument &operator<<(QDBusArgument &argument, const ATOLocation &loc)
{
	argument.beginStructure();
	argument << loc.x << loc.y << loc.width << loc.height;
	argument.endStructure();
	return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ATOLocation &loc)
{
	argument.beginStructure();
	argument >> loc.x >> loc.y >> loc.width >> loc.height;
	argument.endStructure();
	return argument;
}

//ATOPosition
QDBusArgument &operator<<(QDBusArgument &argument, const ATOPosition &pos)
{
	argument.beginStructure();
	argument << pos.x << pos.y;
	argument.endStructure();
	return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ATOPosition &pos)
{
	argument.beginStructure();
	argument >> pos.x >> pos.y;
	argument.endStructure();
	return argument;
}
// <omfg />


QString DBusBackend::getName(QString service, QString path, bool descriptive)
{
	QDBusReply<QVariant> kdeName;
	if (descriptive)
	{
		QDBusInterface oldSvc(service, "/MainApplication",
				"org.freedesktop.DBus.Properties");
		kdeName = oldSvc.call("Get",
				"com.trolltech.Qt.QCoreApplication", "applicationName");
	}

	QDBusInterface svc(service, path,
			   "org.freedesktop.accessibleobject");
	QDBusReply<QString> name = svc.call("name");
	if (descriptive)
		return QString("\"%1\" (%2)").arg(name.value()).arg(kdeName.value().toString());
	else return name.value();
}

QString DBusBackend::getDescription(QString service, QString path)
{
	QDBusInterface svc(service, path,
			   "org.freedesktop.accessibleobject");
	QDBusReply<QString> desc = svc.call("description");
	return desc.value();
}

QString DBusBackend::getClassName(QString service, QString path)
{
	QDBusInterface svc(service, path,
			   "org.freedesktop.accessibleobject");
	QDBusReply<QString> className = svc.call("className");
	return className.value();
}

ATOLocation DBusBackend::getLocation(QString service, QString path, bool absolute)
{
	QDBusInterface svc(service, path,
			"org.freedesktop.accessibleobject");
	QDBusReply<ATOLocation> aTOLocationReply = svc.call("location");
	ATOLocation location = aTOLocationReply.value();

	if (!absolute)
	{
		//fetch non-absolute x/y from LocationInParent
		//and replace x / y
		QDBusInterface svc2(service, path,
				"org.freedesktop.accessiblecomponent");
		QDBusReply<ATOPosition> posReply = svc2.call("locationInParent");
		ATOPosition pos = posReply.value();
		location.x = pos.x;
		location.y = pos.y;
		
	}
	return location;
}

QString DBusBackend::getMenuName(QString service, QString path, int index)
{
	QDBusInterface svc(service, path,
			   "org.freedesktop.accessibleAction");
	QDBusReply<QString> name = svc.call("name", index);
	return name.value();
}

QStringList DBusBackend::getMenuNames(QString service, QString path)
{
	QStringList names;
	int i=0;
	QString currentName = getMenuName(service, path, i);
	while (!currentName.isEmpty()) {
		names << currentName;
		currentName = getMenuName(service, path, ++i);
	}
	return names;
}


void DBusBackend::handleMenuBar(QString service, QString path)
{
	//this Menu is
	//
	// |File|   |Settings|   |Help|
	
	if (!currentWindow) return;
	ATOMenu *thisMenu = new ATOMenu;
	thisMenu->type=MenuBar;
	thisMenu->geometry = getLocation(service,path, true);
			
	QStringList thisMenuNames = getMenuNames(service,path);
	for (int i=0; i < thisMenuNames.count(); i++)
	{
		ATOMenu *MenuEntry = new ATOMenu;
		MenuEntry->type=Entry;
		MenuEntry->title = thisMenuNames[i];
		thisMenu->actions.append(MenuEntry);
	}
	currentActionIndex=0;
	
	currentWindow->addMenu(thisMenu);
	currentMenu = thisMenu;
}

void DBusBackend::handleMenuEntry(QString service, QString path)
{
	if (currentMenu->type==MenuBar || currentMenu->type==ToolBar)
	{
				//this ATOMenu is 
		//
				//|File|
				//+-------------+
				//+ New         +
				//+ Quit        +
				//+-------------+
		ATOMenu thisATOMenu;
		thisATOMenu.type=Entry;
		if (currentActionIndex==0) {
			currentActionIndex++;
			return;
		}
				
		QStringList thisATOMenuNames = getMenuNames(service,path);
		ATOLocation loc = getLocation(service, path, true /*false*/);
		if ((loc.x == 0) && (loc.y==0))
		{ 	//THIS IS A SUBATOMenu!
				  //SKIP!
			ignoredSubmenue++;
			return;
		}
		for (int i=0; i < thisATOMenuNames.count(); i++)
		{
			ATOMenu *ATOMenuEntry = new ATOMenu;
			ATOMenuEntry->type=Entry;
			ATOMenuEntry->title = thisATOMenuNames[i];
			currentMenu->actions[currentActionIndex-1]->actions.append(ATOMenuEntry);
		}
		currentActionIndex++;
	} else
	{
		KMessageBox::error(0, i18n("Jackpot!"), i18n("You encountered an IMPOSSIBLE error!\n\nHave a cookie!"));
	}
}

ATObjectList* DBusBackend::parseObject(QString service, QString path, ATObject *parent)
{
	bool qtWorkArounds = Settings::getB("GuiRecognition/QtWorkarounds");
	
	ATObjectList* objects = new ATObjectList();
	QString thisClassName = getClassName(service, path);
	QString thisName = getName(service, path);
	
	ATObject* thisObject = new ATObject(parent,
				thisName, 
				thisClassName, 
    				getDescription(service, path));
	
	ATOLocation loc = this->getLocation(service, path, true /*false*/ );
	thisObject->setGeometry(loc.x, loc.y, loc.width, loc.height);
	
	if (parent==currentRoot) currentWindow = thisObject;
	objects->append(thisObject);
	
	
	if (qtWorkArounds)
	{
		if ((thisClassName == "QMenuBar") || (thisClassName=="KMenuBar"))
		{
			handleMenuBar(service, path);
			return objects;
		}
		if (thisClassName == "QMenu")
		{
			handleMenuEntry(service, path);
			return objects;
		}
	}

	if (qtWorkArounds && 
		finalClasses.contains(thisClassName))
		return objects;

	QDBusInterface introspect(service, path,
				  "org.freedesktop.DBus.Introspectable");
	QDBusReply<QString> exportedObjects = introspect.call("Introspect");

	QDomDocument doc;
	doc.setContent(exportedObjects);

	QDomElement elem = doc.documentElement();

	if (!elem.isNull()) 
		elem = elem.firstChildElement("node");

	QString name;
	while (!elem.isNull())
	{
		name = elem.attribute("name");
		if (!elem.isNull())
		{
			ATObjectList* children = parseObject(service, path+"/"+name, thisObject);
			for (int i=0; i<children->count(); i++)
				objects->append(children->at(i));
		}
		elem = elem.nextSiblingElement("node");
	}
	return objects;
}

QStringList DBusBackend::buildFinalClasses()
{
	QStringList final;
	if (Settings::getB("GuiRecognition/QtWorkarounds"))
	{
		final << "QListView";
		final << "KListWidget";
		final << "QTableView";
		final << "QTableWidget";
		final << "QRubberBand";
	}
	return final;
}

QStringList DBusBackend::getAccessibleWindowNames(QString service)
{
	QStringList windows;
	QDBusInterface introspect(service, "/org/freedesktop/accessibility",
				  "org.freedesktop.DBus.Introspectable");
	QDBusReply<QString> exportedWindows = introspect.call("Introspect");

	QDomDocument doc;
	doc.setContent(exportedWindows);
	QDomElement elem = doc.documentElement();

	if (!elem.isNull()) 
		elem = elem.firstChildElement("node");

	while (!elem.isNull())
	{
		if (!elem.attribute("name").isEmpty())
		{
			windows << elem.attribute("name");
		}
		elem = elem.nextSiblingElement("node"); 
	}
	return windows;
}


QStringList DBusBackend::getWindows(QString service)
{
	QString applicationName = getName(service);
	QDBusInterface introspect(service, "/"+applicationName,
			"org.freedesktop.DBus.Introspectable");
	QDBusReply<QString> exportedWindows = introspect.call("Introspect");

	QDomDocument doc;
	doc.setContent(exportedWindows);

	QDomElement elem = doc.documentElement();

	if (!elem.isNull()) 
		elem = elem.firstChildElement("node");

	QStringList windows;
	QString name;
	while (!elem.isNull())
	{
		name = elem.attribute("name");
		while (!elem.isNull() && !((elem.tagName() == "node") && 
			(name.startsWith("MainWindow") /*|| (name =="Dolphin")*/ || name.endsWith("_Shell"))))
		{
			elem = elem.nextSiblingElement("node");
			name = elem.attribute("name");
		}
		if (!elem.isNull()) windows << name;

		elem = elem.nextSiblingElement("node");
	}
	return windows;
}


QStringList DBusBackend::fetchActions(QString serviceName, QString windowName)
{
	QString applicationName = getName(serviceName);

	QDBusInterface svc(serviceName, "/"+applicationName+"/"+windowName,
			"org.kde.KMainWindow");
	QDBusReply<QStringList> actions = svc.call("actions");
	
	return actions.value();
}


QString DBusBackend::getWindowName(QString service, QString window)
{
	QString applicationName = getName(service);

	QDBusInterface svc(service, "/"+applicationName+"/"+window,
			"org.freedesktop.DBus.Properties");

	QDBusReply<QVariant> title = svc.call("Get", "com.trolltech.Qt.QWidget", "windowTitle");
	return title.value().toString();
}

void DBusBackend::execute(QString service, QString window, QString action)
{
	QString name = getName(service);
	QDBusInterface svc(service, "/"+name+"/"+window,
			       "org.kde.KMainWindow");
	svc.call("activateAction", action);
}


DBusBackend::~DBusBackend()
{
}


