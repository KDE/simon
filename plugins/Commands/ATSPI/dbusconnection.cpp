/*
 * D-Bus AT-SPI, Qt Adaptor
 *
 * Copyright 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "dbusconnection.h"

#include <QtCore/QDebug>

#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

DBusConnection::DBusConnection()
    : dbusConnection(connectDBus())
{}

QDBusConnection DBusConnection::connectDBus()
{
  QString address = getAccessibilityBusAddress();

  if (!address.isEmpty()) {
    QDBusConnection c = QDBusConnection::connectToBus(address, "a11y");

    if (c.isConnected()) {
      qDebug() << "Connected to accessibility bus at: " << address;
      return c;
    }

    qWarning("Found Accessibility DBus address but cannot connect. Falling back to session bus.");
  } else {
    qWarning("Accessibility DBus not found. Falling back to session bus.");
  }

  QDBusConnection c = QDBusConnection::sessionBus();

  if (!c.isConnected()) {
    qWarning("Could not connect to DBus.");
  }

  return QDBusConnection::sessionBus();
}

QString DBusConnection::getAccessibilityBusAddress() const
{
  Display* bridge_display = QX11Info::display();

  Atom actualType;
  int actualFormat;
  char *propData = 0;
  unsigned long nItems;
  unsigned long leftOver;
  Atom AT_SPI_BUS = XInternAtom (bridge_display, "AT_SPI_BUS", False);
  XGetWindowProperty (bridge_display,
                      XDefaultRootWindow (bridge_display),
                      AT_SPI_BUS, 0L,
                      (long) BUFSIZ, False,
                      (Atom) 31, &actualType, &actualFormat,
                      &nItems, &leftOver,
                      (unsigned char **) (void *) &propData);

  QString busAddress = QString::fromLocal8Bit(propData);

  XFree(propData);
  return busAddress;
}

QDBusConnection& DBusConnection::connection()
{
  return dbusConnection;
}
