/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "contextmanager.h"
#include "processinfo.h"
#include <KService>
#include <KServiceTypeTrader>
#include <KStandardDirs>
#include <KDebug>
#include <QTextStream>

ContextManager* ContextManager::m_instance;

ContextManager::ContextManager(QObject *parent) :
    QObject(parent)
{
    kDebug() << "creating context manager";
    m_sampleGroupCondition = new SampleGroupCondition();
    connect(m_sampleGroupCondition, SIGNAL(sampleGroupChanged(QStringList)),
            this, SIGNAL(sampleGroupChanged(QStringList)));
}

ContextManager::~ContextManager()
{
    ProcessInfo::instance()->deleteLater();
    qDeleteAll(m_conditionLookup);
}

ContextManager* ContextManager::instance()
{
    if (!m_instance)
        m_instance = new ContextManager();

    return m_instance;
}

QStringList ContextManager::getDeactivatedSampleGroups()
{
    return m_sampleGroupCondition->getDeactivatedSampleGroups();
}

QList<Condition*> ContextManager::getConditions()
{
    QList<Condition*> conditions;
    KService::List services;
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    Condition *condition;

    services = trader->query("simon/ConditionPlugin");
    foreach (KService::Ptr service, services)
    {
        condition = getEmptyCondition(service->storageId());

        if (condition)
            conditions.push_back(condition);
    }

    return conditions;
}

Condition* ContextManager::getEmptyCondition(const QString &pluginName)
{
    Condition* condition;

    //get the service
    KService::Ptr service = KService::serviceByStorageId(pluginName);
    if (!service) {
      kDebug() << "Service not found! Source: " << pluginName;
      return 0;
    }

    //create the factory for the service
    KPluginFactory *factory = KPluginLoader(service->library()).factory();
    if (factory) {
      condition = factory->create<Condition>();
      factory->deleteLater();
    }
    else {
      kDebug() << "Factory not found! Source: " << pluginName;
      return 0;
    }

    return condition;
}

Condition* ContextManager::getCondition(const QDomElement &elem)
{
    Condition* condition;
    QString source;
    QString str;
    QTextStream stream(&str);
    
    //check to see if the condition has already been created
    //if so, just return the existing condition
    elem.save(stream, 4);
    kDebug() << "Condition: " + str;
    condition = m_conditionLookup.value(str, 0);
    if (condition != 0)
    {
	kDebug() << "Condition is a duplicate!";
        // increment reference counter
        incrementRefCount(condition);
	return condition;
    }

    //get the name of the service
    source = elem.attribute("name");

    //get the service
    KService::Ptr service = KService::serviceByStorageId(source);
    if (!service) {
      kDebug() << "Service not found! Source: " << source;
      return 0;
    }

    //create the factory for the service
    KPluginFactory *factory = KPluginLoader(service->library()).factory();
    if (factory) {
      condition = factory->create<Condition>();
      factory->deleteLater();
    }
    else {
      kDebug() << "Factory not found! Source: " << source;
      return 0;
    }

    //deserialize the service data
    condition->deSerialize(elem);
    
    incrementRefCount(condition);
    //add the condition to member containers for future lookup
    m_conditionLookup.insert(str, condition);
    
    return condition;
}

void ContextManager::releaseCondition ( Condition* c )
{
  decrementRefCount(c);
}

void ContextManager::incrementRefCount ( Condition* c )
{
  int refCount = m_conditionReferenceCounter.value(c, 0);
  ++refCount;
  kDebug() << "New ref count: " << refCount;
  m_conditionReferenceCounter.insert(c, refCount); // will replace old value
}

void ContextManager::decrementRefCount ( Condition* c )
{
  int refCount = m_conditionReferenceCounter.value(c, 0);
  --refCount;
  kDebug() << "New ref count: " << refCount;
  if (refCount == -1) return; // not in there
  if (refCount == 0) {
    m_conditionLookup.remove(m_conditionLookup.key(c)); // keys are unique so this will delete the only entry
    m_conditionReferenceCounter.remove(c);
    delete c;
    return;
  }
  m_conditionReferenceCounter.insert(c, refCount); // will replace old value
}
