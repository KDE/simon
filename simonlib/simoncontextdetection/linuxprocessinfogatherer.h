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

#ifndef LINUXPROCESSINFOGATHERER_H
#define LINUXPROCESSINFOGATHERER_H

/** \file linuxprocessinfogatherer.h
 * \brief The file containing the LinuxProcessInfoGatherer baseclass header.
 */

#include "processinfogatherer.h"
#include "linuxprocessinfogathererhelper.h"

/**
 *	@class LinuxProcessInfoGatherer
 *	@brief The LinuxProcessInfoGatherer class monitors running processes on Linux-like platforms
 *
 *      LinuxProcessInfoGatherer is a ProcessInfoGatherer for Linux platforms.
 *
 *      \sa ProcessInfoGatherer, ProcessInfo, ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class LinuxProcessInfoGatherer : public ProcessInfoGatherer
{
    Q_OBJECT
public:
    explicit LinuxProcessInfoGatherer(QObject *parent = 0);
    ~LinuxProcessInfoGatherer();

private:
    /** \brief Helper object which resides in the main GUI thread
      *
      * The LinuxProcessInfoGatherer runs on a separate thread which means that it cannot access certian x11 functions that it needs
      * in order to gather active window information.  The helper object resides in the main thread and relays such information
      * to the LinuxProcessInfoGatherer.
      */
    LinuxProcessInfoGathererHelper* m_helper;

protected:
    /** \brief Reimplemented checkCurrentProcesses() function that checks the current processes on a Linux-like system
      *
      */
    void checkCurrentProcesses();
    /** \brief Reimplemented checkActiveWindow() function that checks the process and name of the active window on a Linux-like system
      *
      * This function makes use of a LinuxProcessInfoGathererHelper in order to get information that must be gathered from the main GUI thread
      *
      */
    void checkActiveWindow();

signals:
    /// Triggers the LinuxProcessInfoGathererHelper to gather active window data
    void triggerHelper();

public slots:

};

#endif // LINUXPROCESSINFOGATHERER_H
