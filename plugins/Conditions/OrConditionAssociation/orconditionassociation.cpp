#include "orconditionassociation.h"
#include <KDebug>

K_PLUGIN_FACTORY( OrConditionAssociationPluginFactory,
registerPlugin< OrConditionAssociation >();
)

K_EXPORT_PLUGIN( OrConditionAssociationPluginFactory("simonorconditionassociation") )

OrConditionAssociation::OrConditionAssociation(QObject *parent, const QVariantList &args) :
    ConditionAssociation(parent, args)
{
}

void OrConditionAssociation::evaluateConditions()
{
   Condition* condition;
  
   if (m_satisfied)
   {
      foreach(condition, m_conditions)
      {
	if (condition->isSatisfied())
	{
	  return;
	}
      }
      
      m_satisfied = false;
      kDebug() << "Or condition is false!";
      emit conditionChanged();
   }
   else
   {
     foreach(condition, m_conditions)
     {
       if (condition->isSatisfied())
       {
	  m_satisfied = true;
	  kDebug() << "Or condition is true!";
	  emit conditionChanged();
       }
     }
   }
}