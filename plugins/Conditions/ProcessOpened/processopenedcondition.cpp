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

#include "processopenedcondition.h"
#include <KDE/KDebug>
#include "createprocessopenedconditionwidget.h"
#include <QWidget>

K_PLUGIN_FACTORY( ProcessOpenedPluginFactory,
registerPlugin< ProcessOpenedCondition >();
)

K_EXPORT_PLUGIN( ProcessOpenedPluginFactory("simonprocessopenedcondition") )

ProcessOpenedCondition::ProcessOpenedCondition(QObject *parent, const QVariantList &args) :
    Condition(parent, args)
{
    m_processName = "";
    m_pluginName = "simonprocessopenedconditionplugin.desktop";
}

CreateConditionWidget* ProcessOpenedCondition::getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget* parent)
{
    return new CreateProcessOpenedConditionWidget(compoundCondition, parent);
}

QDomElement ProcessOpenedCondition::privateSerialize(QDomDocument *doc, QDomElement elem)
{
    QDomElement nameElem = doc->createElement("processname");
    nameElem.appendChild(doc->createTextNode(m_processName));

    elem.appendChild(nameElem);

    return elem;
}

QString ProcessOpenedCondition::name()
{
    if (!isInverted())
      return i18nc("The given process needs to be running to satisfy the condition", "'%1' is opened", m_processName);
    else
      return i18nc("The given process must not be running to satisfy the condition", "'%1' is not opened", m_processName);
}

bool ProcessOpenedCondition::privateDeSerialize(QDomElement elem)
{
    QDomElement nameElement;

    //get the process name
    nameElement = elem.firstChildElement("processname");
    if (nameElement.isNull())
    {
        kDebug() << "No processes name specified!  Deserialization failure!";
        return false;
    }
    m_processName = nameElement.text();

    //connect to the ProcessInfo instance
    ProcessInfo* processInfo = ProcessInfo::instance();

    connect(processInfo, SIGNAL(processAdded(QString)),
            this, SLOT(checkAddedProcess(QString)));
    connect(processInfo, SIGNAL(processRemoved(QString)),
            this, SLOT(checkRemovedProcess(QString)));

    //initialize the condition
    m_openedInstances = processInfo->getRunningProcesses().count(m_processName);

    if (m_openedInstances > 0)
    {
        m_satisfied = true;
        kDebug() << name() + " is true!";
        emit conditionChanged();
    }
    else
    {
        m_satisfied = false;
        kDebug() << name() + " is false!";
        emit conditionChanged();
    }

    return true;
}

void ProcessOpenedCondition::checkAddedProcess(QString processName)
{
    if (m_processName == processName)
    {
        m_openedInstances++;

        if (m_openedInstances == 1)
        {
            m_satisfied = true;
            kDebug() << name() + " is true!";
            emit conditionChanged();
        }
    }
}

void ProcessOpenedCondition::checkRemovedProcess(QString processName)
{
    if (m_processName == processName)
    {
        m_openedInstances--;

        if (m_openedInstances == 0)
        {
            m_satisfied = false;
            kDebug() << name() + " is false!";
            emit conditionChanged();
        }
    }
}
