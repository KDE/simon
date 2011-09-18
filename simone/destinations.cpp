#include "destinations.h"
#include "destinationcommand.h"
#include <QStringList>
#include <QDebug>

Destinations::Destinations()
{
    setupRoleNames();
}

void Destinations::setupRoleNames()
{
    QHash<int, QByteArray> names = roleNames();
    names.insert(Qt::UserRole+1, "destination");
    setRoleNames(names);
}

QVariant Destinations::data (const QModelIndex & index, int role) const
{
    if (role == Qt::UserRole+1)
        return static_cast<DestinationCommand*>(m_commands[index.row()])->destination();

    return CommandModel::data(index, role);
}

void Destinations::restore(const QStringList& config)
{
    QList<Command*> commands;
    foreach (const QString& c, config) {
        Command *destination = DestinationCommand::deSerialize(c);
        if (!destination)
            qWarning() << "Error serializing destination " << c;
        else
            commands << destination;
    }

    //reconstruct from settings
    init(commands);
    reset();
}

void Destinations::removeDestination(int index)
{
    if (index > m_commands.count())
            return;

    beginRemoveRows(QModelIndex(), index, index);
    delete m_commands.takeAt(index);
    endRemoveRows();
}

void Destinations::addDestination()
{
    int count = m_commands.count();
    beginInsertRows(QModelIndex(), count, count);
    m_commands << new DestinationCommand();
    endInsertRows();
}

void Destinations::updateDestination(int i, const QString& trigger, const QString& destination)
{
    if (m_loading)
        return;

    m_commands[i]->setTrigger(trigger);
    static_cast<DestinationCommand*>(m_commands[i])->setDestination(destination);

    QModelIndex lineToChange = index(i, 0);
    if (!lineToChange.isValid()) return;
    emit dataChanged(lineToChange, lineToChange);
}
