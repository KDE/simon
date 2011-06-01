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

#include "windowsprocessinfogatherer.h"

#ifdef Q_OS_WIN32

#include "windows.h"
#include "tchar.h"
#include "psapi.h"
#include <QDebug>

WindowsProcessInfoGatherer::WindowsProcessInfoGatherer(QObject *parent) :
    ProcessInfoGatherer(parent)
{
}

void WindowsProcessInfoGatherer::checkCurrentProcesses()
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    TCHAR szProcessName[MAX_PATH];
    HANDLE hProcess;
    HMODULE hMod;
    QString processName;
    QStringList processNames;
    unsigned int i;

    //Get the list of process identifiers.
    EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded );

    //Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    //Go through all of the processes, getting their handles/names
    for (i=0; i<cProcesses; i++)
    {
        if(aProcesses[i] != 0)
        {
            // Get a handle to the process.
            hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, aProcesses[i] );

            // Get the process name.
            if (NULL != hProcess )
            {
                //if EnumProcessModules fails, nothing is done
                //it most likely fails because of privelages
                //this should be fixed so that all the processes are listed
                if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
                                         &cbNeeded) )
                {

                    GetModuleBaseName( hProcess, hMod, szProcessName,
                                       sizeof(szProcessName)/sizeof(TCHAR) );

                    //Make the process name a QString
                    #ifdef UNICODE
                    processName = QString::fromUtf16((ushort*)szProcessName);
                    #else
                    processName = QString::fromLocal8Bit(szProcessName);
                    #endif

                    //check to see if the name is new
                    if (m_previouslyRunningProcesses.contains(processName))
                    {
                        m_previouslyRunningProcesses.removeOne(processName);
                    }
                    else
                    {
                        emit processAdded(processName);
                    }

                    //add the process name to the list of process names
                    processNames.push_back(processName);
                }
            }

            // Release the handle to the process.
            CloseHandle( hProcess );
        }
    }

    //signal any processes that were removed
    foreach (processName, m_previouslyRunningProcesses)
    {
        emit processRemoved(processName);
    }

    emit updateProcesses(processNames);
    m_previouslyRunningProcesses = processNames;
}

#endif
