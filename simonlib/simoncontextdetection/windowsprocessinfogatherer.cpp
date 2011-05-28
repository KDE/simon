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

                    //add the process name to the list of process names
                    processNames.push_back(processName);

                    qDebug() << processName;

                    //check to see if the name is new
                    if (m_previouslyRunningProcesses.contains(processName))
                    {
                        m_previouslyRunningProcesses.removeOne(processName);
                    }
                    else
                    {
                        emit processAdded(processName);
                    }
                }
            }

            // Release the handle to the process.
            CloseHandle( hProcess );
        }
    }

    //signal any processes that were removed
    foreach (processName, m_previouslyRunningProcesses)
    {
        //check to make sure it was not just one of many instances of the process name
        if (!processNames.contains(processName))
        {
            emit processRemoved(processName);
        }
    }

    emit updateProcesses(processNames);
    m_previouslyRunningProcesses = processNames;
}

#endif
