#ifndef WINDOWSPROCESSINFOGATHERER_H
#define WINDOWSPROCESSINFOGATHERER_H

#include "processinfogatherer.h"

#ifdef Q_OS_WIN32

class WindowsProcessInfoGatherer : public ProcessInfoGatherer
{
    Q_OBJECT
public:
    explicit WindowsProcessInfoGatherer(QObject *parent = 0);

signals:

public slots:
    void checkCurrentProcesses();
};

#endif

#endif // WINDOWSPROCESSINFOGATHERER_H
