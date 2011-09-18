#ifndef CONTACTCOMMAND_H
#define CONTACTCOMMAND_H

#include "command.h"

class ContactCommand : public Command
{
private:
    QString m_displayName;
    QString m_number;

public:
    ContactCommand(const QString& trigger=QString(), const QString& displayName=QString(), const QString& number=QString());
    static ContactCommand* deSerialize(const QString& data);
    QString serialize();

    QString number() { return m_number; }
    QString name();

    void setDisplayName(const QString& name) { m_displayName = name; }
    void setNumber(const QString& number) { m_number = number; }
};

#endif // CONTACTCOMMAND_H


