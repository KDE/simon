/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "sscdcontrol.h"
#include "basedirectory.h"
#include "databaseaccess.h"
#include <QSettings>
#include <QDir>

SSCDControl::SSCDControl(QObject* parent) : QTcpServer(parent)
,db(new DatabaseAccess(this))
{
  connect (db, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
}


bool SSCDControl::init()
{
  qDebug() << "Reading: " << SSCD_BASE_DIRECTORY+QDir::separator()+"sscd.conf";
  QSettings settings(SSCD_BASE_DIRECTORY+QDir::separator()+"sscd.conf", QSettings::IniFormat);

  QString dbType = settings.value("DatabaseType", "QMYSQL").toString();
  QString dbHost = settings.value("DatabaseHost", "127.0.0.1").toString();
  qint16  dbPort = settings.value("DatabasePort", 3306).toInt();
  QString dbName = settings.value("DatabaseName", "ssc").toString();
  QString dbUser = settings.value("DatabaseUser", "sscuser").toString();
  QString dbPassword = settings.value("DatabasePassword", "").toString();
  QString dbOptions = settings.value("DatabaseOptions", "").toString();

  int serverPort = settings.value("Port", 4440).toInt();

  bool bindTo = settings.value("Bind", false).toBool();
  QString bindHost = settings.value("BindHost", "127.0.0.1").toString();

  if (!db->init(dbType, dbHost, dbPort, dbName, dbUser, dbPassword, dbOptions)) {
    qWarning() << "Database could not be opened. Aborting.";
    return false;
  }

  if (bindTo)
    startServer(QHostAddress(bindHost), serverPort);
  else
    startServer(QHostAddress::Any, serverPort);

  return true;
}


void SSCDControl::handleError(const QString& error)
{
  QFile f(SSCD_BASE_DIRECTORY+QDir::separator()+"error.log");
  f.open(QIODevice::Append|QIODevice::Append);
  f.write(SSCD_BASE_DIRECTORY.toUtf8()+'\n'+error.toUtf8()+'\n');
  qWarning() << error;
}


void SSCDControl::startServer(const QHostAddress& allowedClient, quint16 port)
{
  qWarning() << "Starting server listening on port " << port;

  listen(allowedClient, port);
}


void SSCDControl::stopServer()
{
  qWarning() << "Stopping server";

  close();

  foreach (ClientSocket *client, clients) {
    client->close();
  }

  db->deleteLater();
  db = 0;
}


void SSCDControl::incomingConnection (int descriptor)
{
  ClientSocket *clientSocket = new ClientSocket(descriptor, db, this);

  //TODO: Implement the "ForceEncryption" setting which only allows encrypted settings
  //(configuration item)
  // 	socket->setProtocol(QSsl::SslV2);
  // 	socket->setCiphers(Settings::getS("Cipher"));
  // 	socket->startServerEncryption();
  // 	socket->ignoreSslErrors();

  connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
    this, SLOT(connectionClosing(QAbstractSocket::SocketState)));

  clients << clientSocket;
}


void SSCDControl::connectionClosing(QAbstractSocket::SocketState state)
{
  if (state != QAbstractSocket::ClosingState) return;

  for (int i=0; i<clients.count(); i++) {
    if (clients[i]->state() == state) {
      qDebug() << "Connection dropped from " << clients[i]->peerAddress().toString();
      clients.takeAt(i)->deleteLater();
      i--;
    }
  }
}


SSCDControl::~SSCDControl()
{
  stopServer();
  db->deleteLater();
}
