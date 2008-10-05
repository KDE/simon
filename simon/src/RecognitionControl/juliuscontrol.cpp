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


#include "juliuscontrol.h"
#include "simoninfo/simoninfo.h"
#include "coreconfiguration.h"

#include <QByteArray>
#include <QSslSocket>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QStringList>
#include <KMessageBox>
#include <KLocalizedString>
#include <KPasswordDialog>

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
JuliusControl::JuliusControl(QWidget *parent) : QObject(parent)
{
	socket = new QSslSocket();
	timeoutWatcher = new QTimer(this);
	connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
			
	connect(socket, SIGNAL(readyRead()), this, SLOT(messageReceived()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()));
	connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured()));
	
	connect(this, SIGNAL(error(QString, bool)), this, SLOT(disconnectFromServer()));
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
	
	disconnect(socket, SIGNAL(encrypted()), 0, 0);
	disconnect(socket, SIGNAL(connected()), 0, 0);

	if (CoreConfiguration::juliusdEncrypted())
	{
		socket->setProtocol(QSsl::TlsV1);
		connect(socket, SIGNAL(encrypted()), this, SLOT(connectedTo()));
		socket->connectToHostEncrypted( server, port );
	} else {
		connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
		socket->connectToHost( server, port );
	}
	timeoutWatcher->start(CoreConfiguration::juliusdConnectionTimeout());
	
}

void JuliusControl::errorOccured()
{
	if (socket->state()== QAbstractSocket::UnconnectedState) return;
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	QList<QSslError> errors = socket->sslErrors();
	if ((errors.count() == 1) && (errors[0].error() == QSslError::SelfSignedCertificate) && (CoreConfiguration::juliusdEncrypted()))
	{
		if (KMessageBox::questionYesNoCancel(0, i18n("Das Zertifikat der Gegenstelle ist selbst-signiert und nicht vertrauenswürdig.\n\nWollen Sie die Verbindung trozdem fortsetzen?"), i18n("Selbst-Signiertes Zertifikat"))==KMessageBox::Yes)
		{
			socket->ignoreSslErrors();
			return;
		} else {
			socket->abort();
			socket->flush();
			socket->close();
		}
	}
	if (socket->error() != QAbstractSocket::UnknownSocketError)
		emit connectionError(socket->errorString());
	else 
	{ //build ssl error list
		QString sslErrorStr;
		for (int i=0; i < errors.count(); i++)
			sslErrorStr += errors[i].errorString()+"\n";
		emit connectionError(sslErrorStr);
	}
}

/**
 * \brief Returns whether the socket is connected to a valid juliusd server
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
	emit connectionError(i18n("Zeitüberschreitung der Anforderung (%1 ms)", CoreConfiguration::juliusdConnectionTimeout()));
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
		{
			/* login accepted */
			emit loggedIn();

			//start recognition
			sendRequest (100 /*start recognition*/);
		}
			break;
		case 2:
		{
			/* user / pass rejected */
			emit error(i18n("Benutzername oder Passwort falsch."));
			this->disconnectFromServer();
		}
			break;
		
		case 3:
		{
			/* no pass specified */
			emit error(i18n("Kein Passwort angegeben. Aus Sicherheitsgründen muss ein Passwort festgelegt werden"));
		}
			break;
		
		case 4:
		{
			/* login rejected, because the version is known to NOT be supported */
			emit error(i18n("Version nicht unterstützt"));
		}
			break;
		case 5:
		{
			/* login accepted, but the version is not known to be supported */
			QString reason=i18n("Version mglw. nicht unterstützt");
			if (CoreConfiguration::juliusdContinueOnWarning())
			{
				emit warning(reason);
				sendRequest (100 /*start recognition*/);
				emit loggedIn();
			} else 
				emit error(reason,true);
		}
			break;


		case 20000:
		{
			/* word recognized */
			QString word, sampa, samparaw;
			msg >> word;
			msg >> sampa;
			msg >> samparaw;
			emit recognised(word, sampa, samparaw);
		}
			break;
		//TextSync the data of the syncprocess is here avaiable
		case 99990:
		{
			QString timestamp;
			QString text;
			QString filename;
			int count;

			msg >> filename;

			msg >> count;
			
			QFile *datafile = new QFile(filename);
			QFile *stampfile = new QFile("shadow"+filename);
			
			if(!datafile->open(QIODevice::WriteOnly))
			{
				KMessageBox::error(NULL,i18n("Fehler beim lesen der Daten-Datei"));
			}
			if(!stampfile->open(QIODevice::WriteOnly))
			{
				KMessageBox::error(NULL,i18n("Fehler beim lesen der Shadow-Datei"));
			}

			
			QByteArray data, stamp;
			
			for (int i = 0; i<count; ++i)
			{
				msg >> text >> timestamp;
				data.append(text.toUtf8());
				stamp.append(timestamp.toUtf8());
				stamp.append('\n');
			}
			datafile->flush();
			datafile->write(data);
			datafile->close();
			stampfile->flush();
			stampfile->write(stamp);
			stampfile->close();
			
		}
			break;
		//_______
	}
}

/**
 * \brief Sends a simple request identified by the request id
 * \author Peter Grasch
 * @param request The request to send
 */
void JuliusControl::sendRequest(qint32 request)
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << request;
	socket->write(toWrite);
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
	QString user = CoreConfiguration::juliusdUsername();
	QString pass = CoreConfiguration::juliusdPassword();

	if (user.isEmpty())
	{
		KPasswordDialog dlg( dynamic_cast<QWidget*>(parent()), 
					KPasswordDialog::ShowUsernameLine|KPasswordDialog::ShowKeepPassword );
		dlg.setPrompt( i18n( "Bitte geben Sie die Benutzerdaten für die Verbindung zum Juliusd an." ));
		if( !dlg.exec() || dlg.username().isEmpty() )
		{
			disconnectFromServer(); //the user canceled
			return;
		}
		user = dlg.username();
		pass = dlg.password();

		if (dlg.keepPassword())
		{
			CoreConfiguration::setJuliusdUsername(user);
			CoreConfiguration::setJuliusdPassword(pass);
		}
	}

	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);

	out << qint32(0 /*login*/) << user << pass << protocolVersion;

	out.device()->seek(0);

	socket->write(toWrite);
}


//TextSync / just a testdata is send to the mergeprocess
void JuliusControl::sendSyncFile(QString filename)
{
	
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);

	out << qint32(99990);
	
	
// 	QFile *file = new QFile(filename);
	
	out << filename;
	
	QFile *datafile = new QFile(filename);
	QFile *stampfile = new QFile("shadow"+filename);
	
	if(!datafile->open(QIODevice::ReadOnly))
	{
		KMessageBox::error(NULL,i18n("Fehler beim lesen der Daten-Datei"));
	}
	if(!stampfile->open(QIODevice::ReadOnly))
	{
		KMessageBox::error(NULL,i18n("Fehler beim lesen der Shadow-Datei"));
	}

	QByteArray datareader, stampreader;

	int i = 0;

	while(!datafile->atEnd())
	{
		datareader = datafile->readLine();
		stampreader = stampfile->readLine();
		stampreader = stampreader.remove(23,2);
		
		QString data(datareader);
		QString stamp(stampreader);
		out  << data << stamp;
		++i;
	}
	
	//marks the end of the file
	out << QString("[file_end]") << QString("0000.00.00-00:00:00:000");
	
	out.device()->seek(0);

	socket->write(block);
}
//_____



/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
JuliusControl::~JuliusControl()
{
    socket->deleteLater();
    timeoutWatcher->deleteLater();
}


