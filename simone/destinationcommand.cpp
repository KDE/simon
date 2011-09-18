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
