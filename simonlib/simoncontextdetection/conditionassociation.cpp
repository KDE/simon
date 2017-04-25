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

#include "conditionassociation.h"
#include "contextmanager.h"
#include <QDebug>

ConditionAssociation::ConditionAssociation(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
}

ConditionAssociation::~ConditionAssociation()
{
    foreach(Condition* condition, m_conditions)
        ContextManager::instance()->releaseCondition(condition);
}

QDomElement ConditionAssociation::privateSerialize(QDomDocument *doc, QDomElement elem)
{
    foreach(Condition* condition, m_conditions)
    {
        elem.appendChild(condition->serialize(doc));
    }

    return elem;
}

bool ConditionAssociation::privateDeSerialize(QDomElement elem)
{
    QDomElement conditionElem;
    Condition* condition;
    ContextManager* manager;
    
    //get manager instance
    manager = ContextManager::instance();

    conditionElem = elem.firstChildElement("condition");
    while(!conditionElem.isNull())
    {
	//get a condition from the dom element
	condition = manager->getCondition(conditionElem);
	if (condition == NULL)
        {
            qDebug() << "Context manager returned NULL Condition!  Deserialization failure!";
            return 0;
	}
	
	//add the condition to the list and connect it to the appropriate slot
	m_conditions.push_back(condition);
	connect(condition, SIGNAL(conditionChanged()),
		this, SLOT(evaluateConditions()));
	
	//get the next condition dom element
        conditionElem = conditionElem.nextSiblingElement("condition");
    }
    
    this->evaluateConditions();
    
    return 1;
}
