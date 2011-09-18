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

#ifndef SIMONDCONNECTOR_H
#define SIMONDCONNECTOR_H

#include <QObject>
#include <QList>
#include <QVariant>
#include "simone.h"
#include "recognitionresult.h"

class QSslSocket;
class QTimer;
class QDataStream;
class SoundInput;

class SimondConnector : public QObject
{
    Q_OBJECT

signals:
    void status(const QString& status=QString());
    void error(const QString& error);

    void connectionState(ConnectionState);

    void microphoneLevel(int level, int min, int max);
    void listening();
    void recognizing();
    void recognized(const RecognitionResultList&);

private:
    ConnectionState state;
    QSslSocket *socket;
    QTimer *timeoutTimer;

    QDataStream *response;

    SoundInput *mic;
    bool passThroughSound;

    //helper
    void sendRequest(qint32 requestId, const QByteArray& data=QByteArray(), bool sendLength=true);
    void waitFor(int size);
    void parseLength();

    //protocol
    void login();

private slots:
    void setCurrentState(ConnectionState state);
    void timeoutReached();
    void connectionEstablished();
    void socketError();
    void connectionLost();
    void messageReceived();

    void soundDataAvailable();

    void configurationChanged();

public:
    explicit SimondConnector(QObject *parent = 0);
    void init();

public slots:
    void connectToServer();
    void disconnectFromServer();

    void startRecording();
    void commitRecording();

};

#endif // SIMONDCONNECTOR_H
