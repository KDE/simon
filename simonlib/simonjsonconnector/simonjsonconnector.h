/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SIMONJSONCONNECTOR_H_D25D6E7E21C94C78AD6FA6148EA8F8C8
#define SIMON_SIMONJSONCONNECTOR_H_D25D6E7E21C94C78AD6FA6148EA8F8C8

#include "simonjsonconnector_export.h"
#include <qt5/QtCore/QHash>
#include <qt5/QtCore/QObject>

class QNetworkReply;
class JsonNotificationReceiver;

namespace KIO
{
  class AccessManager;
}

class SIMONJSONCONNECTOR_EXPORT SimonJsonConnector : public QObject
{
  Q_OBJECT
  private:
    QHash<QNetworkReply*, JsonNotificationReceiver*> notificationReceivers;

    KIO::AccessManager *m_accessManager;
    static SimonJsonConnector *instance;

    SimonJsonConnector(QObject *parent=0);

  private slots:
    void requestFinished(QNetworkReply *reply);

  public:
    static SimonJsonConnector* getInstance();

    bool sendRequest(const QString& host, int port, const QString& url, const QString& request,
        QObject *receiver=0, const QByteArray& slot=QByteArray());
    ~SimonJsonConnector();

};
#endif

