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

protected:
    void checkCurrentProcesses();
    void checkActiveWindow();

signals:

public slots:

};

#endif // LINUXPROCESSINFOGATHERER_H
