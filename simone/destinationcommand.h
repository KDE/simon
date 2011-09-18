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
