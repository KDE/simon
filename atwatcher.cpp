//
// C++ Implementation: atwatcher
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "atwatcher.h"
#include "atobject.h"

#ifdef linux
#include "dbusbackend.h"
#endif

#ifdef __WIN32
#include "msaabackend.h"
#endif

#include <QStack>
#include <QDebug>


ATWatcher::ATWatcher ( QObject* parent ) : QObject ( parent )
{
// 	currentObjectTree=0;
	focusedWindow=0;
#ifdef linux
	backend = new DBusBackend(this);
#endif
#ifdef __WIN32
	backend = new MSAABackend(this);
#endif
	
	connect(backend, SIGNAL(objectFound(ATObject*)), this, SLOT(addObject(ATObject*)));
	connect(backend, SIGNAL(objectFocused(ATObject*)), this, SLOT(translateFocusToWindow(ATObject*)));
	connect(backend, SIGNAL(objectDeleted(ATObject*)), this, SLOT(deleteObject(ATObject*)));
	backend->startMonitoring();
}

/**
 * \brief Translates the given object to its application window
 * \author Peter Grasch
 * Instead of traversing the object tree we just iterate as long as there is a parent, which has a parent, which has a parent,...
 * @param selectedObject 
 */
void ATWatcher::translateFocusToWindow(ATObject* selectedObject)
{
	if (!selectedObject || !selectedObject->parent())
	{
		//!< Because we can't focus a "application" - only there windows
		//!< and the window would have the application as parent
		qDebug() << "received garbage";
		return;
	}
	ATObject *application= (ATObject*) selectedObject->parent();
	ATObject *window=(ATObject*) selectedObject;
	
	while (application->parent())
	{
		window = application;
		application = (ATObject*) application->parent();
	}
	if (applications.contains(selectedObject))
		qDebug() << "Window not in window list";
	
	focusedWindow = window;
	focusedApplication = application;
}

void ATWatcher::addObject(ATObject *newObject)
{
	qDebug() << "Adding new object: " << newObject;

	applications.append(newObject);
}

void ATWatcher::deleteObject(ATObject *oldObject)
{
	applications.removeAll(oldObject);
	oldObject->deleteLater();
}

/**
 * \brief Triggers the widget with the given triggerstring
 * @param triggerString The trigger of the widget to trigger
 * @return True if we found the widget; else: false
 */
bool ATWatcher::trigger(QString triggerString)
{
	qDebug() << "execute: " << triggerString;
	QStack<ATObject*> objectsToSearch;
	
	//TODO: once the "currentWindow" is working, only push the current appwindow
	for (int i=0; i < applications.count(); i++)
		objectsToSearch.push(applications[i]);

	ATObject *current;
	while (!objectsToSearch.isEmpty())
	{
		//traverse the object tree and find the item
		current = objectsToSearch.pop();
		if (current->getName() == triggerString)
		{
			qDebug() << "FOUND IT! It is a " << current->getClassName();
			current->trigger();
			return true;
		}
		QObjectList childs = current->children();
		for (int i=0; i < childs.count(); i++)
			objectsToSearch.push((ATObject*) childs.at(i));
	}
		
	return false;
}


ATWatcher::~ATWatcher()
{
	backend->deleteLater();
}


