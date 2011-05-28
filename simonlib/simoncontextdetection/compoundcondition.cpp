#include "compoundcondition.h"

#include <KDebug>

CompoundCondition::CompoundCondition(QList<Condition *> conditions, QObject *parent) :
    QObject(parent)
{
    m_satisfied = false;
    m_conditions = conditions;
  
    foreach(Condition* condition, conditions)
    {
        connect(condition, SIGNAL(conditionChanged()),
                this, SLOT(evaluateConditions()));
    }
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
		
		kDebug() << "Compound Condition Is Not Satisfied!";
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
	kDebug() << "Compound Condition Satisfied!";
    }
}
