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
#include "networkcontrol.h"
#include <QDebug>
#include "simonsocket.h"
#include "settings.h"
#include "databaseaccess.h"
#include <QMessageBox>
#include <KLocalizedString>

NetworkControl::NetworkControl()
{
	this->sockets = new SocketList();
	db = 0;
}

void NetworkControl::sendDbError(QString error)
{
	emit errorOccured(error, -1);
}

bool NetworkControl::initDb()
{
	if (db) return true;

	this->db = new DatabaseAccess();
	connect (db, SIGNAL(error(QString)), this, SLOT(sendDbError(QString)));
	connect (db, SIGNAL(connected(QString)), this, SLOT(connectedToDb(QString)));
	if (!db->connectTo()) {
		db->deleteLater();
		db = 0;
		return false;
	}
	return true;
}

void NetworkControl::connectedToDb(QString db)
{
	emit info(i18n("Verbunden zur Datenbank auf Host %1", db),-1 /*id*/);
}

/**
 * @brief Stops the Server and disconnects all clients
 *
 *	@author Peter Grasch
 */
void NetworkControl::stopServer()
{
	close();
	if (!sockets) return;
	for (int i=0; i < sockets->count(); i++)
	{
		sockets->at(i)->close();
 		//sockets->at(i)->deleteLater();
	}
	db->deleteLater();
	db = 0;
}

/**
 * @brief Starts the Server
 *
 *	@author Peter Grasch
 */
void NetworkControl::startServer(QHostAddress allowedClient, quint16 port)
{
	listen( allowedClient, port );
}

void NetworkControl::incomingConnection (int descriptor)
{
	SimonSocket *socket = new SimonSocket(descriptor, db);

	//TODO: Implement the "ForceEncryption" setting which only allows encrypted settings
	//(configuration item)
// 	socket->setProtocol(QSsl::SslV2);
// 	socket->setCiphers(Settings::getS("Cipher"));
// 	socket->startServerEncryption();
// 	socket->ignoreSslErrors();
	
	connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), 
		this, SLOT(connectionClosing(QAbstractSocket::SocketState)));
	connect(socket, SIGNAL(info(QString, int)), this, SIGNAL(info(QString, int)));
	connect(socket, SIGNAL(errorOccured(QString,int)), this, SIGNAL(errorOccured(QString,int)));
	connect(socket, SIGNAL(wordSent(QString,int)), this, SIGNAL(wordSent(QString,int)));
	
	connect(socket, SIGNAL(sendId(int, QByteArray)),this,SLOT(sendSocket(int, QByteArray)));
	
	emit connectionReceived(socket->localAddress().toString(), descriptor);
	
	//TextSync
	//this->client = this->server.nextPendingConnection();
	//connect(client,SIGNAL(readyRead()),this,SLOT(readClient()));
	//_______
	
	sockets->append(socket);
}


/**
 * \brief Relays the given string to all the sockets simulating a recognition result
 * \author Peter Grasch
 * @param word The "recognized" word
 */
void NetworkControl::sendWord(QString word)
{
	if (!sockets) return;
	for (int i=0; i < this->sockets->count(); i++)
		sockets->at(i)->sendWord(word, "" /*sampa*/, "" /*samparaw*/);
}

/**
 * @brief Reloads the Server
 * 
 * 	Calls the stopServer() and the startServer() functions
 *
 *	@author Peter Grasch
 *	@see startServer()
 *	@see stopServer()
 */
void NetworkControl::reloadServer()
{
	stopServer();
	startServer();
}

/**
 * @brief Sends a word through the socket
 * 
 * 	Writes the Latin1 encoded Data of the supplied Data to the socket
 *
 *	@author Peter Grasch
 *	@param QString data
 *	The data to write
 */
void NetworkControl::sendString(QString data)
{
	for (int i=0; i<sockets->count(); i++)
		sockets->at(i)->write(data.toUtf8());
}


/**
 * @brief Reacts on a client who closes the connection
 * 
 *	@author Peter Grasch
 */
void NetworkControl::connectionClosing(QAbstractSocket::SocketState state)
{
	if (state == QAbstractSocket::ClosingState)
	{
		for (int i=0; i<sockets->count(); i++)
		{
			if (sockets->at(i)->state() == state)
			{
				emit connectionDropped(sockets->at(i)->localAddress().toString(),
					sockets->at(i)->socketDescriptor());
				sockets->takeAt(i)->deleteLater();
				i--;
			}
		}
	}
}


void NetworkControl::readClient()
{
	
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
NetworkControl::~NetworkControl()
{
	if (sockets) delete sockets;
	if (db) db->deleteLater();
}




//TextsYnc
void NetworkControl::sendSocket(int sd, QByteArray out)
{	
	for (int i = 0; i < sockets->count(); i++)
	{
		if (sockets->at(i)->socketDescriptor() == sd)
		{
			NetworkReader *reader = new NetworkReader(sockets->at(i), out);
			reader->createMergeContainer();
		}
	}
	
	
}
	
//______
