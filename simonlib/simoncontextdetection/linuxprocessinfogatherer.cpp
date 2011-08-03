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

#include "linuxprocessinfogatherer.h"
#include <QRegExp>
#include <QDir>
#include <QApplication>

LinuxProcessInfoGatherer::LinuxProcessInfoGatherer(QObject *parent) :
    ProcessInfoGatherer(parent)
{
    m_helper = new LinuxProcessInfoGathererHelper();

    connect(this, SIGNAL(triggerHelper()),
            m_helper, SLOT(checkActiveWindow()));
    connect(m_helper, SIGNAL(activeWindowProcessChanged(QString)),
            this, SIGNAL(activeWindowProcessChanged(QString)));
    connect(m_helper, SIGNAL(activeWindowTitleChanged(QString)),
            this, SIGNAL(activeWindowTitleChanged(QString)));

    m_helper->moveToThread(qApp->thread());
}

void LinuxProcessInfoGatherer::checkCurrentProcesses()
{
    QRegExp digits;
    QDir procDir;
    QFileInfoList files;
    QFile commFile;
    QString processName;

    //process directories will be a series of digits
    digits = QRegExp("[0-9]+");

    //get a list of all the directories in the /proc directory
    procDir.setPath("/proc");
    files = procDir.entryInfoList(QStringList("*"), QDir::AllDirs, QDir::Unsorted);

    //find the process directories and get the names from the comm files
    foreach (QFileInfo info, files)
    {
        if (digits.exactMatch(info.baseName()))
        {
            commFile.setFileName("/proc/" + info.baseName() + "/comm");

            if (commFile.exists())
            {
                commFile.open(QFile::ReadOnly);
                processName = QString(commFile.readAll());
                commFile.close();

                //get rid of the newline character
                processName.chop(1);

                //add the process name to the list of current process names
                m_currentlyRunningProcesses.push_back(processName);
            }
        }
    }
}

LinuxProcessInfoGatherer::~LinuxProcessInfoGatherer()
{
    m_helper->deleteLater();
}

void LinuxProcessInfoGatherer::checkActiveWindow()
{
    emit triggerHelper();
}
