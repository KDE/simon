#include "linuxprocessinfogatherer.h"
#include <QRegExp>
#include <QDir>

LinuxProcessInfoGatherer::LinuxProcessInfoGatherer(QObject *parent) :
    ProcessInfoGatherer(parent)
{
}

void LinuxProcessInfoGatherer::checkCurrentProcesses()
{
    QRegExp digits;
    QDir procDir;
    QFileInfoList files;
    QFile commFile;
    QString processName;
    QStringList processNames;

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

                //add the process name to the list of process names
                processNames.push_back(processName);

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
