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

#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

class Command
{
protected:
    QString m_trigger;
public:
    Command(const QString& trigger);
    virtual ~Command() {}
    QString trigger() { return m_trigger; }
    virtual QString name() { return m_trigger; }
    void setTrigger(const QString& trigger) { m_trigger = trigger; }
    virtual QString serialize()=0;
};

#endif // COMMAND_H
