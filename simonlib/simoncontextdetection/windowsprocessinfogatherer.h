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

#ifndef WINDOWSPROCESSINFOGATHERER_H
#define WINDOWSPROCESSINFOGATHERER_H

/** \file windowsprocessinfogatherer.h
 * \brief The file containing the WindowsProcessInfoGatherer baseclass header.
 */

#include "processinfogatherer.h"

/**
 *	@class WindowsProcessInfoGatherer
 *	@brief The WindowsProcessInfoGatherer class monitors running processes on Windows platforms
 *
 *      WindowsProcessInfoGatherer is a ProcessInfoGatherer for Windows platforms.
 *
 *      \sa ProcessInfoGatherer, ProcessInfo, ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class WindowsProcessInfoGatherer : public ProcessInfoGatherer
{
    Q_OBJECT
public:
    explicit WindowsProcessInfoGatherer(QObject *parent = 0);

signals:

public slots:
    /** \brief Reimplemented function for gathering active window information on Windows
      *
      */
    void checkActiveWindow();

    /** \brief Reimplemented function for gathering current process list information on Windows
      *
      */
    void checkCurrentProcesses();
};

#endif // WINDOWSPROCESSINFOGATHERER_H
