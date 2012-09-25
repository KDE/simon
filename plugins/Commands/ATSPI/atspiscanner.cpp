/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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

#include "atspiscanner.h"
#include <QThread>
#include <QMutexLocker>
#include <KDebug>

ATSPIScanner::ATSPIScanner() : m_abort(false), m_thread(new QThread()), m_registry(0)
{
  moveToThread(m_thread);
  connect(m_thread, SIGNAL(started()), this, SLOT(initialize()));
  m_thread->start();
}

void ATSPIScanner::initialize()
{
  m_registry = new QAccessibleClient::Registry(this);
  m_registry->applications(); // FIXME: KDE bug: 307264

  connect(m_registry, SIGNAL(windowActivated(QAccessibleClient::AccessibleObject)), this, SLOT(windowActivated(QAccessibleClient::AccessibleObject)));
  connect(m_registry, SIGNAL(visibleDataChanged(QAccessibleClient::AccessibleObject)), this, SLOT(visibleDataChanged(QAccessibleClient::AccessibleObject)));
  connect(m_registry, SIGNAL(modelChanged(QAccessibleClient::AccessibleObject)), this, SLOT(modelChanged(QAccessibleClient::AccessibleObject)));

  connect(m_registry, SIGNAL(accessibleNameChanged(QAccessibleClient::AccessibleObject)), this, SLOT(nameChanged(QAccessibleClient::AccessibleObject)));
  connect(m_registry, SIGNAL(accessibleDescriptionChanged(QAccessibleClient::AccessibleObject)), this, SLOT(descriptionChanged(QAccessibleClient::AccessibleObject)));

  m_registry->subscribeEventListeners(QAccessibleClient::Registry::AllEventListeners);
}

ATSPIScanner::~ATSPIScanner()
{
  disconnect(m_registry);
  m_thread->quit();
  m_thread->wait();
  delete m_registry;
  clearATModel();
}

void ATSPIScanner::clearATModel()
{
  m_abort = true;
  m_modelMutex.lock();
  m_actions.clear();
  m_reverseActions.clear();
  m_abort = false;
  m_modelMutex.unlock();
}

QVector<QSharedPointer<QAction> > ATSPIScanner::getActions(const QString& triggerName)
{
  QMutexLocker l(&m_modelMutex);
  return m_actions.value(triggerName).actions();
}

void ATSPIScanner::windowActivated(const QAccessibleClient::AccessibleObject& object)
{
  kDebug() << "Window activated: " << object.name() << object.childCount();
  
  // parse all children of this object
  QStringList alreadyParsed;
  
  QList<QAccessibleClient::AccessibleObject> objectsToParse;
  objectsToParse.append(object.children());
  
  while (!objectsToParse.isEmpty()) {
    const QAccessibleClient::AccessibleObject& o  = objectsToParse.takeFirst();
    //kDebug() << "Object " << o.name() << " is visible: " << o.isVisible();
    //if (alreadyParsed.contains(o.id()) || !o.isVisible()) continue;
    if (alreadyParsed.contains(o.id())) continue;
    //kDebug() << "Parsing: " << o.name();
    
    if (!o.actions().isEmpty()) {
      kDebug() << "========== Triggerable: " << o.name();

      m_modelMutex.lock();
      if (m_abort) {
        m_modelMutex.unlock();
	return;
      }
      m_actions.insertMulti(o.name(), o);
      m_reverseActions.insert(o, o.name());
      m_modelMutex.unlock();
    }

    alreadyParsed << o.id();
    //add children to the list to parse
    objectsToParse.append(o.children());
  }
  
  QMutexLocker l(&m_modelMutex);
  kDebug() << "Emitting commands shown";
  emit commandsShown(m_actions.keys());
}

void ATSPIScanner::modelChanged(const QAccessibleClient::AccessibleObject& object)
{
  kDebug() << "Model changed";
}

void ATSPIScanner::visibleDataChanged(const QAccessibleClient::AccessibleObject& object)
{
  kDebug() << "Visible data changed";
}

void ATSPIScanner::descriptionChanged(const QAccessibleClient::AccessibleObject& object)
{
  kDebug() << "description changed";
}

void ATSPIScanner::nameChanged(const QAccessibleClient::AccessibleObject& object)
{
  m_modelMutex.lock();
  if (!m_reverseActions.contains(object)) {
    m_modelMutex.unlock();
    return;
  }

  kDebug() << "Name changed: " << object.name();

  QStringList toAdd;
  QStringList toRemove;

  QString oldName = m_reverseActions.value(object);
  m_reverseActions.insert(object, object.name());

  if (!m_actions.contains(object.name()))
    toAdd << object.name();

  QHash<QString,QAccessibleClient::AccessibleObject>::iterator i = m_actions.find(oldName);
  while (i != m_actions.end()) {
    if (*i == object) {
      m_actions.erase(i);
      i = m_actions.find(oldName);
    }
  }
  kDebug() << oldName << m_actions.keys();
  if (!m_actions.keys().contains(oldName))
    toRemove << oldName;
  m_modelMutex.unlock();

  emit commandsModified(toRemove, toAdd);
}

