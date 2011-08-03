#include "linuxprocessinfogathererhelper.h"
#include <KWindowSystem>
#include <NETWinInfo>
#include <QX11Info>
#include <QFile>
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
    QFile commFile;
    QString processName;
    QString windowName;
    QString strPID;

    strPID.setNum(netInfo.pid());
    commFile.setFileName("/proc/" + strPID + "/comm");
    if (commFile.exists())
    {
        commFile.open(QFile::ReadOnly);
        processName = QString(commFile.readAll());
        commFile.close();
        //get rid of the newline character
        processName.chop(1);

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
