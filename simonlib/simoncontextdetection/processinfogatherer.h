#ifndef PROCESSINFOGATHERER_H
#define PROCESSINFOGATHERER_H

#include <QThread>
#include <QStringList>

class ProcessInfoGatherer : public QThread
{
    Q_OBJECT
public:
    explicit ProcessInfoGatherer(QObject *parent = 0);

protected:
    void run();
    QStringList m_previouslyRunningProcesses;
    virtual void checkCurrentProcesses()=0;

signals:
    void processAdded(QString);
    void processRemoved(QString);
    void updateProcesses(QStringList);

public slots:


};

#endif // PROCESSINFOGATHERER_H
