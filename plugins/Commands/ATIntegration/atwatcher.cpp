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

#include "atwatcher.h"
#include "atobject.h"
#include "ato.h"

#ifdef Q_OS_UNIX
#include "dbusbackend.h"
#endif

#ifdef Q_OS_WIN
#include "msaabackend.h"
#endif

#include <QStack>

ATWatcher* ATWatcher::instance;

ATWatcher::ATWatcher(QObject* parent) : QObject(parent),
focusedWindow(0)
#ifdef Q_OS_UNIX
, backend(new DBusBackend(this))
#else
#ifdef Q_OS_WIN
, backend(new MSAABackend(this))
#endif
#endif
{
  // 	currentObjectTree=0;

  connect(backend, SIGNAL(objectFound(ATObject*)), this, SLOT(addObject(ATObject*)));
  connect(backend, SIGNAL(objectFocused(ATObject*)), this, SLOT(translateFocusToWindow(ATObject*)));
  connect(backend, SIGNAL(objectDeleted(ATObject*)), this, SLOT(deleteObject(ATObject*)));

  applySettings();
}


/**
 * \brief Starts / stops the monitoring according to the defined settings
 * \author Peter Grasch
 */
void ATWatcher::applySettings()
{
  // 	if (Settings::getB("GuiRecognition/SupportAT"))
  // 		backend->startMonitoring();
  // 	else backend->stopMonitoring();
}


/**
 * \brief Translates the given object to its application window
 * \author Peter Grasch
 * Instead of traversing the object tree we just iterate as long as there is a parent, which has a parent, which has a parent,...
 * @param selectedObject
 */
void ATWatcher::translateFocusToWindow(ATObject* selectedObject)
{
  if (!selectedObject || !selectedObject->parent()) {
    //!< Because we can not focus a "application" - only their windows
    //!< and the window would have the application as parent
    return;
  }
  ATObject *application= (ATObject*) selectedObject->parent();
  ATObject *window=(ATObject*) selectedObject;

  while (application->parent()) {
    window = application;
    application = (ATObject*) application->parent();
  }
  // 	if (applications.contains(selectedObject))
  // 		qDebug() << "Window not in window list";

  focusedWindow = window;
  focusedApplication = application;
}


void ATWatcher::addObject(ATObject *newObject)
{
  // 	qDebug() << "Adding new object: " << newObject;

  // 	applications.append(newObject);
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
bool ATWatcher::trigger(const QString &triggerString)
{
  // 	qDebug() << "execute: " << triggerString;
  QStack<ATObject*> objectsToSearch;

  //TODO: once the "currentWindow" is working, only push the current appwindow
  for (int i=0; i < applications.count(); i++)
    objectsToSearch.push(applications[i]);

  ATObject *current;
  while (!objectsToSearch.isEmpty()) {
    //traverse the object tree and find the item
    current = objectsToSearch.pop();
    if (current->getName() == triggerString) {
      // 			qDebug() << "FOUND IT! It is a " << current->getClassName();
      current->trigger();
      return true;
    }

    QList<ATOMenu*> menuList = current->getMenuList();
    while (menuList.count()>0) {
      ATOMenu *menu = menuList.takeAt(0);
      if (!menu) continue;
      // 			qDebug() << "vor abfrage";
      if (menu->title == triggerString) {
        // 				qDebug() << "in abfrage";
        // 				qDebug() << "FOUND IT! It is a menu!";
        return true;
      }                                           //else
      // 				qDebug() << "war nix"<<menu->title;
      ATOMenu *submenu;
      // 			qDebug() << "subi";
      QList<ATOMenu*> submenus = menu->actions;
      // 			qDebug() << "go";
      foreach (submenu, submenus)
        menuList.append(submenu);
      // 			qDebug() << "nach subi";
    }

    QObjectList children = current->children();
    for (int i=0; i < children.count(); i++)
      objectsToSearch.push((ATObject*) children.at(i));
  }

  return false;
}


ATWatcher::~ATWatcher()
{
  backend->deleteLater();
}
