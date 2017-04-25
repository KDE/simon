/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
//#include "build/ssc/src/sscconfig.h"

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
#include <QThread>
#include <QDataStream>
#include <QStringList>
#include <KMessageBox>

#include <KI18n/KLocalizedString>
#include <QDebug>
#include <KMessageBox>

#define parseLengthHeader()     waitForMessage(sizeof(qint64),stream, msg); \
  qint64 length; \
  stream >> length; \
  waitForMessage(length,stream,msg);

/**
 *	@brief Constructor
 *	@author Peter Grasch
 */
SSCDAccess::SSCDAccess(QWidget* parent) : QObject(parent),
readyToRead(false),
socket(new QSslSocket()),
timeout(3000), //default timeout
timeoutWatcher(new QTimer(this))
{
  connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()), Qt::DirectConnection);
  connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured()), Qt::DirectConnection);
  connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()), Qt::DirectConnection);
  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void SSCDAccess::readyRead()
{
  fprintf(stderr, "Reading...\n");
  qDebug() << "Available: " << socket->bytesAvailable();
  qDebug() << "Thread: " << thread();
  readyToRead = true;
}
const int& SSCDAccess::getTimeout() const
{
  return timeout;
}
void SSCDAccess::setTimeout(const int& timeout_)
{
  timeout=timeout_;
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
  }
  else {
    connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
    socket->connectToHost( server, port );
  }
  timeoutWatcher->start(timeout);
}


void SSCDAccess::errorOccured()
{
  if (timeoutWatcher->isActive())
    timeoutWatcher->stop();

  QList<QSslError> errors = socket->sslErrors();
  if ((errors.count() == 1) && (errors[0].error() == QSslError::SelfSignedCertificate)) {
    if (KMessageBox::questionYesNoCancel(0, i18n("The certificate of the remote host is self-signed and thus not trustworthy. "
    "\n\nDo you still want to continue?"), i18n("Self-Signed Certificate"))==KMessageBox::Yes) {
      socket->ignoreSslErrors();
      return;
    }
    else {
      socket->abort();
      socket->flush();
      socket->close();
    }
  }
  if (socket->error() != QAbstractSocket::UnknownSocketError) {
    qDebug() << "Populating unknown error: " << socket->errorString();
    emit error(socket->errorString());
  }
  else {
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
  emit error(i18nc("%1 is the timeout length", "Request timed out (%1 ms) or connection was reset.\n\nPlease check your network connection and try again.", timeout));
  abort();
}


void SSCDAccess::abort()
{
  timeoutWatcher->stop();
  socket->abort();
  socket->close();
}


/**
 *	@brief Disconnects the socket from sscd
 *
 *	@author Peter Grasch
 */
void SSCDAccess::disconnectFromServer()
{
  if (timeoutWatcher->isActive())
    timeoutWatcher->stop();

  if ((socket->state() == QAbstractSocket::UnconnectedState) ||
    (socket->state() == QAbstractSocket::ConnectingState) ||
  (socket->state() == QAbstractSocket::HostLookupState)) {
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
    lastErrorString = i18n("Could not write request");
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
  fprintf(stderr, "Sending request (2 inputs)");
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  out << request << message;
  if (socket->write(toWrite) == -1) {
    lastErrorString = i18n("Could not write request");
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
    lastErrorString = i18n("Could not write request");
    return false;
  }

  return true;
}


bool SSCDAccess::waitForMessage(qint64 length, QDataStream& stream, QByteArray& message)
{
  qDebug() << "Socket state: " << socket->state();
  Q_ASSERT(stream.device());
  while (stream.device()->bytesAvailable() < length) {
    qDebug() << thread();
    qDebug() << QThread::currentThread();
    qDebug() << socket->thread();
    if (QThread::currentThread() == socket->thread()) {
      if (socket->waitForReadyRead(timeout /*timeout*/)) {
        message += socket->readAll();
      }
      else {
        //timeout reached
        qDebug() << "Timeout reached!" << socket->errorString() << socket->error() << socket->state();
        abort();
        return false;
      }
    }
    else {
      fprintf(stderr, "Calling from a different thread!\n");
      int passedTime = 0;
      while (!readyToRead && (passedTime < timeout)) {
      //while ((socket->bytesAvailable() == 0) && (passedTime < SSCConfig::timeout()))
        #ifdef Q_OS_WIN32
        Sleep(100 /* 100 ms */);
        #else
        usleep(100000 /* 100 ms */);
        #endif
        passedTime += 100;
        fprintf(stderr, "Time passed: %d, bytes available: %d\n", (int) passedTime,
          (int) socket->bytesAvailable());
      }
      if (passedTime >= timeout) {
        //timeout
        abort();
        return false;
      }
      else {
        readyToRead = false;
        message += socket->readAll();
      }
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

  fprintf(stderr, "Writing: %d\n", (int) socket->bytesToWrite());
  qDebug() << "Bytes still to write: " << socket->bytesToWrite();
  socket->write(toWrite);
  fprintf(stderr, "Writing: %d\n", (int) socket->bytesToWrite());
  qDebug() << "Bytes still to write: " << socket->bytesToWrite();
  socket->write(body);
  qDebug() << "Bytes still to write: " << socket->bytesToWrite();
  fprintf(stderr, "Just send an object... Bytes still to write: %d\n", (int) socket->bytesToWrite());
}


/*
 * Asks the server for the user identified by the given id;
 * Return value is 0 if an error occurred
 */
User* SSCDAccess::getUser(qint32 id)
{
  if (!sendRequest(SSC::GetUser,id)) {
    // 		qDebug() << "could not send request";
    return 0;
  }

  QByteArray msg;
  QDataStream stream(&msg, QIODevice::ReadOnly);

  waitForMessage(sizeof(qint32),stream, msg);

  qint32 type;
  stream >> type;

  // 	qDebug() << "Received reply: " << type;
  switch (type) {
    case SSC::User:
    {
      parseLengthHeader();
      QByteArray userByte = stream.device()->read(length);

      User *u = new User();
      u->deserialize(userByte);
      return u;
    }

    case SSC::UserRetrievalFailed:
    {
      lastErrorString = i18n("User ID not found");
      return 0;
      break;
    }
    default:
    {
      lastErrorString = i18n("Unknown error");
      break;
    }
  }
  return 0;
}


QList<User*> SSCDAccess::getUsers(User *filterUser, qint32 institutionId, const QString& referenceId, bool *ok)
{
  {                                               //send User and institution data
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
    case SSC::Users:
    {
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

    case SSC::UserRetrievalFailed:
    {
      lastErrorString = i18n("Users could not be read");
      *ok = false;
      break;
    }

    default:
    {
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
    case SSC::UserAdded:
    {
      waitForMessage(sizeof(qint32),stream, msg);
      qint32 userId;
      stream >> userId;
      return userId;
    }
    case SSC::AddUserFailed:
    {
      lastErrorString = i18n("Could not add user");
      break;
    }
    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }

    case SSC::UserRetrievalFailed:
    {
      lastErrorString = i18n("User ID not found");
      break;
    }

    default:
    {
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
    case SSC::Microphones:
    {
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

    case SSC::MicrophoneRetrievalFailed:
    {
      lastErrorString = i18n("Microphones could not be read");
      *ok = false;
      break;
    }

    default:
    {
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
    case SSC::SoundCards:
    {
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

    case SSC::SoundCardRetrievalFailed:
    {
      lastErrorString = i18n("SoundCards could not be read");
      *ok = false;
      break;
    }

    default:
    {
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
  fprintf(stderr, "Sending get or create mic request: %d\n", (int) socket->bytesAvailable());
  waitForMessage(sizeof(qint32),stream, msg);
  qint32 type;
  stream >> type;
  qDebug() << type;
  fprintf(stderr, "Get or create microphone returned: %d\n", type);

  switch (type) {
    case SSC::GotMicrophone:
    {
      waitForMessage(sizeof(qint32),stream, msg);
      qint32 id;
      stream >> id;
      qDebug() << "Received id: " << id;
      fprintf(stderr, "Received id: %d\n", id);

      microphone->setId(id);
      *ok = true;
      /*
      fprintf(stderr, "Still available: %d\n", socket->bytesAvailable());
      Sleep(10000);
      if (socket->bytesAvailable())
      {
        fprintf(stderr, "WTF is in there?\n");
        QByteArray s = socket->readAll();
        fprintf(stderr, "Hier: %s\n", s.constData());
      }
      fprintf(stderr, "Still available: %d\n", socket->bytesAvailable());*/
      return id;
    }

    case SSC::MicrophoneRetrievalFailed:
    {
      lastErrorString = i18n("Microphone could not be fetched / created");
      *ok = false;
      break;
    }

    default:
    {
      /*
      fprintf(stderr, "Still available: %d\n", socket->bytesAvailable());
      Sleep(10000);
      if (socket->bytesAvailable())
      {
        fprintf(stderr, "WTF is in there?\n");
        QByteArray s = socket->readAll();
        fprintf(stderr, "Hier: %s\n", s.constData());
      }
      fprintf(stderr, "Still available: %d\n", socket->bytesAvailable());*/

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
    case SSC::GotSoundCard:
    {
      waitForMessage(sizeof(qint32),stream, msg);
      qint32 id;
      stream >> id;

      soundCard->setId(id);
      *ok = true;
      return id;
    }

    case SSC::SoundCardRetrievalFailed:
    {
      lastErrorString = i18n("Sound card could not be fetched / created");
      *ok = false;
      break;
    }

    default:
    {
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
    case SSC::Languages:
    {
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

    case SSC::LanguageRetrievalFailed:
    {
      lastErrorString = i18n("Languages could not be read");
      *ok = false;
      break;
    }

    default:
    {
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
    case SSC::Institutions:
    {
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

    case SSC::InstitutionRetrievalFailed:
    {
      lastErrorString = i18n("Institutions could not be read");
      *ok = false;
      break;
    }

    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }
    case SSC::AddInstitutionFailed:
    {
      lastErrorString = i18n("Could not add institution");
      break;
    }
    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }

    case SSC::InstitutionRetrievalFailed:
    {
      lastErrorString = i18n("Institution ID not found");
      break;
    }

    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }
    case SSC::RemoveInstitutionFailed:
    {
      lastErrorString = i18n("Could not delete institution");
      break;
    }
    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }
    case SSC::RemoveUserFailed:
    {
      lastErrorString = i18n("Could not delete user");
      break;
    }
    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }
    case SSC::UserRetrievalFailed:
    {
      lastErrorString = i18n("Could not find user");
      break;
    }
    case SSC::InstitutionRetrievalFailed:
    {
      lastErrorString = i18n("Could not find institution");
      break;
    }
    default:
    {
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
    case SSC::Ok:
    {
      return true;
    }
    case SSC::UserRetrievalFailed:
    {
      lastErrorString = i18n("Could not find record");
      break;
    }
    default:
    {
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
    case SSC::UserInstitutionAssociations:
    {
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

    case SSC::UserRetrievalFailed:
    {
      lastErrorString = i18n("User - institution associations could not be read");
      *ok = false;
      break;
    }

    default:
    {
      lastErrorString = i18n("Unknown error");
      *ok = false;
      break;
    }
  }
  return userInInstitutions;

}


bool SSCDAccess::sendSample(Sample *s)
{
  qDebug() << "Sending sample";
  fprintf(stderr, "Sending sample: %d\n", (int) socket->bytesAvailable());
  sendObject(SSC::Sample, s);
  return true;
}


bool SSCDAccess::processSampleAnswer()
{
//   fprintf(stderr, "Processing sample answer: %d\n", (int) socket->bytesAvailable());
//   QByteArray msg = socket->readAll();
//   QDataStream streamRet(&msg, QIODevice::ReadOnly);
//   int messageCountTheSame = 0;
//   qint32 previousMessageCount = 0;
// 
//   int breakTime = timeout / 100 /* milliseconds */;
//   qDebug() << "Break time: " << breakTime;
// 
//   while ((unsigned int) msg.count() < (unsigned int) sizeof(qint32)) {
//     qDebug() << "Bytes available: " << msg.count() <<
//       " looking for " << (unsigned int) sizeof(qint32);
//     qDebug() << "Thread: " << thread();
//     #ifdef Q_OS_WIN32
//     Sleep(100 /* 100 ms */);
//     #else
//     usleep(100000 /* 100 ms */);
//     #endif
//     msg += socket->readAll();
// 
//     qDebug() << "Bytes still to write: " << socket->bytesToWrite();
//     previousMessageCount = socket->bytesToWrite();
//     if (previousMessageCount == msg.count()) {
//       if (messageCountTheSame++ == breakTime) {
//         abort();
// 	qDebug() << "Timeout";
//         return false;
//       }
//     }
//     else messageCountTheSame = 0;
//     previousMessageCount = msg.count();
//   }
//   msg += socket->readAll();

//   bool SSCDAccess::waitForMessage(qint64 length, QDataStream& stream, QByteArray& message)
  QByteArray message;
  QDataStream streamRet(&message, QIODevice::ReadOnly);
  bool ok = waitForMessage(sizeof(qint32), streamRet, message);
  if (!ok)
    return false;
  
  qint32 type;
  streamRet >> type;
  fprintf(stderr, "Server returned on sample storage request: %d\n", type);
  switch (type) {
    case SSC::Ok:
    {
      return true;
    }
    case SSC::SampleStorageFailed:
    {
      lastErrorString = i18n("Server failed to store sample");
      break;
    }
    default:
    {
      lastErrorString = i18n("Unknown error");
      break;
    }
  }
  return false;
}

QString SSCDAccess::remote() const
{
  return socket->peerName();
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
