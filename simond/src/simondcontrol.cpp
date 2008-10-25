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

#include "simondcontrol.h"
#include "databaseaccess.h"

#include <KDebug>

SimondControl::SimondControl(QObject *parent) : QTcpServer(parent)
{
	db = new DatabaseAccess(this);
	connect (db, SIGNAL(error(const QString&)), this, SLOT(handleError(const QString&)));
}


bool SimondControl::init()
{
	if (!db->init())
		return false;

	startServer();

	return true;
}


void SimondControl::handleError(const QString& error)
{
	kDebug() << error;
}

void SimondControl::startServer(const QHostAddress& allowedClient, quint16 port)
{
	kDebug() << "Starting server listening on port " << port;

	listen(allowedClient, port);
}

void SimondControl::stopServer()
{
	kDebug() << "Stopping server";

	close();
	
	foreach (ClientSocket *client, clients)
	{
		client->close();
	}

	db->deleteLater();
	db = 0;
}

void SimondControl::incomingConnection (int descriptor)
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
// 	connect(socket, SIGNAL(info(QString, int)), this, SIGNAL(info(QString, int)));
// 	connect(socket, SIGNAL(errorOccured(QString,int)), this, SIGNAL(errorOccured(QString,int)));
// 	connect(socket, SIGNAL(wordSent(QString,int)), this, SIGNAL(wordSent(QString,int)));
// 	connect(socket, SIGNAL(sendId(int, QByteArray)),this,SLOT(sendSocket(int, QByteArray)));
	

	
	clients << clientSocket;
}



void SimondControl::connectionClosing(QAbstractSocket::SocketState state)
{
	if (state != QAbstractSocket::ClosingState) return;

	for (int i=0; i<clients.count(); i++)
	{
		if (clients[i]->state() == state)
		{
			kDebug() << "Connection dropped from " << clients[i]->localAddress().toString();
			clients.takeAt(i)->deleteLater();
			i--;
		}
	}
}



//debug
void SimondControl::sendString(const QString& data)
{
	foreach (ClientSocket *client, clients)
		client->sendRecognitionResult(data, "" /*sampa*/, "" /*samparaw*/);
}



SimondControl::~SimondControl()
{
	stopServer();
}
