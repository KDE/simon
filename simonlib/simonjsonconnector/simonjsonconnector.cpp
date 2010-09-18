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

#include "simonjsonconnector.h"
#include "jsonnotificationreceiver.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <kio/accessmanager.h>
#include <KDebug>


SimonJsonConnector* SimonJsonConnector::instance = 0;

SimonJsonConnector::SimonJsonConnector(QObject *parent) : QObject(parent), m_accessManager(0)
{
  qRegisterMetaType<QNetworkReply*>("QNetworkReply*");

  m_accessManager = new KIO::AccessManager(parent);
  connect(m_accessManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(requestFinished(QNetworkReply*)));
  kDebug() << "Access manager: " << m_accessManager << "connecting...";
}

SimonJsonConnector* SimonJsonConnector::getInstance()
{
  if (!instance) instance = new SimonJsonConnector;
  return instance;
}

void SimonJsonConnector::requestFinished(QNetworkReply *reply)
{
  if (!notificationReceivers.contains(reply))
  {
    kDebug() << "Doesn't contain reply...";
    return;
  }

  //invoking receiver...
  JsonNotificationReceiver *notify = notificationReceivers.value(reply);

  QMetaObject::invokeMethod(notify->receiver(), notify->slot().data(), Qt::AutoConnection, QGenericReturnArgument(),
      Q_ARG(QNetworkReply*, reply));
  
  delete notify;
}

bool SimonJsonConnector::sendRequest(const QString& host, int port, const QString& url, const QString& request,
        QObject *receiver, const QByteArray& slot)
{
  QNetworkRequest networkRequest;
  QString urlSrc = "http://"+host+':'+QString::number(port)+'/'+url;
  networkRequest.setUrl(QUrl(urlSrc));
  kDebug() << "Url: " << urlSrc;

  QNetworkReply *reply = m_accessManager->post(networkRequest, request.toAscii());
  kDebug() << "Request: " << request.toAscii();

  if (receiver)
    notificationReceivers.insert(reply, new JsonNotificationReceiver(receiver, slot));

  return true;
}

SimonJsonConnector::~SimonJsonConnector()
{
  QList<QNetworkReply*> replies = notificationReceivers.keys();
  foreach (QNetworkReply* reply, replies)
  {
    delete notificationReceivers.value(reply);
    delete reply;
  }
}

