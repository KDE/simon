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


#include "linuxprocessinfogathererhelper.h"
#include <KWindowSystem>
#include <NETWinInfo>
#include <QX11Info>
#include <QFile>
#include <QFileInfo>
#include <KDebug>

LinuxProcessInfoGathererHelper::LinuxProcessInfoGathererHelper(QObject *parent) :
    QObject(parent)
{
}

void LinuxProcessInfoGathererHelper::checkActiveWindow()
{
    //Get the active window info
    WId wid = KWindowSystem::activeWindow();

    NETWinInfo netInfo(QX11Info::display(),
                       wid,
                       QX11Info::appRootWindow(),
                       NET::WMPid | NET::WMName);

    //Get tne name of the process controlling the active window with the pid from netInfo
    QString processName;
    QString windowName;
    QString strPID;

    processName = getProcessName(netInfo.pid());
    if (!processName.isNull()) {
        windowName = netInfo.name();

        //emit any changes to the active window name or its process
        if (m_currentActiveWindowTitle != windowName)
        {
            m_currentActiveWindowTitle = windowName;
            emit activeWindowTitleChanged(windowName);
        }
        if (m_currentActiveWindowProgram != processName)
        {
            m_currentActiveWindowProgram = processName;
            emit activeWindowProcessChanged(processName);
        }
    }
}

QString LinuxProcessInfoGathererHelper::getProcessName(int id)
{
    QFile procFile;
    QString stat;
    QString command;
    QString executable;

    procFile.setFileName("/proc/" + QString::number(id) + "/cmdline");
    if (!procFile.exists())
        return QString();

    procFile.open(QFile::ReadOnly);
    stat = QString(procFile.readAll());

    command = stat.left(stat.indexOf(QChar('\0')));

    QFileInfo commandInfo(command);
    if (commandInfo.exists())
      return commandInfo.fileName();

    return command;
}
