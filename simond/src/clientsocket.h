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

#ifndef SIMON_CLIENTSOCKET_H_4485408A4C1743CDB368CCC3616AC16F
#define SIMON_CLIENTSOCKET_H_4485408A4C1743CDB368CCC3616AC16F

#include "recognitioncontrol.h"
#include <simonddatabaseaccess/databaseaccess.h>
#include <simonprotocol/simonprotocol.h>
#include <QSslSocket>
#include <QList>
#include <QHash>
#include <QMutex>
#include <QString>

class RecognitionControlFactory;
const qint8 protocolVersion=5;

class DatabaseAccess;
class RecognitionControl;
class SynchronisationManager;
class ModelCompilationManager;
class ModelCompilationAdapter;
class ContextAdapter;
class Model;
class WAV;
class QHostAddress;

class ClientSocket : public QSslSocket
{
  Q_OBJECT

  private:
    bool m_keepSamples;
    bool m_writeAccess;

    bool synchronisationRunning;

    QString username;

    DatabaseAccess *databaseAccess;
    RecognitionControlFactory *recognitionControlFactory;
    RecognitionControl *recognitionControl;
    SynchronisationManager *synchronisationManager;
    ContextAdapter *contextAdapter;

    QHash<qint8, WAV *> currentSamples;
    QMutex sendingMutex;
    QMutex recognitionInitializationMutex;

    bool waitForMessage(qint64 length, QDataStream& stream, QByteArray& message);
    void send(qint32 requestId, const QByteArray& data, bool includeLength=true);
    void sendCode(Simond::Request code);

  public slots:
    void sendRecognitionResult(const QString& fileName, const RecognitionResultList& recognitionResults);

  private slots:
    void processRecognitionResults(const QString& fileName, const RecognitionResultList& recognitionResults);

    void startSynchronisation();
    void recognitionDone(const QString& fileName);

    void processRequest();
    void slotSocketError();

    bool sendModel(Simond::Request request, const QDateTime& changedTime, Model *m);
    bool sendActiveModel();
    bool sendBaseModel();

    void recognitionReady();
    void recognitionError(const QString& error, const QByteArray& log);
    void recognitionWarning(const QString& warning);
    void recognitionStarted();
    void recognitionStopped();

    bool sendLanguageDescription();
    bool sendTraining();

    void sendAvailableModels();

    void updateModelCompilationParameters();

    void sendModelCompilationLog();
    void slotModelCompilationStatus(QString status, int progressNow, int progressMax);
    void slotModelCompilationError(QString error);
    void slotModelCompilationWordUndefined(const QString& word);
    void slotModelCompilationPhonemeUndefined(const QString& phoneme);
    void slotModelCompilationClassUndefined(const QString& undefClass);

    void synchronisationComplete();
    void synchronisationDone();

    void sendScenarioList();
    void requestScenario(const QString& scenarioId);
    bool sendScenario(const QString& scenarioId);
    bool sendSelectedScenarioList();

    void fetchTrainingSample(const QString& name);
    void sendSample(QString sampleName);

    void activeModelCompiled(const QString& path);
    void activeModelCompilationAborted();

    void closeRecognitionControl();
    void initializeRecognitionSmartly();

  public:
    ClientSocket(int socketDescriptor, DatabaseAccess *databaseAccess, RecognitionControlFactory *factory, bool keepSamples, const QHostAddress& writeAccessHost, QObject *parent=0);

    virtual ~ClientSocket();

    QString getUsername();

};
#endif
