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


#ifndef THREADEDSSLSOCKET_H
#define THREADEDSSLSOCKET_H

#include <QIODevice>
#include <QMutex>
#include <QBuffer>
#include <QWaitCondition>
#include <QAbstractSocket>
#include <QSslError>

class SimonSSLSocket;
class QThread;
class SocketThread;

class ThreadedSSLSocket : public QIODevice
{
Q_OBJECT

signals:  
  void error(QAbstractSocket::SocketError);
  void sslErrors(QList<QSslError>);
  void disconnected();
  void encrypted();
  void connected();
  
  void requestConnect(const QString& hostName, quint16 port);
  void requestConnectEncrypted(const QString& hostName, quint16 port);
  void readyWrite();

public:
  explicit ThreadedSSLSocket(QObject* parent = 0);
  virtual ~ThreadedSSLSocket();
  
  QAbstractSocket::SocketState state() const;
  void abort();
  void setProtocol(QSsl::SslProtocol);
  void connectToHost(const QString& hostName, quint16 port);
  void connectToHostEncrypted(const QString& hostName, quint16 port);
  QList<QSslError> sslErrors() const;
  void ignoreSslErrors();
  void flush();
  QAbstractSocket::SocketError error();
  void disconnectFromHost();
  qint64 bytesAvailable() const;
  
  virtual void close();
  QString errorString() const;
  virtual QByteArray readAll();
  
  bool processBuffer();
  void readFromSocket();

protected:
  virtual qint64 readData(char* data, qint64 maxlen);
  virtual qint64 writeData(const char* data, qint64 len);
  
private:
  QMutex        readLock;
  QByteArray  readBuffer;
  QMutex       writeLock;
  QBuffer    writeBuffer;
  SocketThread  *socketThread;
  SimonSSLSocket *socket;
  
  QMutex              bytesToWriteLock;
//   QWaitCondition bytesToWriteCondition;
};


#endif // THREADEDSSLSOCKET_H
