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

#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <QAbstractListModel>

#include <QModelIndex>
#include <QList>

class Command;

class CommandModel : public QAbstractListModel
{
protected:
    bool m_loading;
    QList<Command*> m_commands;
    bool init(QList<Command*> commands);

public:
    CommandModel();
    ~CommandModel();
    int columnCount(const QModelIndex& parent = QModelIndex()) const {
        Q_UNUSED(parent);
        return 1;
    }
    QModelIndex parent(const QModelIndex &child) const {
        Q_UNUSED(child);
        return QModelIndex();
    }
    int rowCount(const QModelIndex &parent) const;
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

    void startLoading() { m_loading = true; }
    void stopLoading() { m_loading = false; }

    Command* getCommand(const QString& trigger);
    QStringList store();

    virtual void restore(const QStringList& config)=0;

};

#endif // COMMANDMODEL_H
