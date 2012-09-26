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

ATSPIScanner::ATSPIScanner() : m_abort(false), 
  m_thread(new QThread()), m_registry(0), m_cleanStringRegExp(QRegExp("[^\\w ]"))
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
  connect(m_registry, SIGNAL(stateChanged(QAccessibleClient::AccessibleObject, QString, bool)), this, SLOT(stateChanged(QAccessibleClient::AccessibleObject, QString, bool)));

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
  kDebug() << "Clearing tracked objects";
  m_reverseActions.clear();
  m_abort = false;
  m_modelMutex.unlock();
}

QVector<QSharedPointer<QAction> > ATSPIScanner::getActions(const QString& triggerName)
{
  QMutexLocker l(&m_modelMutex);

  QAccessibleClient::AccessibleObject object = m_actions.value(triggerName);
  if (!object.isValid())
    return QVector<QSharedPointer<QAction> >();
  return object.actions();
}

void ATSPIScanner::processTree(const QAccessibleClient::AccessibleObject &object, bool added, bool reset)
{
  QStringList alreadyParsed;
  QList<QAccessibleClient::AccessibleObject> objectsToParse;
  objectsToParse.append(object);

  // in case this is called from a recently hidden object, this objects visibility will be false
  // but we still want to remove all visible children from the AT model. So force re-evaluation
  // of the objects children
  objectsToParse.append(object.children());
  
  while (!objectsToParse.isEmpty()) {
    const QAccessibleClient::AccessibleObject& o  = objectsToParse.takeFirst();
    kDebug() << "Object " << o.name() << " is visible: " << o.isVisible() << " id: " << o.id();

    if (alreadyParsed.contains(o.id()))
      continue;

    QString cleanName = cleanString(o.name());
    if (!added && !o.isVisible()) {
      m_modelMutex.lock();
      removeAction(cleanName, o);
      m_reverseActions.remove(o);
      m_modelMutex.unlock();
    }

    if (!o.isVisible()) {
      alreadyParsed << o.id();
      continue;
    }
    
    if (!cleanName.isEmpty()) {
      if (!o.actions().isEmpty()) {
        kDebug() << "========== Triggerable: " << cleanName;

        m_modelMutex.lock();
        if (m_abort) {
          m_modelMutex.unlock();
          return;
        }
        if (added) {
          if (!m_reverseActions.contains(o)) {
	    kDebug() << "Tracking: " << cleanName << o.id();
            m_actions.insertMulti(cleanName, o);
            m_reverseActions.insert(o, cleanName);
          }
        } else {
          removeAction(cleanName, o);
          m_reverseActions.remove(o);
        }
        m_modelMutex.unlock();
      }
    }
    alreadyParsed << o.id();
    //add children to the list to parse
    objectsToParse.append(o.children());
  }

  QMutexLocker l(&m_modelMutex);
  kDebug() << "Emitting commands shown" << m_actions.keys();
  emit commandsShown(m_actions.keys(), reset);
}

void ATSPIScanner::windowActivated(const QAccessibleClient::AccessibleObject& object)
{
  clearATModel();
  kDebug() << "Window activated: " << object.name() << object.childCount();
  
  // parse all children of this object
  processTree(object, true /* added */, true);
}

void ATSPIScanner::stateChanged (const QAccessibleClient::AccessibleObject &object, const QString& state, bool active)
{
  kDebug() << "State changed: " << object.id() << state << active;
  if (state != "showing")
    return;

  if (!active) {
    m_modelMutex.lock();
    if (!m_reverseActions.contains(object)) {
      kDebug() << "Untracked object changed: " << object.name() << object.id();
      m_modelMutex.unlock();
      return;
    }
    m_modelMutex.unlock();
  }

  processTree(object, active, false);
}

void ATSPIScanner::descriptionChanged(const QAccessibleClient::AccessibleObject& object)
{
  Q_UNUSED(object);
  kDebug() << "description changed";
}

void ATSPIScanner::removeAction(const QString& name, const QAccessibleClient::AccessibleObject& o)
{
  kDebug() << "Removing action " << name << o.id();
  QHash<QString,QAccessibleClient::AccessibleObject>::iterator i = m_actions.find(name);
  while (i != m_actions.end()) {
    if (*i == o) {
      m_actions.erase(i);
      i = m_actions.find(name);
    } else
      ++i;
  }
}

void ATSPIScanner::nameChanged(const QAccessibleClient::AccessibleObject& object)
{
  QMutexLocker l(&m_modelMutex);
  if (!m_reverseActions.contains(object)) {
    kDebug() << "Untracked object changed its name: " << object.name() << object.id();
    return;
  }

  QString cleanName = cleanString(object.name());
  kDebug() << "Name changed: " << cleanName;

  QStringList toAdd;
  QStringList toRemove;

  QString oldName = m_reverseActions.value(object);
  m_reverseActions.insert(object, cleanName); // replace

  if (!m_actions.contains(cleanName))
    toAdd << cleanName;

  removeAction(oldName, object);
  m_actions.insert(cleanName, object);
  kDebug() << oldName << m_actions.keys();
  if (!m_actions.keys().contains(oldName))
    toRemove << oldName;

  emit commandsShown(m_actions.keys(), false);
}

inline QString ATSPIScanner::cleanString(const QString& input)
{
  QString out = input;
  out.remove(m_cleanStringRegExp);
  out.remove('_');
  return out;
}

