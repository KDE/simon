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

#include <QThread>
#include "processinfogatherer.h"
#include "simoncontextdetection_export.h"

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
