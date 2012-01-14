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


#ifndef SIMONSSLSOCKET_H
#define SIMONSSLSOCKET_H

#include <QSslSocket>

class ThreadedSSLSocket;

class SimonSSLSocket : public QSslSocket
{
Q_OBJECT

public:
  SimonSSLSocket(ThreadedSSLSocket *socket, QObject* parent = 0);
  virtual qint64 writeData(const char* data, qint64 len);
  virtual qint64 readData(char* data, qint64 maxlen);
    
private slots:
  void processBuffer();
  void readFromSocket();
  
private slots:
  void connectToHostWrapper(const QString& hostName, quint16 port);
  void connectToHostEncryptedWrapper(const QString& hostName, quint16 port);
    
private:
  ThreadedSSLSocket *socket;
};

#endif // SIMONSSLSOCKET_H
