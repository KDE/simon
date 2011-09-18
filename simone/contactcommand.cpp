#include "contactcommand.h"
#include <QStringList>
#include <QDebug>

ContactCommand::ContactCommand(const QString& trigger, const QString& displayName, const QString& number) :
    Command(trigger), m_displayName(displayName), m_number(number)
{
    qDebug() << "Creating contact command: " << trigger << displayName << number;
}

ContactCommand* ContactCommand::deSerialize(const QString& data)
{
    QStringList parts = data.split("||");
    if (parts.count() != 3)
        return 0;

    return new ContactCommand(parts[0], parts[1], parts[2]);
}

QString ContactCommand::serialize()
{
    qDebug() << "Storing command: " << trigger() << m_number;
    return QString("%1||%2||%3").arg(trigger()).arg(m_displayName).arg(m_number);
}

QString ContactCommand::name()
{
    if (!m_displayName.isEmpty())
        return QString("%1 (%2)").arg(m_displayName).arg(m_number);
    return m_number;
}
