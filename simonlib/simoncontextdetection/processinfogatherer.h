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
    explicit ProcessInfoGatherer(QObject *parent = 0);

protected:
    void run();
    QStringList m_previouslyRunningProcesses;
    QStringList m_currentlyRunningProcesses;
    virtual void checkCurrentProcesses()=0;
    void checkProcessListChanges();

signals:
    void processAdded(QString);
    void processRemoved(QString);
    void updateProcesses(QStringList);

public slots:


};

#endif // PROCESSINFOGATHERER_H
