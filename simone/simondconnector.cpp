/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "simondconnector.h"
#include "soundinput.h"
#include "simonprotocol.h"
#include "settings.h"
#include <QDebug>
#include <QTimer>
#include <QCryptographicHash>
#include <QDataStream>
#include <QByteArray>
#include <QtNetwork/QSslSocket>

#define SOCKET_TIMEOUT 15000
#define PROTOCOL_VERSION 5
#define SOUND_CHANNELS 1
#define SOUND_SAMPLERATE 16000

SimondConnector::SimondConnector(QObject *parent) :
    QObject(parent), state(Unconnected),
    socket(new QSslSocket(this)),
    timeoutTimer(new QTimer(this)),
    response(new QDataStream(socket)),
    mic(new SoundInput(SOUND_CHANNELS, SOUND_SAMPLERATE, this)),
    passThroughSound(false)
{
    connect(this, SIGNAL(connectionState(ConnectionState)), this, SLOT(setCurrentState(ConnectionState)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(messageReceived()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
    connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(socketError()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectionEstablished()));
    connect(socket, SIGNAL(encrypted()), this, SLOT(connectionEstablished()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionLost()));

    connect(mic, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(mic, SIGNAL(microphoneLevel(int, int, int)), this, SIGNAL(microphoneLevel(int, int, int)));
    connect(mic, SIGNAL(listening()), this, SLOT(startRecording()));
    connect(mic, SIGNAL(complete()), this, SLOT(commitRecording()));
    connect(mic, SIGNAL(readyRead()), this, SLOT(soundDataAvailable()));

    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(timeoutReached()));
    timeoutTimer->setSingleShot(true);
    timeoutTimer->setInterval(SOCKET_TIMEOUT);
}

void SimondConnector::init()
{
    if (Settings::autoConnect())
        connectToServer();
    if (!mic->init())
        qWarning() << "Failed to initialize audio device";
}

void SimondConnector::setCurrentState(ConnectionState state_)
{
    state = state_;
}

void SimondConnector::connectToServer()
{
    emit status(tr("Connecting to server, please wait..."));
    emit connectionState(Connecting);

    QString server      = Settings::host();
    int port            = Settings::port();

    if (socket->state() != QAbstractSocket::UnconnectedState)
      socket->abort();

    if (false) { //SSL
      socket->setProtocol(QSsl::TlsV1);
      socket->connectToHostEncrypted( server, port );
    } else {
      socket->connectToHost(server, port);
    }

    timeoutTimer->start();
}

void SimondConnector::timeoutReached()
{
    socket->abort();
    socket->close();
    emit connectionState(Unconnected);
    emit error(tr("Timeout connecting to server"));
    emit status(tr("Idle"));
}


void SimondConnector::connectionEstablished()
{
    qDebug() << "Connection established";
    timeoutTimer->stop();
    emit status(tr("Connected; Logging in..."));
    emit connectionState(ConnectedWaiting);

    login();
}

void SimondConnector::socketError()
{
    if (timeoutTimer->isActive())
      timeoutTimer->stop();

    QList<QSslError> errors = socket->sslErrors();
    if ((errors.count() == 1) && (errors[0].error() == QSslError::SelfSignedCertificate)) {
        //silently ignore self signed certificates for now
        socket->ignoreSslErrors();
        return;
    }
    QString errorMessage;
    if (!errors.isEmpty()) {
        for (int i=0; i < errors.count(); i++)
            errorMessage += errors[i].errorString()+"\n";
        errorMessage = errorMessage.trimmed();
    } else
        errorMessage = socket->errorString();

    emit error(tr("Failed to connect to server: ")+errorMessage);

    switch(socket->state()) {
    case QAbstractSocket::UnconnectedState:
        emit connectionState(Unconnected);
        break;
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        emit connectionState(Connecting);
        break;
    case QAbstractSocket::ConnectedState:
    case QAbstractSocket::BoundState:
    case QAbstractSocket::ListeningState:
        emit connectionState(Connected);
        break;
    case QAbstractSocket::ClosingState:
        emit connectionState(Unconnected);
        break;
    }
    emit status(tr("Idle")); //clear current status after error
}

void SimondConnector::connectionLost()
{
    timeoutTimer->stop();
    if (state != Disconnecting) {
        emit error(tr("Connection to server lost"));
    }
    emit connectionState(Unconnected);
    emit status(tr("Disconnected"));
}

void SimondConnector::waitFor(int size)
{
    int oldAvailable = 0;
    int available = socket->bytesAvailable();
    while (available < size) {
        oldAvailable = available;
        socket->waitForReadyRead(3000);
        available = socket->bytesAvailable();
        if (available == oldAvailable) {
            //nothing changed in the last couple of seconds
            socket->abort();
            return;
        }
    }
}


void SimondConnector::disconnectFromServer()
{
    emit status(tr("Disconnecting from server, please wait..."));
    emit connectionState(Disconnecting);
    timeoutTimer->stop();
    socket->abort();
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        emit connectionState(Unconnected);
        emit status(tr("Idle"));
    } else
        socket->disconnectFromHost();
}

void SimondConnector::sendRequest(qint32 requestId, const QByteArray& data, bool sendLength)
{
    QByteArray toWrite;
    QDataStream out(&toWrite, QIODevice::WriteOnly);
    out << requestId;
    if (!data.isEmpty() && sendLength)
        out << (qint64) data.count();
    socket->write(toWrite);

    if (!data.isEmpty())
        socket->write(data);
}

void SimondConnector::configurationChanged()
{
    //settings changed - only relevant for VAD
}

//samples

void SimondConnector::startRecording()
{
    qDebug() << "Starting recording";
    emit listening();

    QByteArray body;
    QDataStream bodyStream(&body, QIODevice::WriteOnly|QIODevice::Unbuffered);
    bodyStream << (qint8) 1 /* mic id */ << mic->channels() << mic->sampleRate();

    sendRequest(Simond::RecognitionStartSample, body);
    passThroughSound = true;
}

void SimondConnector::commitRecording()
{
    qDebug() << "Committing recording";
    emit recognizing();
    passThroughSound = false;

    QByteArray body;
    QDataStream bodyStream(&body, QIODevice::WriteOnly|QIODevice::Unbuffered);
    bodyStream << (qint8) 1 /* mic id */;

    sendRequest(Simond::RecognitionSampleFinished, body, false);
}

void SimondConnector::soundDataAvailable()
{
    if (!passThroughSound) {
        mic->dropCache(); // drop current data
        return;
    }

    QByteArray soundData = mic->readAll();
    if (soundData.isEmpty())
        return;

    QByteArray body;
    QDataStream bodyStream(&body, QIODevice::WriteOnly|QIODevice::Unbuffered);
    bodyStream << (qint8) 1 /* mic id */ << soundData;
    sendRequest(Simond::RecognitionSampleData, body);
}

void SimondConnector::parseLength()
{
    waitFor(sizeof(qint64));
    qint64 length;
    *response >> length;
}

//protocol

void SimondConnector::messageReceived()
{
    waitFor(sizeof(qint32));
    qint32 code;

    *response >> code;
    qDebug() << "Message received: " << code;
    switch (code) {
    case Simond::VersionIncompatible:
        emit error(tr("Server version incompatible with client."));
        disconnectFromServer();
        break;
    case Simond::AuthenticationFailed:
        emit error(tr("Authentication failed. Please check the configured user name and password in the configuration."));
        disconnectFromServer();
        break;
    case Simond::LoginSuccessful:
        emit status(tr("Logged in"));
        break;
    case Simond::RecognitionReady:
        emit status(tr("Recognition ready; Activating..."));
        sendRequest(Simond::StartRecognition);
        break;
    case Simond::RecognitionStarted:
        emit status("Recognition activated");
        emit connectionState(Connected);
        break;
    case Simond::RecognitionError: {
        parseLength();
        QByteArray errorMessage, protocol;
        *response >> errorMessage;
        *response >> protocol;
        emit error(tr("Recognition reported error: ").arg(QString::fromUtf8(errorMessage)));
        disconnectFromServer();
        break;
    }
    case Simond::RecognitionWarning: {
        parseLength();
        QByteArray warningMessage;
        *response >> warningMessage;
        emit error(tr("Recognition reported warning: %1").arg(QString::fromUtf8(warningMessage)));
        break;
    }
    case Simond::RecognitionStopped:
        break; //nothing to do
    case Simond::RecognitionResult: {
        parseLength();

        qint8 sentenceCount;
        *response >> sentenceCount;
        RecognitionResultList recognitionResults;

        for (int i=0; i < sentenceCount; i++) {
          QByteArray word, sampa, samparaw;
          QList<float> confidenceScores;
          *response >> word;
          *response >> sampa;
          *response >> samparaw;
          *response >> confidenceScores;
          recognitionResults.append(RecognitionResult(QString::fromUtf8(word),
            QString::fromUtf8(sampa),
            QString::fromUtf8(samparaw),
            confidenceScores));
        }
        emit recognized(recognitionResults);
        break;
    }

    default:
        qDebug() << "Unhandled request: " << code;
    }
    if (socket->bytesAvailable())
        messageReceived();
}

void SimondConnector::login()
{
    QString user        = Settings::user();
    QString password    = Settings::password();

    qDebug() << "Logging in: " << user << password;

    QByteArray userBytes = user.toUtf8();
    QByteArray passBytes = QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha1);

    QByteArray body;
    QDataStream bodyStream(&body, QIODevice::WriteOnly|QIODevice::Unbuffered);
    bodyStream << (qint8) PROTOCOL_VERSION << userBytes << passBytes;

    sendRequest(Simond::Login, body);
}

