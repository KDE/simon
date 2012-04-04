/*
 *   Copyright (C) 2012 Adam Nash <adam.t.nash@gmail.com>
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


#include "samplegroupcondition.h"
#include "contextmanager.h"
#include <KStandardDirs>
#include <KDebug>
#include <QTextStream>
#include <QFile>

SampleGroupCondition::SampleGroupCondition(QObject *parent) :
    QObject(parent)
{
    m_currentSampleGroup = "default";
}

QString SampleGroupCondition::getSampleGroup(int index)
{
    if (index < getSampleGroupConditionCount() && index >= 0)
        return m_sampleGroups.at(index);
    else
        return QString();
}

QStringList SampleGroupCondition::getSampleGroups()
{
    QStringList sampleGroups;

    sampleGroups << m_sampleGroups;

    sampleGroups.removeDuplicates();

    return sampleGroups;
}

Condition* SampleGroupCondition::getSampleGroupCondition(int index)
{
    if (index < getSampleGroupConditionCount() && index >= 0)
        return m_sampleGroupConditions.at(index);
    else
        return 0;
}

int SampleGroupCondition::getSampleGroupConditionCount()
{
    if (m_sampleGroupConditions.count() != m_sampleGroups.count())
    {
        kDebug() << "Sample group condition list is a different size from the sample group list!!";
        return 0;
    }

    return m_sampleGroupConditions.count();
}

void SampleGroupCondition::addSampleGroupCondition(Condition* condition, QString sampleGroup, int index)
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

    checkAcousticContext();
}

bool SampleGroupCondition::removeSampleGroupCondition(int index)
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

    checkAcousticContext();
}

bool SampleGroupCondition::changeSampleGroup(int index, QString sampleGroup)
{
    if (index < getSampleGroupConditionCount())
    {
        m_sampleGroups[index] = sampleGroup;
        return true;
    }
    else
        return false;

    checkAcousticContext();
}

bool SampleGroupCondition::promoteCondition(int index)
{
    if (index < 1 || index >= getSampleGroupConditionCount())
        return false;
    else
    {
        m_sampleGroupConditions.move(index, index-1);
        m_sampleGroups.move(index, index-1);
        return true;
    }

    checkAcousticContext();
}

bool SampleGroupCondition::demoteCondition(int index)
{
    if (index < 0 || index >= getSampleGroupConditionCount()-1)
        return false;
    else
    {
        m_sampleGroupConditions.move(index, index+1);
        m_sampleGroups.move(index, index+1);
        return true;
    }

    checkAcousticContext();
}

void SampleGroupCondition::checkAcousticContext()
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

void SampleGroupCondition::saveSampleGroupContext()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("samplegroupcontextroot");

    kDebug() << "Saving sample group context";

    for (int i=0 ; i<this->getSampleGroupConditionCount(); i++)
    {
        QDomElement sampleGroupContext = doc.createElement("samplegroupcontext");
        sampleGroupContext.appendChild(getSampleGroupCondition(i)->serialize(&doc));

        QDomElement sampleGroup = doc.createElement("samplegroup");
        sampleGroup.setAttribute("name", getSampleGroup(i));
        sampleGroupContext.appendChild(sampleGroup);

        root.appendChild(sampleGroupContext);
    }

    QString contextDir = KStandardDirs::locateLocal("appdata", "context/");
    QFile sampleGroupContextFile(contextDir + "sampleGroupContext");

    sampleGroupContextFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&sampleGroupContextFile);
    root.save(stream, 2);
    sampleGroupContextFile.close();
}

void SampleGroupCondition::loadSampleGroupContext()
{
    kDebug() << "loading sample group context";
    QString contextDir = KStandardDirs::locateLocal("appdata", "context/");
    QFile sampleGroupContextFile(contextDir + "sampleGroupContext");

    if (!sampleGroupContextFile.exists())
        return;

    sampleGroupContextFile.open(QIODevice::ReadOnly);
    QDomDocument doc;
    doc.setContent(&sampleGroupContextFile);
    sampleGroupContextFile.close();

    QDomElement root = doc.documentElement();

    QDomElement sampleGroupContext = root.firstChildElement("samplegroupcontext");
    while (!sampleGroupContext.isNull())
    {
        QDomElement conditionElement = sampleGroupContext.firstChildElement("condition");
        Condition* condition = ContextManager::instance()->getCondition(conditionElement);

        if (condition)
        {
            QDomElement sampleGroup = sampleGroupContext.firstChildElement("samplegroup");

            addSampleGroupCondition(condition, sampleGroup.attribute("name", "default"));
        }

        sampleGroupContext = sampleGroupContext.nextSiblingElement("samplegroupcontext");
    }
    kDebug() << "finished loading sample group context";
}
