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


#include "sscdaccess.h"
#include "sscconfig.h"

#include <simonprogresstracking/operation.h>
#include <sscprotocol/sscprotocol.h>
#include <sscobjects/institution.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>
#include <sscobjects/userininstitution.h>
#include <sscobjects/user.h>
#include <sscobjects/sample.h>


#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <QByteArray>
#include <QSslSocket>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QStringList>

#include <KMessageBox>
#include <KLocalizedString>
#include <KDebug>

SSCDAccess* SSCDAccess::instance;

#define parseLengthHeader() 		waitForMessage(sizeof(qint64),stream, msg); \
			qint64 length; \
			stream >> length; \
			waitForMessage(length,stream,msg); 

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
SSCDAccess::SSCDAccess(QWidget* parent) : QObject(parent),
	socket(new QSslSocket()),
	timeoutWatcher(new QTimer(this))
{
	connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
			
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()), Qt::DirectConnection);
	connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured()), Qt::DirectConnection);

	connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()), Qt::DirectConnection);
}


void SSCDAccess::connectTo(QString server, quint16 port, bool encrypted)
{
	if (socket->state() != QAbstractSocket::UnconnectedState)
		 socket->abort();
	
	disconnect(socket, SIGNAL(encrypted()), 0, 0);
	disconnect(socket, SIGNAL(connected()), 0, 0);

	if (encrypted) {
		socket->setProtocol(QSsl::TlsV1);
		connect(socket, SIGNAL(encrypted()), this, SLOT(connectedTo()));
		socket->connectToHostEncrypted( server, port );
	} else {
		connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
		socket->connectToHost( server, port );
	}
	timeoutWatcher->start(SSCConfig::timeout());
	
}

void SSCDAccess::errorOccured()
{
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();

	QList<QSslError> errors = socket->sslErrors();
	if ((errors.count() == 1) && (errors[0].error() == QSslError::SelfSignedCertificate))
	{
		if (KMessageBox::questionYesNoCancel(0, i18n("The certificate of the remote host is self-signed and thus not trustworthy. "
"\n\nDo you still want to continue?"), i18n("Self-Signed Certificate"))==KMessageBox::Yes)
		{
			socket->ignoreSslErrors();
			return;
		} else {
			socket->abort();
			socket->flush();
			socket->close();
		}
	}
	if (socket->error() != QAbstractSocket::UnknownSocketError) {
    kDebug() << "Populating unknown error: " << socket->errorString();
		emit error(socket->errorString());
	} else {
		//build ssl error list
		QStringList serverConnectionErrors;
		for (int i=0; i < errors.count(); i++)
			serverConnectionErrors << errors[i].errorString();
		emit error(serverConnectionErrors.join("\n"));
	}
	//emit disconnected();
}

/*
 * Returns the last error message; This only includes
 * errors during normal operation (e.g. Wrong user id) but
 * not system errors (e.g. connection refused); The latter
 * are emitted using the error() signal
 */
QString SSCDAccess::lastError()
{
	return lastErrorString;
}

/**
 * \brief Returns whether the socket is connected to a valid juliusd server
 * 
 * \author Peter Grasch
 * \return bool
 * True = is connected
 */
bool SSCDAccess::isConnected()
{
	if (!socket) return false;
	
	return (socket->state() == QAbstractSocket::ConnectedState);
}


void SSCDAccess::timeoutReached()
{
	emit error(i18n("Request timed out (%1 ms) or connection was reset.\n\nPlease check your network connection and try again.", SSCConfig::timeout()));
  abort();
}

void SSCDAccess::abort()
{
	timeoutWatcher->stop();
	socket->abort();
  socket->close();
}

/**
 *	@brief Disconnects the socket from julius
 *	
 *	@author Peter Grasch
 */
void SSCDAccess::disconnectFromServer()
{
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	if ((socket->state() == QAbstractSocket::UnconnectedState) ||
		(socket->state() == QAbstractSocket::ConnectingState) || 
		(socket->state() == QAbstractSocket::HostLookupState))
	{
		socket->abort();
		emit disconnected();
		return;
	}

	this->socket->disconnectFromHost();
	if ((socket->state() != QAbstractSocket::UnconnectedState) &&
		     (!socket->waitForDisconnected(500)))
		this->socket->abort();
}

/**
 * A Connection has been established
 * Emits the connected() signal
 */
void SSCDAccess::connectedTo()
{
	timeoutWatcher->stop();

	emit connected();
}

/**
 * Sends a request identified by the request id
 */
bool SSCDAccess::sendRequest(qint32 request)
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << request;
	if (socket->write(toWrite) == -1) {
		lastErrorString = i18n("Couldn't write request");
		return false;
	}

	return true;
}

/**
 * Sends a request identified by the request id and with the given message 
 * (usually an id)
 */
bool SSCDAccess::sendRequest (qint32 request, qint32 message)
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << request << message;
	if (socket->write(toWrite) == -1) {
		lastErrorString = i18n("Couldn't write request");
		return false;
	}

	return true;
}

/**
 * Sends a request identified by the request id and with the given messages
 * (usually ids)
 */
bool SSCDAccess::sendRequest (qint32 request, qint32 message, qint32 message2)
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << request << message << message2;
	if (socket->write(toWrite) == -1) {
		lastErrorString = i18n("Couldn't write request");
		return false;
	}

	return true;
}



bool SSCDAccess::waitForMessage(qint64 length, QDataStream& stream, QByteArray& message)
{
	Q_ASSERT(stream.device());
	while (stream.device()->bytesAvailable() < length)
	{
		if (socket->waitForReadyRead(SSCConfig::timeout() /*timeout*/))
		{
			message += socket->readAll();
		}
		else
		{
			//timeout reached
			kDebug() << "Timeout reached!";
			abort();
			return false;
		}
	}
	return true;
}

void SSCDAccess::sendObject(SSC::Request code, SSCObject* object)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);

	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	bodyStream << object->serialize();

	stream << (qint32) code << (qint64) body.count();

    kDebug() << "Bytes still to write: " << socket->bytesToWrite();
	socket->write(toWrite);
    kDebug() << "Bytes still to write: " << socket->bytesToWrite();
	socket->write(body);
    kDebug() << "Bytes still to write: " << socket->bytesToWrite();
}

/*
 * Asks the server for the user identified by the given id;
 * Return value is NULL if an error occured
 */
User* SSCDAccess::getUser(qint32 id)
{
	if (!sendRequest(SSC::GetUser,id))
		return NULL;


	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);

	waitForMessage(sizeof(qint32),stream, msg);

	qint32 type;
	stream >> type;

	switch (type) {
		case SSC::User: {
			parseLengthHeader();
			QByteArray userByte = stream.device()->read(length);

			User *u = new User();
			u->deserialize(userByte);
			return u;
				}

		case SSC::UserRetrievalFailed: {
			lastErrorString = i18n("User ID not found");
			return NULL;
			break;
					       }
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return NULL;
}

QList<User*> SSCDAccess::getUsers(User *filterUser, qint32 institutionId, const QString& referenceId, bool *ok)
{
	{ //send User and institution data
		QByteArray toWrite;
		QDataStream stream(&toWrite, QIODevice::WriteOnly);

		QByteArray body;
		QDataStream bodyStream(&body, QIODevice::WriteOnly);
		bodyStream << filterUser->serialize() << institutionId << referenceId;

		stream << (qint32) SSC::GetUsers << (qint64) body.count();

		socket->write(toWrite);
		socket->write(body);
	}


	QList<User*> users;

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::Users: {
			parseLengthHeader();

			qint32 elementCount;
			stream >> elementCount;
			for (int i=0; i < elementCount; i++) {
				QByteArray userByte;
				stream >> userByte;
				User *l = new User();
				l->deserialize(userByte);
				users << l;
			}
		        *ok = true;
			break;
			      }

		case SSC::UserRetrievalFailed: {
			lastErrorString = i18n("Users could not be read");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return users;

}

qint32 SSCDAccess::addUser(User* u)
{
	sendObject(SSC::AddUser, u);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) { 
		case SSC::UserAdded: {
			waitForMessage(sizeof(qint32),stream, msg);
			qint32 userId;
			stream >> userId;
			return userId;
			      }
		case SSC::AddUserFailed: {
			lastErrorString = i18n("Couldn't add user");
			break;
					 }
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return 0;
}

bool SSCDAccess::modifyUser(User* u)
{
	sendObject(SSC::ModifyUser, u);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::Ok: {
			return true;
			      }

		case SSC::UserRetrievalFailed: {
			lastErrorString = i18n("User ID not found");
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}


QList<Microphone*> SSCDAccess::getMicrophones(bool *ok)
{
	sendRequest(SSC::GetMicrophones);

	QList<Microphone*> microphones;

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::Microphones: {
			parseLengthHeader();

			qint32 elementCount;
			stream >> elementCount;
			for (int i=0; i < elementCount; i++) {
				QByteArray microphoneByte;
				stream >> microphoneByte;
				Microphone *l = new Microphone();
				l->deserialize(microphoneByte);
				microphones << l;
			}
		        *ok = true;
			break;
			      }

		case SSC::MicrophoneRetrievalFailed: {
			lastErrorString = i18n("Microphones could not be read");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return microphones;
}

QList<SoundCard*> SSCDAccess::getSoundCards(bool *ok)
{
	sendRequest(SSC::GetSoundCards);

	QList<SoundCard*> soundCards;

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::SoundCards: {
			parseLengthHeader();

			qint32 elementCount;
			stream >> elementCount;
			for (int i=0; i < elementCount; i++) {
				QByteArray soundCardByte;
				stream >> soundCardByte;
				SoundCard *l = new SoundCard();
				l->deserialize(soundCardByte);
				soundCards << l;
			}
		        *ok = true;
			break;
			      }

		case SSC::SoundCardRetrievalFailed: {
			lastErrorString = i18n("SoundCards could not be read");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return soundCards;
}

qint32 SSCDAccess::getOrCreateMicrophone(Microphone *microphone, bool* ok)
{
	sendObject(SSC::GetOrCreateMicrophone, microphone);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	kDebug() << type;
	fprintf(stderr, "Get or create microphone returned: %d\n", type);
	switch (type) {
		case SSC::GotMicrophone: {
			waitForMessage(sizeof(qint32),stream, msg);
			qint32 id;
			stream >> id;
			kDebug() << "Received id: " << id;

			microphone->setId(id);
			*ok = true;
			return id;
			      }

		case SSC::MicrophoneRetrievalFailed: {
			lastErrorString = i18n("Microphone could not be fetched / created");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return -1;
}

qint32 SSCDAccess::getOrCreateSoundCard(SoundCard *soundCard, bool* ok)
{
	sendObject(SSC::GetOrCreateSoundCard, soundCard);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	fprintf(stderr, "Get or create soundcard returned: %d\n", type);
	switch (type) {
		case SSC::GotSoundCard: {
			waitForMessage(sizeof(qint32),stream, msg);
			qint32 id;
			stream >> id;

			soundCard->setId(id);
			*ok = true;
			return id;
			      }

		case SSC::SoundCardRetrievalFailed: {
			lastErrorString = i18n("Sound card could not be fetched / created");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return -1;
}


QList<Language*> SSCDAccess::getLanguages(bool *ok)
{
	sendRequest(SSC::GetLanguages);

	QList<Language*> languages;

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::Languages: {
			parseLengthHeader();

			qint32 elementCount;
			stream >> elementCount;
			for (int i=0; i < elementCount; i++) {
				QByteArray languageByte;
				stream >> languageByte;
				Language *l = new Language();
				l->deserialize(languageByte);
				languages << l;
			}
		        *ok = true;
			break;
			      }

		case SSC::LanguageRetrievalFailed: {
			lastErrorString = i18n("Languages could not be read");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return languages;
}


QList<Institution*> SSCDAccess::getInstitutions(bool* ok)
{
	sendRequest(SSC::GetInstitutions);

	QList<Institution*> institutions;

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::Institutions: {
			parseLengthHeader();

			qint32 elementCount;
			stream >> elementCount;
			for (int i=0; i < elementCount; i++) {
				QByteArray institutionByte;
				stream >> institutionByte;
				Institution *l = new Institution();
				l->deserialize(institutionByte);
				institutions << l;
			}
		        *ok = true;
			break;
			      }

		case SSC::InstitutionRetrievalFailed: {
			lastErrorString = i18n("Institutions could not be read");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return institutions;

}

bool SSCDAccess::addInstitution(Institution* i)
{
	sendObject(SSC::AddInstitution, i);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) { 
		case SSC::Ok: {
			return true;
			      }
		case SSC::AddInstitutionFailed: {
			lastErrorString = i18n("Couldn't add institution");
			break;
					 }
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}

bool SSCDAccess::modifyInstitution(Institution* i)
{
	sendObject(SSC::ModifyInstitution, i);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::Ok: {
			return true;
			      }

		case SSC::InstitutionRetrievalFailed: {
			lastErrorString = i18n("Institution ID not found");
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}

bool SSCDAccess::deleteInstitution(Institution* i)
{
	sendRequest(SSC::RemoveInstitution, i->id());

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) { 
		case SSC::Ok: {
			return true;
			      }
		case SSC::RemoveInstitutionFailed: {
			lastErrorString = i18n("Couldn't delete institution");
			break;
			}
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}

bool SSCDAccess::deleteUser(User* u)
{
	sendRequest(SSC::RemoveUser, u->userId());

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) { 
		case SSC::Ok: {
			return true;
			      }
		case SSC::RemoveUserFailed: {
			lastErrorString = i18n("Couldn't delete user");
			break;
			}
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}

bool SSCDAccess::addUserInInstitution(UserInInstitution* uii)
{
	sendObject(SSC::AddUserInstitutionAssociation, uii);

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) { 
		case SSC::Ok: {
			return true;
			      }
		case SSC::UserRetrievalFailed: {
			lastErrorString = i18n("Couldn't find user");
			break;
					 }
		case SSC::InstitutionRetrievalFailed: {
			lastErrorString = i18n("Couldn't find institution");
			break;
					 }
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}

bool SSCDAccess::deleteUserInInstitution(UserInInstitution* uii)
{
	sendRequest(SSC::RemoveUserInstitutionAssociation, uii->userId(), uii->institutionId());

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) { 
		case SSC::Ok: {
			return true;
			      }
		case SSC::UserRetrievalFailed: {
			lastErrorString = i18n("Couldn't find record");
			break;
			}
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}

QList<UserInInstitution*> SSCDAccess::getUserInInstitutions(qint32 userId, bool *ok)
{
	sendRequest(SSC::GetUserInstitutionAssociations, userId);

	QList<UserInInstitution*> userInInstitutions;

	QByteArray msg;
	QDataStream stream(&msg, QIODevice::ReadOnly);
	waitForMessage(sizeof(qint32),stream, msg);
	qint32 type;
	stream >> type;
	switch (type) {
		case SSC::UserInstitutionAssociations: {
			parseLengthHeader();

			qint32 elementCount;
			stream >> elementCount;
			for (int i=0; i < elementCount; i++) {
				QByteArray uiiByte;
				stream >> uiiByte;
				UserInInstitution *uii = new UserInInstitution();
				uii->deserialize(uiiByte);
				userInInstitutions << uii;
			}
		        *ok = true;
			break;
			      }

		case SSC::UserRetrievalFailed: {
			lastErrorString = i18n("User - institution associations could not be read");
			*ok = false;
			 break;
			 }

		default: {
			lastErrorString = i18n("Unknown error");
			*ok = false;
			break;
			 }
	}
	return userInInstitutions;

}

bool SSCDAccess::sendSample(Sample *s)
{
	kDebug() << "Sending sample";
	sendObject(SSC::Sample, s);
	return true;
}

bool SSCDAccess::processSampleAnswer()
{
	QByteArray msg;
	QDataStream streamRet(&msg, QIODevice::ReadOnly);
  int messageCountTheSame = 0;
  qint32 previousMessageCount = 0;

  int breakTime = SSCConfig::timeout() / 100 /* milliseconds */;
  kDebug() << "Break time: " << breakTime;

	while ((unsigned int) msg.count() < (unsigned int) sizeof(qint32)) {
		kDebug() << "Bytes available: " << msg.count() <<
			" looking for " << (unsigned int) sizeof(qint32);
#ifdef Q_OS_WIN32
	  Sleep(100 /* 100 ms */);
#else
	  usleep(100000 /* 100 ms */);
#endif
    msg += socket->readAll();

    kDebug() << "Bytes still to write: " << socket->bytesToWrite();
    previousMessageCount = socket->bytesToWrite();
    if (previousMessageCount == msg.count())
    {
      if (messageCountTheSame++ == breakTime)
      {
        abort();
        return false;
      }
    }
    else messageCountTheSame = 0;
    previousMessageCount = msg.count();
	}
  msg += socket->readAll();
	qint32 type;
	streamRet >> type;
	switch (type) { 
		case SSC::Ok: {
			return true;
			      }
		case SSC::SampleStorageFailed: {
			lastErrorString = i18n("Server failed to store sample");
			break;
					 }
		default: {
			lastErrorString = i18n("Unknown error");
			break;
			 }
	}
	return false;
}


/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
SSCDAccess::~SSCDAccess()
{
    socket->deleteLater();
    timeoutWatcher->deleteLater();
}


