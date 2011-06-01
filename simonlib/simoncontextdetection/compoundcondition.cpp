#include "compoundcondition.h"
#include "contextmanager.h"

#include <KDebug>

CompoundCondition::CompoundCondition(QObject *parent) :
    QObject(parent)
{
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

bool CompoundCondition::deSerialize(const QDomElement &elem)
{
    QDomElement conditionElem;
    QList<Condition*> conditions;
    Condition* condition;
    ContextManager* manager;

    manager = ContextManager::instance();

    conditionElem = elem.firstChildElement("Condition");
    while(!conditionElem.isNull())
    {
        //get condition from the condition dom element
        condition = manager->getCondition(conditionElem);

        //error checking
        if (condition == NULL)
        {
            kDebug() << "Error: Invalid Condition within CompoundCondition!";
            return 0;
        }

        //connect condition to the evaluateConditions slot
        connect(condition, SIGNAL(conditionChanged()),
                this, SLOT(evaluateConditions()));

        //add the condition to the conditions list
        m_conditions.push_back(condition);

        //get next condition element
        conditionElem = conditionElem.nextSiblingElement("Condition");
    }

    return 1;
}

QDomElement CompoundCondition::serialize(QDomDocument *doc)
{
    QDomElement elem = doc->createElement("CompoundCondition");

    foreach (Condition* condition, m_conditions)
    {
        elem.appendChild(condition->serialize(doc));
    }

    return elem;
}

void CompoundCondition::evaluateConditions()
{
    Condition *condition;

    if (m_satisfied == true)
    {
        foreach (condition, m_conditions)
        {
            if (!condition->isSatisfied())
            {
                m_satisfied = false;
                emit conditionChanged(m_satisfied);
		
                kDebug() << "CompoundCondition is not satisfied!";
            }
        }
    }
    else
    {
        foreach (condition, m_conditions)
        {
            if (!condition->isSatisfied())
            {
                return;
            }
        }

        m_satisfied = true;
        emit conditionChanged(m_satisfied);
        kDebug() << "CompoundCondition is satisfied!";
    }
}
