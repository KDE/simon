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
#include "qt-atspi.h"
#include <QDBusArgument>

#define QSPI_OBJECT_PATH_PREFIX  "/org/a11y/atspi/accessible/"
#define QSPI_OBJECT_PATH_NULL    QSPI_OBJECT_PATH_PREFIX "null"

QSpiObjectReference::QSpiObjectReference()
    : path(QDBusObjectPath(QSPI_OBJECT_PATH_NULL))
{}


/* QSpiObjectReference */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiObjectReference &address)
{
    argument.beginStructure();
    argument << address.service;
    argument << address.path;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiObjectReference &address)
{
    argument.beginStructure();
    argument >> address.service;
    argument >> address.path;
    argument.endStructure();
    return argument;
}