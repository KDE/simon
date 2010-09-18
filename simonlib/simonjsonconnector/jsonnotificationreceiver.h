/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_JSONNOTIFICATIONRECEIVER_H_D25D6E7E21C94C78AD6FA6148EA8F8C8
#define SIMON_JSONNOTIFICATIONRECEIVER_H_D25D6E7E21C94C78AD6FA6148EA8F8C8

#include <QByteArray>
class QObject;

class JsonNotificationReceiver
{
  private:
    QObject *m_reciever;
    QByteArray m_slot;

  public:
    JsonNotificationReceiver(QObject *reciever, const QByteArray& slot) 
      : m_reciever(reciever), m_slot(slot)
    {}
    ~JsonNotificationReceiver() {}

    QObject* receiver() { return m_reciever; }
    QByteArray slot() { return m_slot; }

};
#endif


