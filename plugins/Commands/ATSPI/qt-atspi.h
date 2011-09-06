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

#ifndef QT_ATSPI_H
#define QT_ATSPI_H
#define QSPI_OBJECT_PATH_ACCESSIBLE  "/org/a11y/atspi/accessible"
#define QSPI_OBJECT_PATH_ACCESSIBLE_NULL  QSPI_OBJECT_PATH_ACCESSIBLE"/null"

#include <QList>
#include <QString>
#include <QDBusObjectPath>
#include <QDBusConnection>
#include <QDBusArgument>

typedef QList<uint> QSpiUIntList;
Q_DECLARE_METATYPE(QSpiUIntList);

namespace QtATSPI {
  void registerTypes();
}

struct QSpiObjectReference
{
    QString service;
    QDBusObjectPath path;

    QSpiObjectReference();
    QSpiObjectReference(const QDBusConnection& connection, const QDBusObjectPath& path)
        : service(connection.baseService()), path(path) {}
};

Q_DECLARE_METATYPE(QSpiObjectReference);


QDBusArgument &operator<<(QDBusArgument &argument, const QSpiObjectReference &address);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiObjectReference &address);

#endif