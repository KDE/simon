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


#include "threadedsslsocket.h"
#include "simonsslsocket.h"
#include <QMutexLocker>
#include <QSslSocket>
#include <QThread>

ThreadedSSLSocket::ThreadedSSLSocket(QObject* parent): QIODevice(parent),
    socketThread(new QThread(this)),
    socket(new SimonSSLSocket(this))
{
  open(QIODevice::ReadWrite); //krazy:exclude=syscalls
  
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  
  readBuffer.open(QIODevice::ReadWrite);
  writeBuffer.open(QIODevice::ReadWrite);
  
  socket->moveToThread(socketThread);
  connect(socketThread, SIGNAL(started()), socket, SLOT(processBuffer()));
  
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)));
  connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SIGNAL(sslErrors(QList<QSslError>)));
  connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
  connect(socket, SIGNAL(connected()), this, SIGNAL(connected()));
  connect(socket, SIGNAL(encrypted()), this, SIGNAL(encrypted()));
  
  socketThread->start();
}

ThreadedSSLSocket::~ThreadedSSLSocket()
{
  delete socket;
}

void ThreadedSSLSocket::readFromSocket()
{
  do {
    QMutexLocker l(&readLock);
    readBuffer.buffer().append(socket->readAll());
    emit readyRead();
  } while (socket->bytesAvailable());
}

bool ThreadedSSLSocket::processBuffer()
{
  bytesToWriteLock.lock();
  
  if (!writeBuffer.buffer().isEmpty()) {
    QByteArray toWrite = writeBuffer.buffer();
    socket->write(toWrite);
    writeBuffer.buffer().clear();
    writeBuffer.seek(0);
  }
  bytesToWriteLock.unlock();
  return true;
}

qint64 ThreadedSSLSocket::readData(char* data, qint64 maxlen)
{
  QMutexLocker l(&readLock);
  qint64 out =  readBuffer.read(data, maxlen);
  return out;
}

qint64 ThreadedSSLSocket::writeData(const char* data, qint64 len)
{
  bytesToWriteLock.lock();
  qint64 out = writeBuffer.write(data, len);
  bytesToWriteLock.unlock();
  emit readyWrite();
  return out;
}

void ThreadedSSLSocket::close()
{
  socket->close();
}

QString ThreadedSSLSocket::errorString() const
{
  return socket->errorString();
}

qint64 ThreadedSSLSocket::bytesAvailable() const
{
  return readBuffer.bytesAvailable();
}


//ssl socket wrappers
QAbstractSocket::SocketState ThreadedSSLSocket::state() const
{
  return socket->state();
}
void ThreadedSSLSocket::abort()
{
  socket->abort();
}
void ThreadedSSLSocket::connectToHost(const QString& hostName, quint16 port)
{
  emit requestConnect(hostName, port);
}
void ThreadedSSLSocket::connectToHostEncrypted(const QString& hostName, quint16 port)
{
  emit requestConnectEncrypted(hostName, port);
}
void ThreadedSSLSocket::setProtocol(QSsl::SslProtocol protocol)
{
  socket->setProtocol(protocol);
}
QAbstractSocket::SocketError ThreadedSSLSocket::error()
{
  return socket->error();
}
void ThreadedSSLSocket::flush()
{
  socket->flush();
}
void ThreadedSSLSocket::ignoreSslErrors()
{
  socket->ignoreSslErrors();
}
QList<QSslError> ThreadedSSLSocket::sslErrors() const
{
  return socket->sslErrors();
}
void ThreadedSSLSocket::disconnectFromHost()
{
  socket->disconnectFromHost();
}

