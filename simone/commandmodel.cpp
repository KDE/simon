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

#include "commandmodel.h"
#include "command.h"
#include <QHash>
#include <QStringList>

CommandModel::CommandModel() :
    m_loading(false)
{
    QHash<int, QByteArray> names = roleNames();
    names.insert(Qt::UserRole, "trigger");
    setRoleNames(names);
}

CommandModel::~CommandModel()
{
    qDeleteAll(m_commands);
}

bool CommandModel::init(QList<Command *> commands)
{
    qDeleteAll(m_commands);
    m_commands.clear();

    m_commands = commands;
    return true;
}

int CommandModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_commands.count();
}

QVariant CommandModel::data (const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole)
        return m_commands[index.row()]->name();
    if (role == Qt::UserRole)
        return m_commands[index.row()]->trigger();
    return QVariant();
}

Command* CommandModel::getCommand(const QString& trigger)
{
    foreach (Command *c, m_commands)
        if (c->trigger() == trigger)
            return c;
    return 0;
}


QStringList CommandModel::store()
{
    QStringList out;
    foreach (Command* c, m_commands)
        out << c->serialize();
    return out;
}
