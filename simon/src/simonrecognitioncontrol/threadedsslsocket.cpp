/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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
#include <KDebug>

class SocketThread : public QThread {
protected:
  virtual void run() {
    exec();
  }
public:
    explicit SocketThread ( QObject* parent = 0 ) : QThread(parent) {}
};

ThreadedSSLSocket::ThreadedSSLSocket(QObject* parent): QIODevice(parent),
    socketThread(new SocketThread(this)),
    socket(new SimonSSLSocket(this))
{
  open(QIODevice::ReadWrite); //krazy:exclude=syscalls
  
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  
  writeBuffer.open(QIODevice::ReadWrite);
  
  socketThread->start();
  socket->moveToThread(socketThread);
  
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)));
  connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SIGNAL(sslErrors(QList<QSslError>)));
  connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
  connect(socket, SIGNAL(connected()), this, SIGNAL(connected()));
  connect(socket, SIGNAL(encrypted()), this, SIGNAL(encrypted()));
}

ThreadedSSLSocket::~ThreadedSSLSocket()
{
  delete socket;
}

void ThreadedSSLSocket::readFromSocket()
{
  readLock.lock();
  QByteArray newData = socket->readAll();
  readBuffer.append(newData);
  readLock.unlock();
  emit readyRead();
}

bool ThreadedSSLSocket::processBuffer()
{
  bytesToWriteLock.lock();
  
  socket->write(writeBuffer.buffer());
  writeBuffer.buffer().clear();
  writeBuffer.seek(0);
  
  bytesToWriteLock.unlock();
  return true;
}

qint64 ThreadedSSLSocket::readData(char* data, qint64 maxlen)
{
  QMutexLocker l(&readLock);
  maxlen = qMin(maxlen, (qint64) readBuffer.count());
  memcpy(data, readBuffer.constData(), maxlen);
  readBuffer.remove(0, maxlen);
  return maxlen;
}

QByteArray ThreadedSSLSocket::readAll()
{
  QMutexLocker l(&readLock);
  QByteArray out(readBuffer);
  readBuffer.clear();
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
  return readBuffer.count();
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

