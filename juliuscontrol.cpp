//
// C++ Implementation: juliuscontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "juliuscontrol.h"
#include <QByteArray>
#include <QTcpSocket>

/**
 *	@brief Constructor
 *	
 *	Creates the Server and sets it up to listen on the supplied port
 *	Default: 4444
 *	
 *	@author Peter Grasch
 *	@param qint16 port
 *	Port the Server should listen to
 */
JuliusControl::JuliusControl()
{
	socket = new QTcpSocket();
	connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(connectionLost()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(recognised()));
}


/**
 * @brief Connects to a juliusd server
 * 
 * 	Connects the QTcpSocket socket (member) to the Host described
 * 	with server (ip or hostname) (default: 127.0.0.1) and port 
 * 	(default: 4444)
 *
 *	@author Peter Grasch
 *	@param QString server
 *	Serverstring - either an ip or a hostname (default: 127.0.0.1)
 *	@param quint16 port
 *	Port (default: 4444)
 */
void JuliusControl::connectTo(QString server, quint16 port)
{
	socket->connectToHost( server, port );
	
	if (!(socket->waitForConnected(5000)))
	{
		emit error(socket->errorString());
		return;
	}
}

/**
 * \brief Returns wether the socket is connected to a valid juliusd server
 * 
 * \author Peter Grasch
 * \return bool
 * True = is connected
 */
bool JuliusControl::isConnected()
{
	if (!socket) return false;
	
	return (socket->state() == QAbstractSocket::ConnectedState);
}

/**
 *	@brief A word has been recognised
 *	
 *	Reads the new data from the socket and tells all others about it
 *	
 *	@author Peter Grasch
 */
void JuliusControl::recognised()
{
	QByteArray word = socket->readAll();
	emit wordRecognised(word);
}

/**
 *	@brief Disconnects the socket from julius
 *	
 *	@author Peter Grasch
 */
void JuliusControl::disconnect()
{
	this->socket->abort();
	this->socket->disconnectFromHost();
}

/**
 *	@brief A Connection has been dropped
 *	
 *	Emits the disconnected() signal
 *	
 *	@author Peter Grasch
 */
void JuliusControl::connectionLost()
{
	emit disconnected();
}

/**
 *	@brief A Connection has been established
 *	
 *	Emits the connected() signal
 *	
 *	@author Peter Grasch
 */
void JuliusControl::connectedTo()
{
	emit connected();
}

/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
JuliusControl::~JuliusControl()
{
}


