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

/**
 *	@brief Constructor
 *	
 *	Creates the Server and sets it up to listen on the supplied port
 *	Default: 4444
 *	
 *	@author Peter Grasch
 *	@param qint16 port
 *	Port the Server should listen to
 *	@todo Restrict the allowed connection to certain hosts
 */
JuliusControl::JuliusControl(quint16 port)
{
	this->server = new QTcpServer();
	server->listen( QHostAddress::Any, port);
	connect (server, SIGNAL(newConnection()), this, SLOT(getConnection()));
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
 *	@brief A connection has been initiated
 *	
 *	This function should be called when the QTcpServer recieves a new connection;
 *	It extracts a pointer to the connecting socket and stores it in the socket (member-)
 *	variable.
 *	It also sets up the signals/slots to react on recieving data
 *	
 *	@author Peter Grasch
 *	@todo If two clients connect simultaniosly the first connection is simply dropped
 */
void JuliusControl::getConnection()
{
	emit connected();
	socket = server->nextPendingConnection();
	QObject::connect (socket, SIGNAL(readyRead()), this, SLOT(recognised()));
	QObject::connect (socket, SIGNAL(disconnected()), this, SLOT(connectionLost()));
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
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
JuliusControl::~JuliusControl()
{
}


