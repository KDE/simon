/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
