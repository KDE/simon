#include "destinationcommand.h"
#include <QStringList>
#include <QDebug>

DestinationCommand::DestinationCommand(const QString& trigger, const QString& destination) :
    Command(trigger), m_destination(destination)
{
    qDebug() << "Creating destination command: " << trigger << destination;
}

DestinationCommand* DestinationCommand::deSerialize(const QString& data)
{
    QStringList parts = data.split("||");
    if (parts.count() != 2)
        return 0;

    return new DestinationCommand(parts[0], parts[1]);
}

QString DestinationCommand::serialize()
{
    qDebug() << "Storing command: " << trigger() << " - " << m_destination;
    return QString("%1||%2").arg(trigger()).arg(m_destination);
}
