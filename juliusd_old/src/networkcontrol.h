/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef NETWORKCONTROL_H
#define NETWORKCONTROL_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>

#include "sync_networkreader.h"

/**
 *	@class NetworkControl
 *	@brief Connects to the server and sends the words to it
 *
 *	@version 0.1
 *	@date 11.02.2007
 *	@author Peter Grasch
 */

class SimonSocket;
class DatabaseAccess;
typedef QList<SimonSocket*> SocketList;

class NetworkControl : public QTcpServer {
	Q_OBJECT
private:
	SocketList *sockets; //!< This will hold all the connected clients
	DatabaseAccess *db;

signals:
	void connectionDropped(QString, int id);
	void connectionReceived(QString, int id);
	void errorOccured(QString, int id);
	void info(QString, int id);
	void wordSent(QString, int);
private slots:
	void connectedToDb(QString db);
	void sendDbError(QString error);
public slots:
	void startServer(QHostAddress allowedClient=QHostAddress::Any, quint16 port=4444);
	void stopServer();
	void reloadServer();
	bool initDb();
	void sendWord(QString word);

	void incomingConnection (int descriptor);

	void connectionClosing(QAbstractSocket::SocketState state);

	//TextsYnc
	void sendSocket(int sd, QByteArray out);
	//______

public:
    NetworkControl();
	DatabaseAccess* getDbManager() { return db; }

    ~NetworkControl();

};

#endif
