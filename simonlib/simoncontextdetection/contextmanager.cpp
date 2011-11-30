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
#include <KDebug>
#include <QTextStream>

ContextManager* ContextManager::m_instance;

//testing
#include <QFile>
//end testing

ContextManager::ContextManager(QObject *parent) :
    QObject(parent)
{
}

ContextManager::~ContextManager()
{
    qDeleteAll(m_conditions);
    ProcessInfo::instance()->deleteLater();
}

ContextManager* ContextManager::instance()
{
    if (!m_instance)
        m_instance = new ContextManager();

    return m_instance;
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
        condition = getCondition(getEmptyCondition(service->storageId()));

        if (condition)
            conditions.push_back(condition);
    }

    return conditions;
}

QDomElement ContextManager::getEmptyCondition(const QString &pluginName)
{
    QDomDocument doc;
    QDomElement emptyElem;
    Condition* condition;

    //get the service
    KService::Ptr service = KService::serviceByStorageId(pluginName);
    if (!service) {
      kDebug() << "Service not found! Source: " << pluginName;
      return QDomElement();
    }

    //create the factory for the service
    KPluginFactory *factory = KPluginLoader(service->library()).factory();
    if (factory) {
      condition = factory->create<Condition>();
      factory->deleteLater();
    }
    else {
      kDebug() << "Factory not found! Source: " << pluginName;
      return QDomElement();
    }

    if (condition)
    {
        //serialize the service data
        emptyElem = condition->serialize(&doc);
        delete condition;

        return emptyElem;
    }

    return QDomElement();
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
    condition = m_conditionLookup.value(str, NULL);
    if (condition != NULL)
    {
	kDebug() << "Condition is a duplicate!";
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
    
    //add the condition to member containers for future lookup
    m_conditions.push_back(condition);
    m_conditionLookup.insert(str, condition);

    return condition;
}

QString ContextManager::getSampleGroup(int index)
{
    if (index < getSampleGroupConditionCount() && index >= 0)
        return m_sampleGroups.at(index);
    else
        return QString();
}

Condition* ContextManager::getSampleGroupCondition(int index)
{
    if (index < getSampleGroupConditionCount() && index >= 0)
        return m_sampleGroupConditions.at(index);
    else
        return 0;
}

int ContextManager::getSampleGroupConditionCount()
{
    if (m_sampleGroupConditions.count() != m_sampleGroups.count())
    {
        kDebug() << "Sample group condition list is a different size from the sample group list!!";
        return 0;
    }

    return m_sampleGroupConditions.count();
}

void ContextManager::addSampleGroupCondition(Condition* condition, QString sampleGroup, int index)
{
    if (!condition)
        return;

    if (index < 0 || index >= getSampleGroupConditionCount())
    {
        m_sampleGroupConditions.push_back(condition);
        m_sampleGroups.push_back(sampleGroup);
    }
    else
    {
        m_sampleGroupConditions.insert(index, condition);
        m_sampleGroups.insert(index, sampleGroup);
    }

    connect(condition, SIGNAL(conditionChanged()),
            this, SLOT(checkAcousticContext()));
}

bool ContextManager::removeSampleGroupCondition(int index)
{
    if (index < getSampleGroupConditionCount() && index >= 0)
    {
        disconnect(m_sampleGroupConditions.at(index), SIGNAL(conditionChanged()),
                this, SLOT(checkAcousticContext()));

        m_sampleGroupConditions.removeAt(index);
        m_sampleGroups.removeAt(index);

        return true;
    }
    else
    {
        kDebug() << "Error: can't remove specified condition!";
        return false;
    }
}

bool ContextManager::changeSampleGroup(int index, QString sampleGroup)
{
    if (index < getSampleGroupConditionCount())
    {
        m_sampleGroups[index] = sampleGroup;
        return true;
    }
    else
        return false;
}

bool ContextManager::promoteCondition(int index)
{
    if (index < 1 || index >= getSampleGroupConditionCount())
        return false;
    else
    {
        m_sampleGroupConditions.move(index, index-1);
        m_sampleGroups.move(index, index-1);
        return true;
    }
}

bool ContextManager::demoteCondition(int index)
{
    if (index < 0 || index >= getSampleGroupConditionCount()-1)
        return false;
    else
    {
        m_sampleGroupConditions.move(index, index+1);
        m_sampleGroups.move(index, index+1);
        return true;
    }
}

void ContextManager::checkAcousticContext()
{
    QString sampleGroup = "default";

    kDebug() << "Evaluating Acoustic Model Context...";

    for (int i=0; i<getSampleGroupConditionCount(); i++)
    {
        if (m_sampleGroupConditions.at(i)->isSatisfied())
        {
            if (!m_sampleGroups.at(i).isEmpty())
            {
                sampleGroup = m_sampleGroups.at(i);
                break;
            }
        }
    }

    if (sampleGroup != m_currentSampleGroup)
    {
        kDebug() << "New Sample Group:" << sampleGroup;

        m_currentSampleGroup = sampleGroup;
        emit sampleGroupChanged(sampleGroup);
    }
}
