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
#include <QSslSocket>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QCryptographicHash>
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
	socket = new QSslSocket();
	timeoutWatcher = new QTimer(this);
	connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
			
// 	connect(socket, SIGNAL(encrypted()), this, SLOT(connectedTo()));
	connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(messageReceived()));
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
	
	if (Settings::get("Juliusd/Encrypted").toBool())
	{
		socket->setCiphers(Settings::getS("Juliusd/Cipher"));
		socket->setPrivateKey(Settings::getS("Juliusd/Cert"), QSsl::Rsa, QSsl::Pem);
		socket->connectToHostEncrypted( server, port );
	} else 
		socket->connectToHost( server, port );
	timeoutWatcher->start(Settings::get("Network/Timeout").toInt());
	
}

void JuliusControl::errorOccured()
{
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	emit connectionError(socket->errorString());
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
 *	@brief Process the request
 *	
 *	Reads the new data from the socket and processes it
 *	
 *	@author Peter Grasch
 */
void JuliusControl::messageReceived()
{
	QByteArray msgByte = socket->readAll();
	
	QDataStream msg(&msgByte, QIODevice::ReadOnly);

	qint32 type;
	msg >> type;
	switch (type)
	{
		case 1:
			/* login accepted */
			emit loggedIn();
			break;
		case 2:
			/* user / pass rejected */
			emit error(tr("Benutzername oder Passwort falsch."));
			this->disconnectFromServer();
			break;
		
		case 3:
			/* no pass specified */
			emit error(tr("Kein Passwort angegeben. Aus Sicherheitsgründen muss ein Passwort festgelegt werden"));
			break;
		
		case 4:
			/* login rejected, because the version is known to NOT be supported */
			emit error(tr("Version nicht unterstützt"));
			break;
		case 5:
			/* login accepted, but the version is not known to be supported */
			QString reason=tr("Version möglicherweise nicht Unterstützt");
			if (Settings::get("Juliusd/ContiniueOnWarning").toBool())
			{
				emit warning(reason);
				emit loggedIn();
			} else emit error(reason,true);
			break;
		
// 		case 1:
// 			/* login accepted */
// 			emit loggedIn();
// 			break;
// 		case 1:
// 			/* login accepted */
// 			emit loggedIn();
// 			break;
	}
}

/**
 *	@brief A word has been recognised
 *	
 *	emits the signal wordRecognised
 *	
 *	@author Peter Grasch
 */
void JuliusControl::recognised(QString sequence)
{
	emit wordRecognised(sequence);
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

	login();
}

void JuliusControl::login()
{
	QString user = Settings::getS("Juliusd/Username");
	QString pass = Settings::getS("Juliusd/Password");

	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);

	out << qint32(0 /*login*/) << user << pass << protocolVersion;

	out.device()->seek(0);

	socket->write(toWrite);
}
/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
JuliusControl::~JuliusControl()
{
}


