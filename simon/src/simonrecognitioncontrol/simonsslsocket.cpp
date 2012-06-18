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


#include "simonsslsocket.h"
#include "threadedsslsocket.h"
#include <QThread>

SimonSSLSocket::SimonSSLSocket(ThreadedSSLSocket *socket, QObject* parent): QSslSocket(parent),
    socket(socket)
{
  connect(socket, SIGNAL(requestConnect(QString,quint16)), this, SLOT(connectToHostWrapper(QString,quint16)), Qt::QueuedConnection);
  connect(socket, SIGNAL(requestConnectEncrypted(QString,quint16)), this, SLOT(connectToHostEncryptedWrapper(QString,quint16)), Qt::QueuedConnection);
  
  connect(socket, SIGNAL(readyWrite()), this, SLOT(processBuffer()), Qt::QueuedConnection);
  connect(this, SIGNAL(readyRead()), this, SLOT(readFromSocket()), Qt::DirectConnection);
}

void SimonSSLSocket::connectToHostWrapper(const QString& hostName, quint16 port)
{
  connectToHost(hostName, port);
}

void SimonSSLSocket::connectToHostEncryptedWrapper(const QString& hostName, quint16 port)
{
  connectToHostEncrypted(hostName, port);
}

void SimonSSLSocket::processBuffer()
{
  socket->processBuffer();
}

void SimonSSLSocket::readFromSocket()
{
  socket->readFromSocket();
}

