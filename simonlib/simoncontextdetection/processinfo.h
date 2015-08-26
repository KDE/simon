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
#include <QDebug>

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
    /** \brief Gets the singleton instance of the ProcessInfo class
      *
      */
    static ProcessInfo* instance();

    /** \brief Destructor
      *
      */
    ~ProcessInfo();

    /** \return The list of running processes as most recently determined by the ProcessInfoGatherer
      *
      */
    QStringList getRunningProcesses() {return m_runningProcesses;}

private:
    /** \brief Private Constructor
      *
      */
    explicit ProcessInfo();
    //ProcessInfo(const ProcessInfo&) {}
    //ProcessInfo& operator=(const ProcessInfo&) {}

    /** \brief The list of running processes as most recently determined by the ProcessInfoGatherer
      *
      */
    QStringList m_runningProcesses;

    /** \brief The ProcessInfoGatherer of this ProcessInfo instance
      *
      */
    ProcessInfoGatherer *m_gatherer;

    /** \brief The ProcessInfo instance
      *
      */
    static ProcessInfo *m_instance;

signals:
    /** \brief Emits a process name whenever that process is found to be newly running
      *
      */
    void processAdded(QString);

    /** \brief Emits a process name whenever that process is found to be no longer running (after having been running)
      *
      */
    void processRemoved(QString);

    /** \brief Emits the active window title whenever the title of the active window changes
      *
      */
    void activeWindowTitleChanged(QString);

    /** \brief Emits the process controlling the active window whenever its controlling process changes
      *
      */
    void activeWindowProcessChanged(QString);

private slots:
    /** \brief Sets the list of currently running processes
      *
      */
    void checkRunningProcesses(QStringList names) { m_runningProcesses = names; }

    /** \brief Relays newly added process information from the ProcessInfoGatherer
      *
      */
    void checkAddedProcesses(QString name) { emit processAdded(name); }

    /** \brief Relays newly removed process information from the ProcessInfoGatherer
      *
      */
    void checkRemovedProcesses(QString name) { emit processRemoved(name); }

    /** \brief Relays the active window title from the ProcessInfoGatherer
      *
      */
    void checkActiveWindowTitle(QString title) {
        emit activeWindowTitleChanged(title);
    }

    /** \brief Relays the active-window-controlling process name from the ProcessInfoGatherer
      *
      */
    void checkActiveWindowProcess(QString process) {
        emit activeWindowProcessChanged(process);
    }
};

#endif // PROCESSINFO_H
