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

#include "orconditionassociation.h"
#include "createorconditionassociationwidget.h"
#include <KDebug>

K_PLUGIN_FACTORY( OrConditionAssociationPluginFactory,
registerPlugin< OrConditionAssociation >();
)

K_EXPORT_PLUGIN( OrConditionAssociationPluginFactory("simonorconditionassociation") )

OrConditionAssociation::OrConditionAssociation(QObject *parent, const QVariantList &args) :
    ConditionAssociation(parent, args)
{
    m_pluginName = "simonorconditionassociationplugin.desktop";
}

CreateConditionWidget* OrConditionAssociation::getCreateConditionWidget(QWidget* parent)
{
    return new CreateOrConditionAssociationWidget(parent);
}

QString OrConditionAssociation::name()
{
    int i;
    QString str;

    for (i=0; i<m_conditions.count(); i++)
    {
        if (i > 0)
        {
            str += " or ";
        }
        str += '(' + m_conditions.at(i)->name() + ')';
    }

    if (isInverted()) 
        return i18nc("%1 is the conditions composited name", "Not: %1", str);
    else 
        return str;
}

void OrConditionAssociation::evaluateConditions()
{
   if (m_satisfied)
   {
      foreach(Condition* condition, m_conditions)
      {
	if (condition->isSatisfied())
	{
	  return;
	}
      }
      
      m_satisfied = false;
      kDebug() << name() + " is false!";
      emit conditionChanged();
      return;
   }
   else
   {
     foreach(Condition* condition, m_conditions)
     {
       if (condition->isSatisfied())
       {
	  m_satisfied = true;
          kDebug() << name() + " is true!";
	  emit conditionChanged();
          return;
       }
     }
   }
}
