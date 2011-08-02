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

#include "processinfo.h"
#include "linuxprocessinfogatherer.h"
#include "windowsprocessinfogatherer.h"

ProcessInfo* ProcessInfo::m_instance;

ProcessInfo::ProcessInfo()
{
    #ifdef Q_OS_WIN32
    m_gatherer = new WindowsProcessInfoGatherer();
    #else
    m_gatherer = new LinuxProcessInfoGatherer();
    #endif

    connect(m_gatherer, SIGNAL(processAdded(QString)),
            this, SLOT(checkAddedProcesses(QString)));
    connect(m_gatherer, SIGNAL(processRemoved(QString)),
            this, SLOT(checkRemovedProcesses(QString)));
    connect(m_gatherer, SIGNAL(updateProcesses(QStringList)),
            this, SLOT(checkRunningProcesses(QStringList)));



    m_gatherer->start(QThread::LowPriority);
}

ProcessInfo* ProcessInfo::instance()
{
    if (!m_instance)
        m_instance = new ProcessInfo();

    return m_instance;
}
