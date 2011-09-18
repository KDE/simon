#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

class Command
{
protected:
    QString m_trigger;
public:
    Command(const QString& trigger);
    virtual ~Command() {}
    QString trigger() { return m_trigger; }
    virtual QString name() { return m_trigger; }
    void setTrigger(const QString& trigger) { m_trigger = trigger; }
    virtual QString serialize()=0;
};

#endif // COMMAND_H
