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

#ifndef PROCESSINFO_H
#define PROCESSINFO_H

/** \file processinfo.h
 * \brief The file containing the ProcessInfo baseclass header.
 */

#include <QThread>
#include "processinfogatherer.h"
#include "simoncontextdetection_export.h"

/**
 *	@class ProcessInfo
 *	@brief The ProcessInfo class broadcasts changes in, and provides a list of the running processes in the system
 *
 *      The ProcessInfo class uses a ProcessInfoGatherer to monitor the running processes on the system.
 *      When a process is added or removed, a signal is sent to broadcast this change.
 *
 *      ProcessInfo is a singleton.
 *
 *      \sa ProcessInfoGatherer, ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTDETECTION_EXPORT ProcessInfo : public QObject
{
    Q_OBJECT
public:
    static ProcessInfo* instance();

    QStringList getRunningProcesses() {return m_runningProcesses;}

private:
    explicit ProcessInfo();
    //ProcessInfo(const ProcessInfo&) {}
    //ProcessInfo& operator=(const ProcessInfo&) {}

    QStringList m_runningProcesses;
    ProcessInfoGatherer *m_gatherer;
    static ProcessInfo *m_instance;

signals:
    void processAdded(QString);
    void processRemoved(QString);

public slots:
    void checkRunningProcesses(QStringList names) {m_runningProcesses = names;}
    void checkAddedProcesses(QString name) {emit processAdded(name);}
    void checkRemovedProcesses(QString name) {emit processRemoved(name);}
};

#endif // PROCESSINFO_H
