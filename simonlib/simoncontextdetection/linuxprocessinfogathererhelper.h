#ifndef LINUXPROCESSINFOGATHERERHELPER_H
#define LINUXPROCESSINFOGATHERERHELPER_H

#include <QObject>

class LinuxProcessInfoGathererHelper : public QObject
{
    Q_OBJECT
public:
    explicit LinuxProcessInfoGathererHelper(QObject *parent = 0);

private:
    QString m_currentActiveWindowTitle;
    QString m_currentActiveWindowProgram;

signals:
    void activeWindowTitleChanged(QString);
    void activeWindowProcessChanged(QString);

public slots:
    void checkActiveWindow();

};

#endif // LINUXPROCESSINFOGATHERERHELPER_H
