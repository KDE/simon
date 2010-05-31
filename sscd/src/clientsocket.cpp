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
#include "basedirectory.h"

#include <sscobjects/sscobject.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>
#include <sscobjects/sample.h>
#include <sscobjects/user.h>
#include <sscobjects/institution.h>
#include <sscobjects/userininstitution.h>
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

void ClientSocket::sendObject(qint32 code, SSCObject* object)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);

	QByteArray body = object->serialize();

	stream << (qint32) code << (qint64) body.count();

	write(toWrite);
	write(body);
}

void ClientSocket::sendObjects(qint32 code, QList<SSCObject*> objects)
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

/**
 * Sends a request identified by the request id and with the given message 
 * (usually an id)
 */
bool ClientSocket::sendResponse(qint32 request, qint32 message)
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << request << message;
	if (write(toWrite) == -1) {
		return false;
	}

	return true;
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
	QStringList *files = databaseAccess->getSamplePaths(id);

	if (!files || !databaseAccess->removeUser(id)) {
		sendCode(SSC::RemoveUserFailed);
		return;
	}

	foreach (const QString& file, *files) {
		qDebug() << "Removing: " << file;
		QFile::remove(file);
	}

	QDir d;
	d.rmdir(samplePath(id));
	delete files;

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

void ClientSocket::removeUserInInstitution(qint32 userId, qint32 institutionId)
{
	if (!databaseAccess->deleteUserInstitutionAssociation(userId, institutionId)) {
		sendCode(SSC::UserRetrievalFailed);
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

void ClientSocket::sendMicrophones()
{
	QList<Microphone*>* ml = databaseAccess->getMicrophones();
	if (!ml) {
		sendCode(SSC::MicrophoneRetrievalFailed);
		return;
	}
	QList<SSCObject*> sendMe;

	foreach (Microphone* m, *ml)
		sendMe << m;

	sendObjects(SSC::Microphones, sendMe);
	qDeleteAll(*ml);
	delete ml;
}

void ClientSocket::sendSoundCards()
{
	QList<SoundCard*>* sl = databaseAccess->getSoundCards();
	if (!sl) {
		sendCode(SSC::SoundCardRetrievalFailed);
		return;
	}
	QList<SSCObject*> sendMe;

	foreach (SoundCard* s, *sl)
		sendMe << s;

	sendObjects(SSC::SoundCards, sendMe);
	qDeleteAll(*sl);
	delete sl;
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

void ClientSocket::sendUserInstitutionAssociations(qint32 userId)
{
	QList<UserInInstitution*>* uiis = databaseAccess->getUserInstitutionAssociation(userId);
	if (!uiis) {
		sendCode(SSC::UserRetrievalFailed);
		return;
	}
	QList<SSCObject*> sendMe;

	foreach (UserInInstitution* uii, *uiis)
		sendMe << uii;

	sendObjects(SSC::UserInstitutionAssociations, sendMe);
	qDeleteAll(*uiis);
	delete uiis;
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
				delete u;
				break;
					    }

			case SSC::AddUser:{
				parseLengthHeader();

				QByteArray userByte;
				stream >> userByte;
				User *u = new User();
				u->deserialize(userByte);

				qint32 userId;
				if (databaseAccess->addUser(u, userId))
					sendResponse(SSC::UserAdded, userId);
				else sendCode(SSC::AddUserFailed);

				delete u;
				break;
					  }

			case SSC::ModifyUser: {
				parseLengthHeader();
				QByteArray userByte;
				stream >> userByte;
				User *u = new User();
				u->deserialize(userByte);
				if (databaseAccess->modifyUser(u)) {
					sendCode(SSC::Ok);
				}else {
					sendCode(SSC::UserRetrievalFailed);
				}
				delete u;
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

			case SSC::GetMicrophones:
				sendMicrophones();
				break;

			case SSC::GetSoundCards:
				sendSoundCards();
				break;

			case SSC::GetOrCreateMicrophone:
			{
				parseLengthHeader();
				QByteArray micByte;
				stream >> micByte;
				Microphone *m = new Microphone();
				m->deserialize(micByte);

				qint16 microphoneId;
				if (databaseAccess->getOrCreateMicrophone(m, microphoneId))
				{
					qDebug() << "Mic id: " << microphoneId;
					sendResponse(SSC::GotMicrophone, microphoneId);
				}
				else sendCode(SSC::MicrophoneRetrievalFailed);
				delete m;
				break;
			}
			case SSC::GetOrCreateSoundCard:
			{
				parseLengthHeader();
				QByteArray soundCardByte;
				stream >> soundCardByte;
				SoundCard *s = new SoundCard();
				s->deserialize(soundCardByte);

				qint16 soundCardId;
				if (databaseAccess->getOrCreateSoundCard(s, soundCardId))
					sendResponse(SSC::GotSoundCard, soundCardId);
				else sendCode(SSC::MicrophoneRetrievalFailed);
				delete s;
				break;
			}


			case SSC::GetInstitutions:
				sendInstitutions();
				break;

			case SSC::AddInstitution:{
				parseLengthHeader();

				//QByteArray institutionByte = stream.device()->read(length);
				QByteArray institutionByte;
				stream >> institutionByte;
				Institution *i = new Institution();
				i->deserialize(institutionByte);

				if (databaseAccess->addInstitution(i))
					sendCode(SSC::Ok);
				else sendCode(SSC::AddInstitutionFailed);

				delete i;
				break;
					  }

			case SSC::ModifyInstitution: {
				parseLengthHeader();
				QByteArray institutionByte;
				stream >> institutionByte;
				Institution *i = new Institution();
				i->deserialize(institutionByte);
				if (databaseAccess->modifyInstitution(i)) {
					sendCode(SSC::Ok);
				}else {
					sendCode(SSC::InstitutionRetrievalFailed);
				}
				delete i;
				break;
					      }
			case SSC::RemoveInstitution: {
				qint32 id;
				waitForMessage(sizeof(qint32), stream, msg);
				stream >> id;
				removeInstitution(id);
				break;
					   }

			case SSC::GetUserInstitutionAssociations: {
				qint32 id;
				waitForMessage(sizeof(qint32), stream, msg);
				stream >> id;
				sendUserInstitutionAssociations(id);
				break;
								  }

			case SSC::AddUserInstitutionAssociation:{
				parseLengthHeader();

				QByteArray uiiByte;
				stream >> uiiByte;
				UserInInstitution *uii = new UserInInstitution();
				uii->deserialize(uiiByte);

				int ret = databaseAccess->addUserInstitutionAssociation(uii);
				switch (ret) {
					case 0:
						sendCode(SSC::Ok);
					case -1:
						sendCode(SSC::UserRetrievalFailed);
					case -2:
						sendCode(SSC::InstitutionRetrievalFailed);
				}
				delete uii;
				break;
					  }

			case SSC::RemoveUserInstitutionAssociation: {
				qint32 userId, institutionId;
				waitForMessage(2*sizeof(qint32), stream, msg);
				stream >> userId;
				stream >> institutionId;
				removeUserInInstitution(userId, institutionId);
				break;
					   }

			case SSC::Sample: {
				parseLengthHeader();

				QByteArray sampleByte;
				stream >> sampleByte;
				Sample *s = new Sample();
				s->deserialize(sampleByte);

				storeSample(s);

        /////////////////////
        //DEBUG: Simulate bug
        //static bool fail = false;
        //if (fail)
        //  delete this;
        //fail = true;
        ///////////////////////
        
				delete s;
					   }

							
		}
		
		if (bytesAvailable())
			msg += readAll();
	}
}

void ClientSocket::storeSample(Sample *s)
{
	qDebug() << "Storing sample...";
	qint32 sampleId;
	databaseAccess->lockTranscation();
	sampleId = databaseAccess->nextSampleId();
	if (sampleId <= 0) {
		qDebug() << "Sample id failed";
		sendCode(SSC::SampleStorageFailed);
		return;
	}

	s->setId(sampleId);
	QString fileName = samplePath(s->userId()) + QDir::separator() + QString::number(sampleId)+".wav";
	qDebug() << "File name: " << fileName;
	s->setPath(fileName);
	if (!databaseAccess->storeSample(s))
		sendCode(SSC::SampleStorageFailed);
	else {
		//store sample...
		QFile f(fileName);
		qDebug() << "Storing sample: " << fileName;
		if ((!f.open(QIODevice::WriteOnly)) ||
				(f.write(s->data())==-1))
			sendCode(SSC::SampleStorageFailed);
		else
		{
			qDebug() << "Sample stored ok";
			sendCode(SSC::Ok);
		}
	}
	databaseAccess->unlockTransaction();
}

QString ClientSocket::samplePath(qint32 userId)
{
	QString baseDir = SSCD_BASE_DIRECTORY;
	QString samplesDir = baseDir+QDir::separator()+"samples"+QDir::separator()+QString::number(userId);
	QDir d(samplesDir);
	if (!d.exists() && !d.mkpath(samplesDir)) {
		qWarning() << "Couldn't create sample storage directory: " << samplesDir;
		exit(1);
	}
	return samplesDir;
}

void ClientSocket::sendCode(qint32 code)
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
