#include "processinfogatherer.h"

ProcessInfoGatherer::ProcessInfoGatherer(QObject *parent) :
    QThread(parent)
{
}

void ProcessInfoGatherer::run()
{
    forever
    {
        checkCurrentProcesses();

        this->sleep(1);
    }
}
