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

#include "windows.h"
#include "tchar.h"
#include "psapi.h"
#include <QDebug>

//WINAPI:
//krazy:excludeall=captruefalse

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
    for (i=0; i<cProcesses; ++i)
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

                    //add the process name to the list of current process names
                    m_currentlyRunningProcesses.push_back(processName);
                }
            }

            // Release the handle to the process.
            CloseHandle( hProcess );
        }
    }
}

void WindowsProcessInfoGatherer::checkActiveWindow()
{
    HWND hwnd;
    DWORD activeWindowProcess;
    HANDLE hProcess;
    HMODULE hMod;
    TCHAR szProcessName[MAX_PATH];
    QString processName;
    WCHAR wWindowName[1024];
    QString windowName;
    unsigned int i;

    //get the current foreground window
    hwnd=GetForegroundWindow();

    if (hwnd)
    {
        //get the foreground window name
        GetWindowText(hwnd,wWindowName,1024);

        //get the process ID of the foreground window
        GetWindowThreadProcessId(hwnd, &activeWindowProcess);

        if (activeWindowProcess != 0)
        {
            //get a process handle for the foreground window process
            hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, activeWindowProcess );

            if (hProcess != 0)
            {
                //get the name of the foreground window process
                GetModuleBaseName( hProcess,
                                  (HINSTANCE__*) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                                  szProcessName,
                                  sizeof(szProcessName)/sizeof(TCHAR) );

                //Make the window and process name a QString
#ifdef UNICODE
                processName = QString::fromUtf16((ushort*)szProcessName);
                windowName = QString::fromUtf16((ushort*)wWindowName);
#else
                processName = QString::fromLocal8Bit(szProcessName);
                windowName = QString::fromLocal8Bit(wWindowName);
#endif

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

            // Release the handle to the process.
            CloseHandle( hProcess );
        }
    }
}


