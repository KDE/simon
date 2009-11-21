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

#include "clientsocket.h"

#include "databaseaccess.h"
#include <sscobjects/sscobject.h>
#include <sscobjects/user.h>
#include <sscobjects/institution.h>
#include <sscobjects/language.h>

#ifdef bzero
#undef bzero
#endif

#include <QDir>
#include <QDateTime>
#include <QHostAddress>
#include <QMutexLocker>
#include <QMap>
#include <QDebug>
#include <QList>

#define parseLengthHeader() 		waitForMessage(sizeof(qint64),stream, msg); \
			qint64 length; \
			stream >> length; \
			waitForMessage(length,stream,msg); 



ClientSocket::ClientSocket(int socketDescriptor, DatabaseAccess* _databaseAccess, QObject *parent) : QSslSocket(parent),
	databaseAccess(_databaseAccess)
{

	qDebug() << "Created ClientSocket with Descriptor " << socketDescriptor;

	this->setSocketDescriptor(socketDescriptor);
	connect(this, SIGNAL(readyRead()), this, SLOT(processRequest()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError()));

	//TODO: Implement encryption
	if (false) //Settings::getB("Encryption"))
	{
		connect(this, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSocketError()));
		startServerEncryption();
	}
}

void ClientSocket::waitForMessage(qint64 length, QDataStream& stream, QByteArray& message)
{
	Q_ASSERT(stream.device());
	while (stream.device()->bytesAvailable() < length)
	{
		if (waitForReadyRead())
			message += readAll();
	}
}

void ClientSocket::sendObject(SSC::Request code, SSCObject* object)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);

	QByteArray body = object->serialize();

	stream << (qint32) code << (qint64) body.count();

	write(toWrite);
	write(body);
}

void ClientSocket::sendObjects(SSC::Request code, QList<SSCObject*> objects)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);

	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	bodyStream << (qint32) (objects.count());

	foreach (SSCObject* o, objects) {
		bodyStream << o->serialize();
	}

	stream << (qint32) code << (qint64) body.count();

	write(toWrite);
	write(body);
}

void ClientSocket::sendUser(qint32 id)
{
	User *u = databaseAccess->getUser(id);
	if (!u) {
		sendCode(SSC::UserRetrievalFailed);
		return;
	}

	sendObject(SSC::User, u);
	delete u;
}

void ClientSocket::sendUsers(User *filterUser, qint32 institutionId, const QString& referenceId)
{
	QList<User*>* users = databaseAccess->getUsers(filterUser, institutionId, referenceId);
	if (!users) {
		sendCode(SSC::UserRetrievalFailed);
		return;
	}
	QList<SSCObject*> sendMe;

	foreach (User* u, *users)
		sendMe << u;

	sendObjects(SSC::Users, sendMe);
	qDeleteAll(*users);
	delete users;
}

void ClientSocket::removeUser(qint32 id)
{
	//TODO: Delete all associated samples
	
	if (!databaseAccess->removeUser(id)) {
		sendCode(SSC::RemoveUserFailed);
		return;
	}

	sendCode(SSC::Ok);
}

void ClientSocket::removeInstitution(qint32 id)
{
	if (!databaseAccess->removeInstitution(id)) {
		sendCode(SSC::RemoveInstitutionFailed);
		return;
	}

	sendCode(SSC::Ok);
}

void ClientSocket::sendLanguages()
{
	QList<Language*>* ll = databaseAccess->getLanguages();
	if (!ll) {
		sendCode(SSC::LanguageRetrievalFailed);
		return;
	}
	QList<SSCObject*> sendMe;

	foreach (Language* l, *ll)
		sendMe << l;

	sendObjects(SSC::Languages, sendMe);
	qDeleteAll(*ll);
	delete ll;
}

void ClientSocket::sendInstitutions()
{
	QList<Institution*>* ins = databaseAccess->getInstitutions();
	if (!ins) {
		sendCode(SSC::InstitutionRetrievalFailed);
		return;
	}
	QList<SSCObject*> sendMe;

	foreach (Institution* i, *ins)
		sendMe << i;

	sendObjects(SSC::Institutions, sendMe);
	qDeleteAll(*ins);
	delete ins;
}


void ClientSocket::processRequest()
{
	QByteArray msg = readAll();
	QDataStream stream(&msg, QIODevice::ReadOnly);
	qint32 type;

	
	while (!stream.atEnd())
	{
		SSC::Request request;
		stream >> type;
		request = (SSC::Request) type;

		switch (request)
		{
			case SSC::GetUser: {
				qint32 id;
				waitForMessage(sizeof(qint32), stream, msg);
				stream >> id;
				sendUser(id);
				break;
					   }

			case SSC::GetUsers: {
				parseLengthHeader();

				QByteArray userByte;
				stream >> userByte;

				User *u = new User();
				u->deserialize(userByte);

				qint32 institutionId;
				stream >> institutionId;
				QString referenceId;
				stream >> referenceId;
				sendUsers(u, institutionId, referenceId);
				break;
					    }

			case SSC::AddUser:{
				parseLengthHeader();

				//QByteArray userByte = stream.device()->read(length);
				QByteArray userByte;
				stream >> userByte;
				User *u = new User();
				u->deserialize(userByte);

				if (databaseAccess->addUser(u))
					sendCode(SSC::Ok);
				else sendCode(SSC::AddUserFailed);

				break;
					  }

			case SSC::ModifyUser: {
				parseLengthHeader();
				//QByteArray userByte = stream.device()->read(length);
				QByteArray userByte;
				stream >> userByte;
				User *u = new User();
				u->deserialize(userByte);
				if (databaseAccess->modifyUser(u)) {
					sendCode(SSC::Ok);
				}else {
					sendCode(SSC::UserRetrievalFailed);
				}
				break;
					      }
			case SSC::RemoveUser: {
				qint32 id;
				waitForMessage(sizeof(qint32), stream, msg);
				stream >> id;
				removeUser(id);
				break;
					   }

			case SSC::GetLanguages:
				sendLanguages();
				break;

			case SSC::GetInstitutions:
				sendInstitutions();
				break;

			case SSC::AddInstitution:{
				parseLengthHeader();

				QByteArray institutionByte = stream.device()->read(length);
				Institution *i = new Institution();
				i->deserialize(institutionByte);

				if (databaseAccess->addInstitution(i))
					sendCode(SSC::Ok);
				else sendCode(SSC::AddInstitutionFailed);

				break;
					  }

			case SSC::ModifyInstitution: {
				parseLengthHeader();
				QByteArray institutionByte = stream.device()->read(length);
				Institution *i = new Institution();
				i->deserialize(institutionByte);
				if (databaseAccess->modifyInstitution(i)) {
					sendCode(SSC::Ok);
				}else {
					sendCode(SSC::InstitutionRetrievalFailed);
				}
				break;
					      }
			case SSC::RemoveInstitution: {
				qint32 id;
				waitForMessage(sizeof(qint32), stream, msg);
				stream >> id;
				removeInstitution(id);
				break;
					   }

							
		}
		
		if (bytesAvailable())
			msg += readAll();
	}
}


void ClientSocket::sendCode(SSC::Request code)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) code;
	write(toWrite);
}

void ClientSocket::slotSocketError()
{
	QString error= errorString()+"\n";
	QList<QSslError> sslErrors;
	sslErrors = this->sslErrors();
	for (int i=0; i < sslErrors.count(); i++)
		error+=sslErrors[i].errorString()+"\n";
	qDebug() << error;

// 	ignoreSslErrors();
}


ClientSocket::~ClientSocket()
{
	qDebug() << "Deleting client";
}
