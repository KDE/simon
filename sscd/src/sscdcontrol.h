/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SSCDCONTROL_H
#define SSCDCONTROL_H

#include "clientsocket.h"

#include <QHostAddress>
#include <QTcpServer>
#include <QList>

class DatabaseAccess;
class QTcpServer;

class SSCDControl : public QTcpServer
{
	Q_OBJECT
	private:
		QList<ClientSocket*> clients;
		DatabaseAccess *db;

	private slots:
		void handleError(const QString& error);
		
		void startServer(const QHostAddress& allowedClient=QHostAddress::Any, quint16 port=4444);
		void stopServer();
		
		void incomingConnection (int descriptor);
		
		void connectionClosing(QAbstractSocket::SocketState state);
		

	public:
		SSCDControl(QObject *parent=0);

		bool init();
			       
		~SSCDControl();
		
};

#endif
