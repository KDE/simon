#ifndef LINUXPROCESSINFOGATHERER_H
#define LINUXPROCESSINFOGATHERER_H

#include "processinfogatherer.h"

class LinuxProcessInfoGatherer : public ProcessInfoGatherer
{
    Q_OBJECT
public:
    explicit LinuxProcessInfoGatherer(QObject *parent = 0);

protected:
    void checkCurrentProcesses();

signals:

public slots:

};

#endif // LINUXPROCESSINFOGATHERER_H
