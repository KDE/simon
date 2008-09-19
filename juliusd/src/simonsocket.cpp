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
#include "simonsocket.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QCryptographicHash>
#include <QSslKey>
#include "settings.h"
#include "juliuscontrol.h"
#include "databaseaccess.h"

#include <KLocalizedString>
#include <QDebug>

SimonSocket::SimonSocket(int socketDescriptor, DatabaseAccess *db)
 : QSslSocket()
{
	if (Settings::getB("Encryption"))
	{
		setProtocol(QSsl::TlsV1);
		setCiphers(Settings::getS("Cipher"));

		QString certPath = Settings::getS("Cert");
		if (!certPath.isEmpty())
		{
			QFile cert(certPath);
			if (cert.open(QIODevice::ReadOnly))
			{
				setLocalCertificate(QSslCertificate(&cert, QSsl::Pem));
				setPrivateKey(Settings::getS("Cert"), QSsl::Rsa, QSsl::Pem);
			} else sendError(i18n("Konnte Zertifikat nicht laden (%1)", Settings::getS("Cert")));
		}
	}

	this->setSocketDescriptor(socketDescriptor);
	
	julius = new JuliusControl();
	connect ( julius, SIGNAL(wordRecognised(QString,QString,QString)), this, 
			SLOT(sendWord(QString,QString,QString)));
	connect (julius, SIGNAL(errorOccured(QString)), this, SLOT(sendError(QString)));
	connect(julius, SIGNAL(juliusStopped(int)), this, SLOT(juliusStopped(int)));

	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
	connect(this, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(socketError()));

	if (Settings::getB("Encryption"))
		startServerEncryption();

	this->db = db;

	connect(this, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

void SimonSocket::juliusStopped(int exitCode)
{
	qDebug() << "Julius tot! Exitcode: " << exitCode;
}

void SimonSocket::sendWord(QString word, QString sampa, QString samparaw)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << 20000 /* word recognized */ << word << sampa << samparaw;
	write(toWrite);
	emit wordSent(word, socketDescriptor());
}

void SimonSocket::socketError()
{
	QString error= errorString()+"\n";
	QList<QSslError> sslErrors;
	sslErrors = this->sslErrors();
	for (int i=0; i < sslErrors.count(); i++)
		error+=sslErrors[i].errorString()+"\n";
// 	qDebug() << error;

// 	ignoreSslErrors();
// 	Logger::log(i18n("[ERR] %1", error));
	sendError(error);
}

void SimonSocket::sendInfo(QString infoStr)
{
	emit info(infoStr, socketDescriptor());
}

void SimonSocket::sendError(QString errorStr)
{
	emit errorOccured(errorStr, socketDescriptor());
}


void SimonSocket::readMessage()
{
	
	QByteArray msg = readAll();
	QDataStream stream(&msg, QIODevice::ReadOnly);
	qint32 type;

	stream >> type;
	
	switch (type)
	{
		case 0:
		{
			/* login requested */
			QString user;
			QString pass;
			qint8 remoteProtocolVersion;
				
			stream >> user;
			stream >> pass;
			stream >> remoteProtocolVersion;
				
			if (remoteProtocolVersion <  protocolVersion)
			{
				sendAnswer(4); /* version rejected */
				return;
			}
			if (pass.isEmpty())
			{
				sendAnswer(3); /* pass required (none specified) */
				return;
			}
				
				
				//version is _probably_ ok; pass is specified
	
			sendInfo(i18n("Verbindung von User \"%1\" mit Protokollversion \"%2\" (Serverversion: %4) und Passwort \"%3\"", user, remoteProtocolVersion, pass, protocolVersion));
			if (pass==db->getPassword(user))
			{
				if (remoteProtocolVersion >  protocolVersion)
				{
					sendAnswer(5); /* give a warning that the version _may_ be incompatible */
				} else 
					sendAnswer(1); // everything is perfect :)
					return;
			} else
			{
				sendError(i18n("Benutzername / Passwort falsch"));
				sendAnswer(2);
			}
		}
			break;
		case 100:
		{
			/* recognition requested */
			sendInfo(i18n("Starte Julius..."));
			julius->startJulius(Settings::getS("Command"), Settings::getS("Arguments"));
		}
			break;
			
		case 9990:
		{
			/* logout */
			julius->stopJulius();
			sendAnswer(9991 /*everything went well*/);
			this->close();
		}
			break;
		//TextSync
		case 99990:
		{
			emit sendId(this->socketDescriptor(), msg);
		}
			break;
		//_______
	}
}

void SimonSocket::sendAnswer(qint32 request)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << request;write(toWrite);
}



SimonSocket::~SimonSocket()
{
	if (julius) julius->deleteLater();
}


