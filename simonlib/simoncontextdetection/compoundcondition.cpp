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

#include "compoundcondition.h"
#include "contextmanager.h"

#include <KDebug>

CompoundCondition::CompoundCondition(QObject *parent) :
    QObject(parent)
{
    m_proxy = new CompoundConditionModel(this);
    m_satisfied = true;
}

CompoundCondition* CompoundCondition::createInstance(const QDomElement &elem)
{
    //create the instance
    CompoundCondition* instance = new CompoundCondition();

    //deserialize the compound condition dom element
    if (!instance->deSerialize(elem))
    {
        kDebug() << "Error: failed to create compound condition!";
        return 0;
    }

    //initial evaluation of the condition
    instance->evaluateConditions();

    return instance;
}

QDomElement CompoundCondition::createEmpty(QDomDocument *doc)
{
    return doc->createElement("compoundcondition");
}

bool CompoundCondition::addCondition(Condition *condition)
{
    if (!condition)
        return false;

    m_conditions.push_back(condition);
    connect(condition, SIGNAL(conditionChanged()),
            this, SLOT(evaluateConditions()));

    this->evaluateConditions();

    m_proxy->update();

    emit modified();

    return true;
}

bool CompoundCondition::removeCondition(Condition *condition)
{
    if (!condition)
        return false;

    if (!m_conditions.removeOne(condition))
    {
        kDebug() << "Condition is not part of the compound condition!";
        return false;
    }

    disconnect(condition, SIGNAL(conditionChanged()),
               this, SLOT(evaluateConditions()));

    this->evaluateConditions();

    m_proxy->update();

    emit modified();

    return true;
}

bool CompoundCondition::deSerialize(const QDomElement &elem)
{
    QDomElement conditionElem;
    Condition* condition;
    ContextManager* manager;

    manager = ContextManager::instance();

    conditionElem = elem.firstChildElement("condition");
    while(!conditionElem.isNull())
    {
        //get condition from the condition dom element
        condition = manager->getCondition(conditionElem);

        //error checking
        if (condition == NULL)
        {
            kDebug() << "Error: Invalid Condition within CompoundCondition!";
            return false;
        }

        //connect condition to the evaluateConditions slot
        connect(condition, SIGNAL(conditionChanged()),
                this, SLOT(evaluateConditions()));

        //add the condition to the conditions list
        m_conditions.push_back(condition);

        //get next condition element
        conditionElem = conditionElem.nextSiblingElement("condition");
    }

    m_proxy->update();

    return true;
}

QDomElement CompoundCondition::serialize(QDomDocument *doc)
{
    QDomElement elem = doc->createElement("compoundcondition");

    foreach (Condition* condition, m_conditions)
    {
        elem.appendChild(condition->serialize(doc));
    }

    m_proxy->update();

    return elem;
}

void CompoundCondition::evaluateConditions()
{
    Condition *condition;

    if (m_satisfied == true)
    {
        // krazy:exclude=foreach
        foreach (condition, m_conditions)
        {
            if (!condition->isSatisfied())
            {
                m_satisfied = false;
                //m_proxy->update();
                kDebug() << "CompoundCondition is not satisfied!";
                emit conditionChanged(m_satisfied);

                return;
            }
        }
        kDebug() << "CompoundCondition is still satisfied!";
    }
    else
    {
        // krazy:exclude=foreach
        foreach (condition, m_conditions)
        {
            if (!condition->isSatisfied())
            {
                kDebug() << "CompoundCondition is still not satisfied!";
                return;
            }
        }

        m_satisfied = true;
        //m_proxy->update();
        kDebug() << "CompoundCondition is satisfied!";
        emit conditionChanged(m_satisfied);
    }

    m_proxy->update();
}
