#include "processinfo.h"
#include "linuxprocessinfogatherer.h"
#include "windowsprocessinfogatherer.h"

ProcessInfo* ProcessInfo::m_instance;

ProcessInfo::ProcessInfo()
{
    #ifdef Q_OS_WIN32
    m_gatherer = new WindowsProcessInfoGatherer();
    #else
    m_gatherer = new LinuxProcessInfoGatherer();
    #endif

    connect(m_gatherer, SIGNAL(processAdded(QString)),
            this, SLOT(checkAddedProcesses(QString)));
    connect(m_gatherer, SIGNAL(processRemoved(QString)),
            this, SLOT(checkRemovedProcesses(QString)));
    connect(m_gatherer, SIGNAL(updateProcesses(QStringList)),
            this, SLOT(checkRunningProcesses(QStringList)));

    m_gatherer->start(QThread::LowPriority);
}

ProcessInfo* ProcessInfo::instance()
{
    if (!m_instance)
        m_instance = new ProcessInfo();

    return m_instance;
}
