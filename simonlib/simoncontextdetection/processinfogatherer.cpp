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

#include "processinfogatherer.h"

#include <KDebug>

ProcessInfoGatherer::ProcessInfoGatherer(QObject *parent) :
    QThread(parent)
{
    m_previouslyRunningProcesses = QStringList();
    m_abort = false;
}

ProcessInfoGatherer::~ProcessInfoGatherer()
{
    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();

    wait();
}

void ProcessInfoGatherer::checkProcessListChanges()
{
    QString processName;
    bool dirty = false;

    foreach(processName, m_currentlyRunningProcesses)
    {
        //check to see if the name is new
        if (m_previouslyRunningProcesses.contains(processName))
        {
            m_previouslyRunningProcesses.removeOne(processName);
        }
        else
        {
            dirty = true;
            emit processAdded(processName);
        }
    }

    //signal any processes that were removed
    foreach (processName, m_previouslyRunningProcesses)
    {
        dirty = true;
        emit processRemoved(processName);
    }

    if (dirty)
    {
        emit updateProcesses(m_currentlyRunningProcesses);
        m_previouslyRunningProcesses = m_currentlyRunningProcesses;
    }

    //Refresh the current process list
    m_currentlyRunningProcesses.clear();
}

void ProcessInfoGatherer::run()
{
    forever
    {
        checkCurrentProcesses();
        checkProcessListChanges();

        checkActiveWindow();

        //emit finishedGatheringStep();

        m_mutex.lock();
        if (m_abort)
            return;
        m_mutex.unlock();

        this->sleep(1);
    }
}
