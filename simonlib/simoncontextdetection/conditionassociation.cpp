#include "conditionassociation.h"
#include "contextmanager.h"
#include <KDE/KDebug>

ConditionAssociation::ConditionAssociation(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
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

    conditionElem = elem.firstChildElement("Condition");
    while(!conditionElem.isNull())
    {
	//get a condition from the dom element
	condition = manager->getCondition(conditionElem);
	if (condition == NULL)
        {
            kDebug() << "Context manager returned NULL Condition!  Deserialization failure!";
            return 0;
	}
	
	//add the condition to the list and connect it to the appropriate slot
	m_conditions.push_back(condition);
	connect(condition, SIGNAL(conditionChanged()),
		this, SLOT(evaluateConditions()));
	
	//get the next condition dom element
	conditionElem = conditionElem.nextSiblingElement("Condition");
    }
    
    this->evaluateConditions();
    
    return 1;
}
