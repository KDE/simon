#include "conditionassociation.h"
#include "contextmanager.h"
#include <KDebug>

ConditionAssociation::ConditionAssociation(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
}

void ConditionAssociation::privateDeSerialize(QDomElement elem)
{
    QDomElement conditionElem;
    Condition* condition;
    ContextManager* manager;
    
    //get manager instance
    manager = ContextManager::instance();

    conditionElem = elem.firstChildElement("Condition");
    while(!conditionElem.isNull())
    {
	kDebug() << "GETTING ASSOCIATION CONDITIONS!";
	condition = manager->getCondition(conditionElem);
        
	if (condition != NULL)
	{
	  m_conditions.push_back(condition);
	}
	conditionElem = conditionElem.nextSiblingElement("Condition");
    }
    
    foreach (condition, m_conditions)
    {
	connect(condition, SIGNAL(conditionChanged()),
		this, SLOT(evaluateConditions()));
    }
    
    this->evaluateConditions();
}