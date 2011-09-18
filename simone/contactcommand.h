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


