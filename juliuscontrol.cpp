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
#include <QDebug>
#include <QTimer>
#include "settings.h"
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
	timeoutWatcher = new QTimer(this);
	connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
			
	connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(recognised()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()));
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
	if (socket->state() != QAbstractSocket::UnconnectedState) {
		 socket->abort();
	}
	
	socket->connectToHost( server, port );
	timeoutWatcher->start(Settings::get("Network/Timeout").toInt());
	
}

void JuliusControl::errorOccured()
{
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	emit error(socket->errorString());
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


void JuliusControl::timeoutReached()
{
	timeoutWatcher->stop();
	emit error(tr("Zeitüberschreitung der Anforderung (%1 ms)").arg(Settings::get("Network/Timeout").toInt()));
	socket->abort();
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
void JuliusControl::disconnectFromServer()
{
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	this->socket->disconnectFromHost();
	if ((socket->state() != QAbstractSocket::UnconnectedState) &&
		     (!socket->waitForDisconnected(1000)))
		this->socket->abort();
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
	QObject::disconnect(socket, SIGNAL(disconnected()), this, SLOT(connectionLost()));
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
	timeoutWatcher->stop();
	emit connected();
	connect(socket, SIGNAL(disconnected()), this, SLOT(connectionLost()));
}

/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
JuliusControl::~JuliusControl()
{
}


