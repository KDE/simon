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
