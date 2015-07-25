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

#ifndef PROCESSINFOGATHERER_H
#define PROCESSINFOGATHERER_H

/** \file processinfogatherer.h
 * \brief The file containing the ProcessInfoGatherer baseclass header.
 */

#include <QThread>
#include <QStringList>

/**
 *	@class ProcessInfoGatherer
 *	@brief The ProcessInfoGatherer class provides an abstract interface for monitoring running processes from a low priority thread
 *
 *      The ProcessInfoGatherer checks the running processes on a system in a continuous loop in a low priority thread.
 *      It is expected to emit signals that specify changes in the list of running processes.
 *
 *      A class that uses the ProcessInfoGatherer interface must re-implement the \ref checkCurrentProcesses() function
 *      which checks the running processes places them in the \ref m_currentlyRunningProcesses list.
 *
 *      \sa ProcessInfo, ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class ProcessInfoGatherer : public QThread
{
    Q_OBJECT
public:
    /** \brief Constructor
      *
      */
    explicit ProcessInfoGatherer(QObject *parent = 0);

    /** \brief Destructor
      *
      */
    ~ProcessInfoGatherer();

private:
    /** \brief Boolean value that determines if the ProcessInfoGatherer should terminate its infinite loop
      *
      */
    bool m_abort;

protected:
    /** \brief Reimplemented run() function which contains the main loop of the ProcessInfoGatherer
      *
      */
    void run();

    /** \brief The previously determined list of running processes
      *
      */
    QStringList m_previouslyRunningProcesses;

    /** \brief The most recently determined list of running processes
      *
      */
    QStringList m_currentlyRunningProcesses;

    /** \brief The most recently determined active window title
      *
      */
    QString m_currentActiveWindowTitle;

    /** \brief The most recently determined active-window-controlling process
      *
      */
    QString m_currentActiveWindowProgram;

    /** \brief Checks the current processes
      * It is a pure virtual function that must be reimplemented by any derived ProcessInfoGatherer class.
      *
      */
    virtual void checkCurrentProcesses()=0;

    /** \brief Checks the current active window title and controlling process
      * It is a pure virtual function that must be reimplemented by any derived ProcessInfoGatherer class.
      *
      */
    virtual void checkActiveWindow()=0;

    /** \brief Checks for any discrepancies between the previously determined list of processes and the most recently determined list of running processes
      *
      */
    void checkProcessListChanges();

signals:
    /** \brief Emits the name of any process that has newly started running
      *
      */
    void processAdded(QString);

    /** \brief Emits the name of a process that is newly finished running
      *
      */
    void processRemoved(QString);

    /** \brief Emits the list of currently running processes whenever that list changes
      *
      */
    void updateProcesses(QStringList);

    /** \brief Emits the name of the active window title whenever it changes
      *
      */
    void activeWindowTitleChanged(QString);

    /** \brief Emits the name of the active-window-controlling process whenever it changes
      *
      */
    void activeWindowProcessChanged(QString);

    /** \brief Emits whenever an iteration of the process-info-gathering loop finishes
      *
      */
    void finishedGatheringStep();

public slots:


};

#endif // PROCESSINFOGATHERER_H
