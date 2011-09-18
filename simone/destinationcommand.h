#ifndef DESTINATIONCOMMAND_H
#define DESTINATIONCOMMAND_H

#include "command.h"

class DestinationCommand : public Command
{
private:
    QString m_destination;

public:
    DestinationCommand(const QString& trigger=QString(), const QString& destination=QString());
    static DestinationCommand* deSerialize(const QString& data);
    QString serialize();

    QString name() { return m_destination; }
    QString destination() { return m_destination; }

    void setDestination(const QString& name) { m_destination = name; }
};

#endif // DESTINATIONCOMMAND_H
