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

#include "recognitioncontrol.h"
#include "threadedsslsocket.h"
#include "recognitionconfiguration.h"

#include <simondstreamer/simondstreamer.h>
#include <simoninfo/simoninfo.h>
#include <simonprotocol/simonprotocol.h>
#include <simonmodelmanagementui/AddWord/addwordview.h>

#include <simonscenarios/trainingmanager.h>
#include <simonscenarios/modelmanager.h>

#include <simonscenarios/languagedescriptioncontainer.h>
#include <simonscenarios/trainingcontainer.h>
#include <simonscenarios/model.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/scenariomanager.h>

#include <simonprogresstracking/operation.h>
#include <simonrecognitionresult/recognitionresult.h>
#include <simoncontextdetection/contextmanager.h>

#include <stdio.h>
#include <unistd.h>

#include <QByteArray>
#include <QTimer>
#include <QProcess>
#include <QFile>
#include <QDataStream>
#include <KDateTime>
#include <QStringList>
#include <QPointer>
#include <QCoreApplication>

#include <KMessageBox>
#include <KLocalizedString>
#include <KDebug>
#include <KPasswordDialog>
#include <KStringHandler>

#define advanceStream(x) \
  qint64 currentPos = ((qint64)msg.device()->pos())-((qint64)x);\
  msgByte.remove(0,(int)x);\
  Q_ASSERT(currentPos >= 0);\
  msg.device()->seek(currentPos);

#define checkIfSynchronisationIsAborting() if (synchronisationOperation && synchronisationOperation->isAborting()) \
  { \
    sendRequest(Simond::AbortSynchronisation); \
    synchronisationDone(); \
    return; \
  }

#define checkIfMessageFinished(length)  if ((unsigned) msg.device()->bytesAvailable() < length) \
  { \
    messageNotYetFinished=true; \
    break; \
  }

#define parseLengthHeader()   checkIfMessageFinished(sizeof(qint64)); \
  qint64 length; \
  msg >> length; \
  checkIfMessageFinished(length);

RecognitionControl* RecognitionControl::instance;

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
RecognitionControl::RecognitionControl() : SimonSender(),
localSimond(0),
blockAutoStart(false),
simondStreamer(new SimondStreamer(this, this)),
recognitionReady(false),
socket(new ThreadedSSLSocket(this)),
synchronisationOperation(0),
modelCompilationOperation(0),
timeoutWatcher(new QTimer(this))
{
  qRegisterMetaType<QList<QSslError> >();
  
  connect(simondStreamer, SIGNAL(started()), this, SLOT(streamStarted()));
  connect(simondStreamer, SIGNAL(stopped()), this, SLOT(streamStopped()));

  connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));

  connect(socket, SIGNAL(readyRead()), this, SLOT(messageReceived()), Qt::QueuedConnection);
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()), Qt::QueuedConnection);
  connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured()), Qt::QueuedConnection);

  connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()), Qt::QueuedConnection);
  connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()), Qt::QueuedConnection);

  connect(this, SIGNAL(simondSystemError(QString)), this, SLOT(disconnectFromServer()));

  connect(ModelManager::getInstance(), SIGNAL(modelChanged()), this, SLOT(askStartSynchronisation()));
  
  timeoutWatcher->setSingleShot(true);
}


RecognitionControl* RecognitionControl::getInstance()
{
  if (!instance) {
    instance = new RecognitionControl();
    connect(qApp, SIGNAL(aboutToQuit()), instance, SLOT(deleteLater()));
  }
  return instance;
}


void RecognitionControl::startPrivateSimond()
{
  if (!localSimond) {
    localSimond = new QProcess(this);
    connect(localSimond, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(startPrivateSimond()));
  }
  if (localSimond->state() != QProcess::NotRunning) {
    localSimond->close();
    localSimond->waitForFinished();
  }

  localSimond->start('"'+KStandardDirs::findExe("simond")+'"');
  localSimond->waitForStarted();
}

void RecognitionControl::startup()
{
  kDebug() << "Starting up";
  RecognitionConfiguration::self()->readConfig();
  
  if (RecognitionConfiguration::startLocalSimond()) {
    startPrivateSimond();
    QTimer::singleShot(300, this, SLOT(actOnAutoConnect()));
  }
  else
    actOnAutoConnect();
}


void RecognitionControl::actOnAutoConnect()
{
  kDebug() << "Acting on auto connect: " << RecognitionConfiguration::juliusdAutoConnect();
  if ( RecognitionConfiguration::juliusdAutoConnect() )
  {
    startConnecting();
  }
}


void RecognitionControl::streamStarted()
{
  kDebug() << "Stream has been started";
  emit recognitionStatusChanged(RecognitionControl::Started);
}


void RecognitionControl::streamStopped()
{
  if (isConnected()) {
    if (!simondStreamer->isRunning())
      emit recognitionStatusChanged(RecognitionControl::Ready);
  }
  //else do not emit anything; The stream could have been stopped
  //by us, disconnecting; That doesn't mean that the recognition is ready, tough
}


bool RecognitionControl::startSimondStreamer()
{
  return simondStreamer->start();
}


bool RecognitionControl::stopSimondStreamer()
{
  return simondStreamer->stop();
}


void RecognitionControl::slotDisconnected()
{
  stopSimondStreamer();

  recognitionReady=false;
  if (synchronisationOperation) {
    if (synchronisationOperation->isRunning())
      synchronisationOperation->canceled();
    synchronisationOperation = 0;
  }
  if (modelCompilationOperation) {
    if (modelCompilationOperation->isRunning())
      modelCompilationOperation->canceled();
    modelCompilationOperation = 0;
  }
}


void RecognitionControl::startConnecting()
{
  if (socket->state() != QAbstractSocket::UnconnectedState)
    disconnectFromServer();

  RecognitionConfiguration::self()->readConfig();
  serverConnectionErrors.clear();
  serverConnectionsToTry = RecognitionConfiguration::juliusdServers();

  if (serverConnectionsToTry.isEmpty())
    return;

  connectToNext();
}


void RecognitionControl::connectToNext()
{
  if (serverConnectionsToTry.isEmpty()) {
    emit connectionError(serverConnectionErrors.join("\n"));
  }
  else {
    QStringList address = serverConnectionsToTry.takeAt(0).split(':');
    if (address.count() == 2)
      connectTo(address[0], address[1].toInt());
    else 
      if (!serverConnectionsToTry.isEmpty())
        connectToNext();
  }
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
void RecognitionControl::connectTo(QString server, quint16 port)
{
  if (socket->state() != QAbstractSocket::UnconnectedState) {
    socket->abort();
  }

  disconnect(socket, SIGNAL(encrypted()), 0, 0);
  disconnect(socket, SIGNAL(connected()), 0, 0);

  if (RecognitionConfiguration::juliusdEncrypted()) {
    socket->setProtocol(QSsl::TlsV1);
    connect(socket, SIGNAL(encrypted()), this, SLOT(connectedTo()));
    socket->connectToHostEncrypted( server, port );
  }
  else {
    connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
    socket->connectToHost( server, port );
  }
  timeoutWatcher->start(RecognitionConfiguration::juliusdConnectionTimeout());
}


void RecognitionControl::errorOccured()
{
  if (timeoutWatcher->isActive())
    timeoutWatcher->stop();

  QList<QSslError> errors = socket->sslErrors();
  if ((errors.count() == 1) && (errors[0].error() == QSslError::SelfSignedCertificate) && (RecognitionConfiguration::juliusdEncrypted())) {
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
    serverConnectionErrors << socket->errorString();
  } else {
                                                  //build ssl error list
    for (int i=0; i < errors.count(); i++)
      serverConnectionErrors << errors[i].errorString();
  }
  connectToNext();
}


/**
 * \brief Returns whether the socket is connected to a valid juliusd server
 *
 * \author Peter Grasch
 * \return bool
 * True = is connected
 */
bool RecognitionControl::isConnected()
{
  if (!socket) return false;

  return (socket->state() == QAbstractSocket::ConnectedState);
}


void RecognitionControl::timeoutReached()
{
  serverConnectionErrors << i18ncp("%1 is timeout length in ms", "Request timed out (%1 ms)", "Request timed out (%1 ms)", RecognitionConfiguration::juliusdConnectionTimeout());
  socket->abort();
  connectToNext();
}


/**
 *	@brief Disconnects the socket from julius
 *
 *	@author Peter Grasch
 */
void RecognitionControl::disconnectFromServer()
{
  if (synchronisationOperation) {
    synchronisationOperation->canceled();
    //		synchronisationOperation->deleteLater();
    synchronisationOperation=0;
  }
  if (modelCompilationOperation) {
    modelCompilationOperation->canceled();
    //		modelCompilationOperation->deleteLater();
    modelCompilationOperation=0;
  }
  serverConnectionsToTry.clear();

  if (timeoutWatcher->isActive())
    timeoutWatcher->stop();

  if ((socket->state() == QAbstractSocket::UnconnectedState) ||
    (socket->state() == QAbstractSocket::ConnectingState) ||
  (socket->state() == QAbstractSocket::HostLookupState)) {
    socket->abort();
    emit disconnected();
    return;
  }

  this->socket->abort();
  this->socket->disconnectFromHost();
}


/**
 *	@brief A Connection has been established
 *
 *	Emits the connected() signal
 *
 *	@author Peter Grasch
 */
void RecognitionControl::connectedTo()
{
  kDebug() << "Connected";
  timeoutWatcher->stop();

  emit connected();
  login();
}

/**
 * \brief Sends a simple request identified by the request id
 * \author Peter Grasch
 * @param request The request to send
 */
void RecognitionControl::sendRequest(qint32 request)
{
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  out << request;

  sendMutex.lock();
  socket->write(toWrite);
  sendMutex.unlock();
}


void RecognitionControl::send(qint32 requestId, const QByteArray& data, bool includeLength)
{
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  out << (qint32) requestId;
  
  if (includeLength)
    out << (qint64) data.count();
  
  sendMutex.lock();
  socket->write(toWrite);
  socket->write(data);
  sendMutex.unlock();
}

void RecognitionControl::login()
{
  QString user = RecognitionConfiguration::juliusdUsername();
  QString pass = RecognitionConfiguration::juliusdPassword();

  if (user.isEmpty()) {
    QPointer<KPasswordDialog> dlg  = new KPasswordDialog(dynamic_cast<QWidget*>(parent()),
                KPasswordDialog::ShowUsernameLine|KPasswordDialog::ShowKeepPassword );
    dlg->setPrompt( i18n( "Please enter your Authentication Details for the Simond below" ));
    if( !dlg->exec() || !dlg || dlg->username().isEmpty() ) {
      disconnectFromServer();                     //the user canceled
      delete dlg;
      return;
    }
    
    user = dlg->username();
    pass = dlg->password();

    if (dlg->keepPassword()) {
      KSharedConfig::Ptr config = KSharedConfig::openConfig("simonrecognitionrc");
      KConfigGroup group(config, "Recognition");
      RecognitionConfiguration::setJuliusdUsername(user);
      RecognitionConfiguration::setJuliusdPassword(pass);
      group.writeEntry("JuliusdUsername", user);
      group.writeEntry("JuliusdPassword", KStringHandler::obscure(pass));
      config->sync();
    }
    delete dlg;
  }
  

  QByteArray userBytes = user.toUtf8();
  QByteArray passBytes = QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Sha1);

  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream << protocolVersion << userBytes << passBytes;
  
  send(Simond::Login, body);
}


bool RecognitionControl::sendActiveModel()
{
  Model *model = ModelManager::getInstance()->createActiveContainer();
  if (!model) {
    emit synchronisationWarning(i18n("Could not create active model container"));
    sendRequest(Simond::ErrorRetrievingActiveModel);
    return false;
  }

  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream << ModelManager::getInstance()->getActiveContainerModifiedTime()
    << model->sampleRate()
    << model->container();

  send(Simond::ActiveModel, body);
  
  delete model;
  return true;
}


void RecognitionControl::sendActiveModelSampleRate()
{
  qint32 smpFreq = ModelManager::getInstance()->getActiveModelSampleRate();

  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  out << smpFreq;

  send(Simond::ActiveModelSampleRate, toWrite, false);
}


void RecognitionControl::sendScenariosToDelete()
{
  kDebug() << "Now sending scenarios to delete...";
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup cg(config, "");

  QStringList deletedScenarios = cg.readEntry("DeletedScenarios", QStringList());
  QStringList deletedScenariosTimes = cg.readEntry("DeletedScenariosTimes", QStringList());

  bodyStream << deletedScenarios
    << deletedScenariosTimes;

  send(Simond::ScenariosToDelete, body);

  cg.writeEntry("DeletedScenarios", QStringList());
  cg.writeEntry("DeletedScenariosTimes", QStringList());
  cg.sync();
}


bool RecognitionControl::sendBaseModel()
{
  kDebug() << "Sending base model";
  Model *model = ModelManager::getInstance()->createBaseModelContainer();
  if (!model) {
    emit synchronisationWarning(i18n("Could not create base model container"));
    sendRequest(Simond::ErrorRetrievingBaseModel);
    return false;
  }

  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream << ModelManager::getInstance()->getBaseModelDate()
    << model->baseModelType()
    << model->container();

  send(Simond::BaseModel, body);

  delete model;
  return true;

}

void RecognitionControl::sendDeactivatedScenarioList()
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream << ScenarioManager::getInstance()->getAllDeactivatedScenarioIds();

  send(Simond::DeactivatedScenarioList, body);
}

void RecognitionControl::sendDeactivatedSampleGroups()
{
  sendDeactivatedSampleGroups(ContextManager::instance()->getDeactivatedSampleGroups());
}

void RecognitionControl::sendDeactivatedSampleGroups(const QStringList& sampleGroups)
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream << sampleGroups;

  send(Simond::DeactivatedSampleGroup, body);
}

void RecognitionControl::sendScenario(QString scenarioId)
{
  checkIfSynchronisationIsAborting();

  kDebug() << "Sending scenario " << scenarioId;
  QFile f(KStandardDirs::locate("appdata", "scenarios/"+scenarioId));
  if (!f.open(QIODevice::ReadOnly)) {
    kDebug() << "Could not retrieve scenario";
    sendRequest(Simond::ErrorRetrievingScenario);
  }
  else {
    kDebug() << "Really sending scenario...";
    QByteArray body;
    QDataStream bodyStream(&body, QIODevice::WriteOnly);
    QByteArray scenarioIdByte = scenarioId.toUtf8();
    QByteArray scenarioByte = f.readAll();
    bodyStream << scenarioIdByte << scenarioByte;
    send(Simond::Scenario, body);
  }
}

void RecognitionControl::sendSelectedScenarioList()
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup cg(config, "");
  QDateTime lastModifiedDate = cg.readEntry("LastModified", QDateTime());
  QStringList selectedScenarios = cg.readEntry("SelectedScenarios", QStringList());

  bodyStream << lastModifiedDate << selectedScenarios;

  send(Simond::SelectedScenarioList, body);
}

void RecognitionControl::sendLanguageDescription()
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  LanguageDescriptionContainer *languageDescription = ModelManager::getInstance()->getLanguageDescriptionContainer();

  bodyStream << ModelManager::getInstance()->getLanguageDescriptionModifiedTime()
    << languageDescription->shadowVocab()
    << languageDescription->languageProfile();

  send(Simond::LanguageDescription, body);

  delete languageDescription;
}

void RecognitionControl::sendTraining()
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  TrainingContainer *training = ModelManager::getInstance()->getTrainingContainer();

  bodyStream << ModelManager::getInstance()->getTrainingModifiedTime()
    << training->sampleRate()
    << training->prompts();
  send(Simond::Training, body);

  delete training;
}


void RecognitionControl::sendSample(QString sampleName)
{
  checkIfSynchronisationIsAborting();
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);

  QByteArray sample = ModelManager::getInstance()->getSample(sampleName);

  if (sample.isNull()) {
    QByteArray body;
    QDataStream bodyStream(&body, QIODevice::WriteOnly);
    bodyStream << sampleName.toUtf8();
    send(Simond::ErrorRetrievingTrainingsSample, body);
    sampleNotAvailable(sampleName);
    return;
  }
  
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream << sampleName.toUtf8() << sample;
  send(Simond::TrainingsSample, body);
}


void RecognitionControl::askStartSynchronisation()
{
  RecognitionConfiguration::self()->readConfig();
  if (isConnected()) {
    switch (RecognitionConfiguration::synchronizationMode()) {
      case 0:                                     //automatic
        startSynchronisation();
        break;
      case 1:                                     //semi-automatic
        if (KMessageBox::questionYesNo(0, i18n("The speech model changed.\n\nSynchronize it now?"))==KMessageBox::Yes)
          startSynchronisation();
        break;
      case 2:                                     //manual
        break;
    }
  }
}


void RecognitionControl::startSynchronisation()
{
  if (!isConnected()) return;

  if (synchronisationOperation) {
    synchronisationOperation->deleteLater();
  }

  synchronisationOperation = new Operation(thread(), i18n("Model synchronization"), i18n("Initializing..."), 0, 100, false);

  kDebug() << "Starting synchronization";
  ModelManager::getInstance()->startGroup();

  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  //base model
  bodyStream << ModelManager::getInstance()->getBaseModelDate();
  //active model
  bodyStream << ModelManager::getInstance()->getActiveContainerModifiedTime();
  //language description
  bodyStream << ModelManager::getInstance()->getLanguageDescriptionModifiedTime();
  //training
  bodyStream << ModelManager::getInstance()->getTrainingModifiedTime();
  //samples to fetch
  QStringList missing, available;
  ModelManager::getInstance()->buildSampleList(available, missing);
  bodyStream << missing;
  //other samples
  bodyStream << available;

  //scenarios to delete
  KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup cg(config, "");
  bodyStream << cg.readEntry("DeletedScenarios", QStringList())
    << cg.readEntry("DeletedScenariosTimes", QStringList());
  //selected scenarios
  bodyStream << cg.readEntry("LastModified", QDateTime());

  cg.writeEntry("DeletedScenarios", QStringList());
  cg.writeEntry("DeletedScenariosTimes", QStringList());
  cg.sync();

  //all scenarios
  QStringList ids = ScenarioManager::getInstance()->getAllAvailableScenarioIds();
  bodyStream << (qint32) ids.count();
  foreach (const QString& id, ids) {
    QDateTime scenarioDate = Scenario::skimDate(Scenario::pathFromId(id));
    kDebug() << "Date for " << id << scenarioDate ;
    bodyStream << id << scenarioDate;
  }

  send(Simond::SynchronisationInformation, body);
}


void RecognitionControl::synchronisationComplete()
{                                                 //successful
  kDebug() << "Synchronization completed";
  if (synchronisationOperation)
    synchronisationOperation->finished();
  synchronisationDone();
}


void RecognitionControl::synchronisationDone()
{
  kDebug() << "Finishing up synchronization";

  if (synchronisationOperation) {
    if (!synchronisationOperation->isFinished()) {
      synchronisationOperation->canceled();
    }
    synchronisationOperation=0;
  }

  ModelManager::getInstance()->commitGroup(true /*silent*/);
  emit synchroniationCompleted();
}


/**
 *	@brief Process the request
 *
 *	Reads the new data from the socket and processes it
 *
 *	@author Peter Grasch
 */
void RecognitionControl::messageReceived()
{
  qint32 type;
  Simond::Request request;
  QByteArray msgByte = stillToProcess;
  QDataStream msg(&msgByte, QIODevice::ReadOnly);
  while (socket->bytesAvailable()) {
    bool messageNotYetFinished = false;
    msgByte += socket->readAll();
    msg.device()->seek(0);
    
    while (((unsigned) msg.device()->bytesAvailable() >= sizeof(qint32)) && !messageNotYetFinished) {
      msg >> type;
      request = (Simond::Request) type;
      switch (request) {
        ////////////////////    LOGIN    //////////////////////////////////////

        case Simond::LoginSuccessful:
        {
          advanceStream(sizeof(qint32));
          emit loggedIn();
          sendDeactivatedScenarioList();
          sendDeactivatedSampleGroups();
          askStartSynchronisation();
          break;
        }

        case Simond::VersionIncompatible:
        {
          advanceStream(sizeof(qint32));
          emit simondSystemError(i18n("Version not supported"));
          break;
        }

        case Simond::AuthenticationFailed:
        {
          advanceStream(sizeof(qint32));
          emit simondSystemError(i18n("Wrong username or password."));
          this->disconnectFromServer();
          break;
        }

        case Simond::AccessDenied:
        {
          advanceStream(sizeof(qint32));
          emit simondSystemError(i18n("Access Denied."));
          this->disconnectFromServer();
          break;
        }

        ////////////////////    SYNCHRONIZATION    ////////////////////////////

        case Simond::StartSynchronisation:
        {
          advanceStream(sizeof(qint32));
          startSynchronisation();
          break;
        }

        case Simond::SynchronisationAlreadyRunning:
        {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("The synchronization is already running.\n\nIf you are sure that this is a mistake, please disconnect from Simond and re-connect."));
          synchronisationDone();
          break;
        }

        case Simond::AbortSynchronisationFailed:
        {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Could not abort synchronization."));
          break;
        }

        case Simond::GetActiveModel:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server requested active model";
          sendActiveModel();
          break;
        }

        case Simond::ActiveModel:
        {
          parseLengthHeader();
          kDebug() << "Server sent active model";

          qint32 sampleRate;
          QByteArray container;

          QDateTime changedTime;
          msg >> changedTime;
          msg >> sampleRate;
          msg >> container;

          storeActiveModel(changedTime, sampleRate, container);

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          checkIfSynchronisationIsAborting();
          break;
        }

        case Simond::GetActiveModelSampleRate:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          sendActiveModelSampleRate();
          break;
        }

        case Simond::ActiveModelStorageFailed:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Could not store active model on server";
          emit synchronisationError(i18n("The server could not store the active model."));

          break;
        }

        case Simond::GetBaseModel:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();
          sendBaseModel();
          break;
        }
        case Simond::BaseModel:
        {
          kDebug() << "Server sent base model";

          parseLengthHeader();

          int baseModelType;
          QDateTime changedTime;
          QByteArray container;

          msg >> changedTime;
          msg >> baseModelType;
          msg >> container;

          storeBaseModel(changedTime, baseModelType, container);

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          checkIfSynchronisationIsAborting();
          break;
        }
        case Simond::BaseModelStorageFailed:
        {
          advanceStream(sizeof(qint32));
          kWarning() << "Base model storage failed";
          break;
        }

        case Simond::GetScenariosToDelete:
        {
          advanceStream(sizeof(qint32));
          kDebug() << "Got deletion request now sending scenarios to delete";
          break;
        }

        case Simond::ScenarioStorageFailed:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server cannot store scenario!";
          emit synchronisationError(i18n("The server could not store the scenario."));
          break;
        }

        case Simond::GetScenario:
        {
          checkIfSynchronisationIsAborting();
          parseLengthHeader();

          QByteArray scenarioNameByte;
          msg >> scenarioNameByte;

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          kDebug() << "Server requested Scenario: " << scenarioNameByte;

          sendScenario(QString::fromUtf8(scenarioNameByte));

          break;
        }

        case Simond::Scenario:
        {
          checkIfSynchronisationIsAborting();
          parseLengthHeader();

          QByteArray scenarioId;
          QByteArray scenario;
          msg >> scenarioId;
          msg >> scenario;

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          kDebug() << "Server sent Scenario " << scenarioId;

          if (!ScenarioManager::getInstance()->storeScenario(QString::fromUtf8(scenarioId), scenario))
            sendRequest(Simond::ScenarioStorageFailed);
          else {
            sendRequest(Simond::ScenarioStored);
          }
          break;
        }

        case Simond::GetSelectedScenarioList:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();
          kDebug() << "Server requested selected scenarios";
          sendSelectedScenarioList();
          break;
        }

        case Simond::SelectedScenarioList:
        {
          checkIfSynchronisationIsAborting();
          parseLengthHeader();

          QStringList list;
          msg >> list;

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          kDebug() << "Server sent Scenario list";

          KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
          KConfigGroup cg(config, "");
          cg.writeEntry("SelectedScenarios", list);
          cg.writeEntry("LastModified", KDateTime::currentUtcDateTime().dateTime());

          if (!ScenarioManager::getInstance()->setupScenarios())
            emit synchronisationError(i18n("Could not re-initialize scenarios. Please restart Simon."));

          break;
        }
        case Simond::SelectedScenarioListStorageFailed:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server could not store selected scenario list";
          emit synchronisationError(i18n("The server could not store the list of selected scenarios."));
          break;
        }

        case Simond::GetTraining:
        {
          synchronisationOperation->update(i18n("Sending Training-Corpus"), 11);
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          if (!synchronisationOperation)
            synchronisationOperation = new Operation(thread(), i18n("Model synchronization"), i18n("Synchronizing Training"), 1, 100, false);

          kDebug() << "Server requested training";
          sendTraining();
          break;
        }

        case Simond::Training:
        {
          synchronisationOperation->update(i18n("Loading Training"), 11);
          checkIfSynchronisationIsAborting();

          kDebug() << "Server sent training";
          parseLengthHeader();
          qint32 sampleRate;
          QByteArray prompts;

          QDateTime changedTime;
          msg >> changedTime;
          msg >> sampleRate;
          msg >> prompts;

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          storeTraining(changedTime, sampleRate,prompts);
          break;
        }

        case Simond::TrainingStorageFailed:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server could not store training";
          emit synchronisationError(i18n("The server could not store the training corpus."));
          break;
        }

        case Simond::GetLanguageDescription:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server requested lang. desc.";
          sendLanguageDescription();
          break;
        }

        case Simond::LanguageDescription:
        {
          checkIfSynchronisationIsAborting();

          parseLengthHeader();

          kDebug() << "Server sent languagedescription";

          QByteArray shadowVocab, languageProfile;
          QDateTime changedTime;
          msg >> changedTime;
          msg >> shadowVocab;
          msg >> languageProfile;
	  
          storeLanguageDescription(changedTime, shadowVocab, languageProfile);
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          break;
        }

        case Simond::LanguageDescriptionStorageFailed:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server could not store languagedescription";
          emit synchronisationError(i18n("The server could not store language description."));
          break;
        }

        case Simond::ErrorRetrievingTrainingsSample:
        {
          parseLengthHeader();
	  QByteArray sampleName;
	  msg >> sampleName;
          sampleNotAvailable(QString::fromUtf8(sampleName));
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          break;
        }

        case Simond::GetTrainingsSample:
        {
          checkIfSynchronisationIsAborting();

          if (synchronisationOperation)
            synchronisationOperation->update(i18n("Synchronizing Training Corpus"), 68);

          parseLengthHeader();

          QByteArray sampleNameByte;
          msg >> sampleNameByte;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          kDebug() << "Server requested sampleNameByte";

          kDebug() << sampleNameByte;
          sendSample(QString::fromUtf8(sampleNameByte));
          break;
        }

        case Simond::TrainingsSample:
        {
          checkIfSynchronisationIsAborting();
          if (synchronisationOperation)
            synchronisationOperation->update(i18n("Synchronizing Training Corpus"), 68);

          parseLengthHeader();

          QByteArray name;
          QByteArray sample;
          msg >> name;
          msg >> sample;

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          kDebug() << "Server sent Training Sample";

          if (!storeSample(QString::fromUtf8(name), sample)) {
            sendRequest(Simond::TrainingsSampleStorageFailed);
          }
          break;
        }

        case Simond::TrainingsSampleStorageFailed:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();

          kDebug() << "Server could not store training sample";
          emit synchronisationError(i18n("The server could not store training sample."));
          break;
        }

        case Simond::SynchronisationCommitFailed:
        {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Could not complete Synchronization"));
          synchronisationDone();
          break;
        }

        case Simond::SynchronisationComplete:
        {
          advanceStream(sizeof(qint32));
          synchronisationComplete();
          break;
        }

        case Simond::AvailableModels:
        {
          kDebug() << "Server sent Available models";

          parseLengthHeader();

          QList<QDateTime> models;
          msg >> models;

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          emit modelsAvailable(models);
          break;
        }
        case Simond::SwitchToModelFailed:
        {
          advanceStream(sizeof(qint32));

          emit synchronisationError(i18n("Could not restore old model"));
          break;
        }

        case Simond::SynchronisationEndPending:
        {
          advanceStream(sizeof(qint32));
          checkIfSynchronisationIsAborting();
          sendRequest(Simond::SynchronisationComplete);
          break;
        }

        ////////////////////    COMPILATION    ////////////////////////////////

        case Simond::ModelCompilationStarted:
        {
          advanceStream(sizeof(qint32));
          if (modelCompilationOperation)
            modelCompilationOperation->canceled();

          modelCompilationOperation = createModelCompilationOperation();
          break;
        }

        case Simond::ModelCompilationStatus:
        {

          parseLengthHeader();

          qint32 progNow, progMax;
          msg >> progNow;
          msg >> progMax;

          QString statusMsg;
          QByteArray statusByte;
          msg >> statusByte;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          statusMsg = QString::fromUtf8(statusByte);

          if (!modelCompilationOperation)
            modelCompilationOperation = createModelCompilationOperation();

          modelCompilationOperation->update(i18nc("Model compilation status message", "Model: %1", statusMsg), progNow, progMax);
          break;
        }

        case Simond::ModelCompilationError:
        {
          parseLengthHeader();

          QString errorMsg;
          QByteArray errorByte;
          QString protocol;
          QByteArray protocolByte;

          msg >> errorByte;
          msg >> protocolByte;

          errorMsg = QString::fromUtf8(errorByte);
          protocol = QString::fromUtf8(protocolByte);
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          if (modelCompilationOperation) {
            modelCompilationOperation->canceled();
            modelCompilationOperation=0;
          }
          emit compilationError(errorMsg, protocol);
          break;
        }

        case Simond::ModelCompilationAborted:
        {
          advanceStream(sizeof(qint32));

          kDebug() << "model compilation aborted!";
          if (modelCompilationOperation) {
            modelCompilationOperation->canceled();
            modelCompilationOperation=0;
          }
          break;
        }

        case Simond::ModelCompilationWordUndefined:
        {
          parseLengthHeader();
          QByteArray word;
          msg >> word;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          modelCompilationOperation->canceled();
          modelCompilationOperation=0;
          wordUndefined(QString::fromUtf8(word));
          break;
        }
        case Simond::ModelCompilationClassUndefined:
        {
          parseLengthHeader();
          QByteArray undefClass;
          msg >> undefClass;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          modelCompilationOperation->canceled();
          modelCompilationOperation=0;
          classUndefined(QString::fromUtf8(undefClass));
          break;
        }
        case Simond::ModelCompilationPhonemeUndefined:
        {
          parseLengthHeader();
          QByteArray phoneme;
          msg >> phoneme;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          modelCompilationOperation->canceled();
          modelCompilationOperation=0;
          phonemeUndefined(QString::fromUtf8(phoneme));
          break;
        }

        case Simond::ModelCompilationCompleted:
        {
          advanceStream(sizeof(qint32));
          if (modelCompilationOperation)
          {
            modelCompilationOperation->finished();
            modelCompilationOperation=0;
          }
          else
          {
              kDebug() << "modelCompilationOperation was NULL on compilation completion!!";
          }
          break;
        }

        //TODO: is this deprecated?
        case Simond::ErrorRetrievingModelCompilationProtocol:
        {
          advanceStream(sizeof(qint32));
          KMessageBox::sorry(0, i18n("Could not retrieve model compilation protocol"));
          break;
        }

        case Simond::ModelCompilationProtocol:
        {
          parseLengthHeader();
          QByteArray protocol;
          msg >> protocol;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          displayCompilationProtocol(QString::fromUtf8(protocol));
          break;
        }

        ////////////////////    RECOGNITION    ////////////////////////////////

        case Simond::RecognitionReady:
        {
          kDebug() << "Recognition is ready...";
          advanceStream(sizeof(qint32));

          emit recognitionStatusChanged(RecognitionControl::Ready);

          recognitionReady = false;

          RecognitionConfiguration::self()->readConfig();
          if (!blockAutoStart && RecognitionConfiguration::automaticallyEnableRecognition())
            sendRequest(Simond::StartRecognition);

          break;
        }

        case Simond::RecognitionError:
        {
          parseLengthHeader();

          QByteArray errormsgByte;
          QByteArray protocolByte;
          msg >> errormsgByte;
          msg >> protocolByte;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          QString errormsg = QString::fromUtf8(errormsgByte);
          QString log = QString::fromUtf8(protocolByte);
          recognitionReady=false;
          emit recognitionError(errormsg, log);
          emit recognitionStatusChanged(RecognitionControl::Ready);
          break;
        }

        case Simond::RecognitionWarning:
        {
          parseLengthHeader();

          QByteArray warningmsgByte;
          msg >> warningmsgByte;
          advanceStream(sizeof(qint32)+sizeof(qint64)+length);
          QString warningmsg = QString::fromUtf8(warningmsgByte);
          emit recognitionWarning(warningmsg);
          break;
        }

        case Simond::RecognitionStarted:
        {
          advanceStream(sizeof(qint32));

          recognitionReady=true;

          startRecognition();
          break;
        }

        case Simond::RecognitionStopped:
        {
          advanceStream(sizeof(qint32));
          recognitionReady=false;
          stopSimondStreamer();
          emit recognitionStatusChanged(RecognitionControl::Stopped);
          break;
        }

        case Simond::RecognitionResult:
        {
          parseLengthHeader();

          qint8 sentenceCount;
          msg >> sentenceCount;

          emit receivedResults();
          RecognitionResultList recognitionResults;

          for (int i=0; i < sentenceCount; i++) {
            QByteArray word, sampa, samparaw;
            QList<float> confidenceScores;
            msg >> word;
            msg >> sampa;
            msg >> samparaw;
            msg >> confidenceScores;
            recognitionResults.append(RecognitionResult(QString::fromUtf8(word),
              QString::fromUtf8(sampa),
              QString::fromUtf8(samparaw),
              confidenceScores));
          }

          advanceStream(sizeof(qint32)+sizeof(qint64)+length);

          emit recognised(recognitionResults);
          break;
        }

        case Simond::ErrorRetrievingBaseModel: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve base model"));
          break;
          }
        case Simond::ErrorRetrievingActiveModel: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve active model"));
          break;
          }
        case Simond::ErrorRetrievingLanguageDescription: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve language description"));
          break;
          }
        case Simond::ErrorRetrievingTraining: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve training data"));
          break;
          }
        case Simond::ErrorRetrievingScenarioList: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve scenario list"));
          break;
          }
        case Simond::ErrorRetrievingSelectedScenarioList: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve selected scenario list"));
          break;
          }
        case Simond::ErrorRetrievingScenario: {
          advanceStream(sizeof(qint32));
          emit synchronisationError(i18n("Failed to retrieve scenario"));
          break;
          }

        default:
          kDebug() << "Unknown request: " << request;
	  Q_ASSERT(false);
      }
    }

    messageNotYetFinished=false;

    //this is actually not the correct place (should be at the end of the function)
    //BUT: that way the very last thing the function does it check if there are still bytes available
    //this _ensures_ that we do not loose something
    stillToProcess=msgByte;
  }
}

void RecognitionControl::abortModelCompilation()
{
  sendRequest(Simond::AbortModelCompilation);
}


bool RecognitionControl::getAvailableModels()
{
  sendRequest(Simond::GetAvailableModels);
  return true;
}

bool RecognitionControl::switchToModel(const QDateTime& model)
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream << model;

  send(Simond::SwitchToModel, body);
  return true;
}

void RecognitionControl::fetchCompilationProtocol()
{
  sendRequest(Simond::GetModelCompilationProtocol);
}

bool RecognitionControl::startRecognition()
{
  if (recognitionReady)
  {
    bool succ = startSimondStreamer();
    if (!succ)
      emit recognitionError(i18n("Failed to activate the recognition.\n\n"
                               "Please check if you configured at least "
                               "one recording to be used for "
                               "recognition in the sound configuration."), 
                               i18n("No details available."));
    return succ;
  }
  
  sendRequest(Simond::StartRecognition);
  return true;
}

bool RecognitionControl::stopRecognition()
{
  bool succ = stopSimondStreamer();
  sendRequest(Simond::StopRecognition);
  return succ;
}


void RecognitionControl::displayCompilationProtocol(const QString& protocol)
{
  KMessageBox::detailedSorry(0, i18n("Protocol:"), protocol);
}

void RecognitionControl::sampleNotAvailable(const QString& sample)
{
  if (KMessageBox::questionYesNo(0, i18nc("%1 is sample path",
  "The sample \"%1\" could neither be found on the local computer nor on the "
  "server.\n\nDo you want to remove it from the training database?", sample)) == KMessageBox::Yes) {
    //kick some poor samples ass
    ModelManager::getInstance()->startGroup();
    QString sampleBaseName = sample.left(sample.length()-4);
    kDebug() << "Deleting: " << sample << sampleBaseName;
    //Was: removeSample
    bool succ = TrainingManager::getInstance()->deletePrompt(sampleBaseName);
    if (succ)
      TrainingManager::getInstance()->savePrompts();

    ModelManager::getInstance()->commitGroup(false /*silent*/);

    if (!succ)
      KMessageBox::error(0, i18n("Could not remove sample from the training corpus"));
  }
}

void RecognitionControl::wordUndefined(const QString& word)
{
  int ret = KMessageBox::questionYesNoCancel(0,
    i18n("The word \"%1\" is used in your training samples but it is not contained "
    "in your wordlist.\n\nWhat do you want to do?", word), QString(),
    KGuiItem(i18n("Remove samples containing the word"), KIcon("list-remove")),
    KGuiItem(i18n("Add the word"), KIcon("list-add")));

  switch (ret) {
    case KMessageBox::Cancel:
      return;
    case KMessageBox::Yes:
      //removing the samples
      if (!TrainingManager::getInstance()->deleteWord(word))
        KMessageBox::information(0,
          i18n("Failed to remove word \"%1\" from the training corpus", word));
      break;
    case KMessageBox::No:
      //adding the word
      AddWordView *addWordView = new AddWordView(0);
      addWordView->createWord(word);
      addWordView->show();
      connect(addWordView, SIGNAL(finished(int)), addWordView, SLOT(deleteLater()));
      break;
  }
}

void RecognitionControl::classUndefined(const QString& undefClass)
{
  KMessageBox::sorry(0, i18n("Your grammar uses the undefined terminal \"%1\".\n\nPlease add a word that uses this terminal or remove the structure(s) containing the terminal from your grammar.", undefClass));
}


void RecognitionControl::phonemeUndefined(const QString& phoneme)
{
  KMessageBox::sorry(0, i18n("The phoneme \"%1\" is undefined.\n\nPlease train at least one word that uses it.", phoneme));
}


Operation* RecognitionControl::createModelCompilationOperation()
{
  Operation* modelCompilationOperation = new Operation(thread(), i18n("Compiling model"), i18n("Initializing..."), 0, 0, false /*not atomic*/);
  connect(modelCompilationOperation, SIGNAL(aborting()), this, SLOT(abortModelCompilation()));
  return modelCompilationOperation;
}

bool RecognitionControl::pauseRecognition()
{
  return stopSimondStreamer();
}

bool RecognitionControl::resumeRecognition()
{
  return startSimondStreamer();
}

void RecognitionControl::startSampleToRecognizePrivate(qint8 id, qint8 channels, qint32 sampleRate)
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream << id << channels << sampleRate;

  send(Simond::RecognitionStartSample, body);
}

void RecognitionControl::sendSampleToRecognizePrivate(qint8 id, const QByteArray& data)
{
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream << id << data;

  send(Simond::RecognitionSampleData, body);

}

void RecognitionControl::recognizeSamplePrivate(qint8 id)
{
  kDebug() << "Recognize on the last transmitted data";

  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  out << id;
  send(Simond::RecognitionSampleFinished, toWrite, false);
}

void RecognitionControl::setBlockAutoStart(bool block)
{
  blockAutoStart = block;
}

bool RecognitionControl::storeBaseModel(const QDateTime& changedTime, int baseModelType, const QByteArray& container)
{
  bool succ = ModelManager::getInstance()->storeBaseModel(changedTime, baseModelType, container);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the base model received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool RecognitionControl::storeLanguageDescription(const QDateTime& changedTime, QByteArray& shadowVocab, const QByteArray& languageProfile)
{
  bool succ = ModelManager::getInstance()->storeLanguageDescription(changedTime, shadowVocab, languageProfile);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the language description received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool RecognitionControl::storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& prompts)
{
  bool succ = ModelManager::getInstance()->storeTraining(changedTime, sampleRate, prompts);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the training corpus received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool RecognitionControl::storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& container)
{
  bool succ = ModelManager::getInstance()->storeActiveModel(changedTime, sampleRate, container);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the active model received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool RecognitionControl::storeSample(const QString& name, const QByteArray& sample)
{
  bool succ = ModelManager::getInstance()->storeSample(name, sample);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is sample name, %2 is training data folder",
      "Could not store the sample %1 received from the server."
      "\n\nPlease check the permissions on the sample folder: %2",
      name, TrainingManager::getInstance()->getTrainingDir()));
  }
  return succ;
}


RecognitionControl::~RecognitionControl()
{
  simondStreamer->stop();
  simondStreamer->deleteLater();
  if (localSimond) {
    localSimond->terminate();
    localSimond->waitForFinished(1000);
    if (localSimond->state() != QProcess::NotRunning)
      localSimond->kill();

    localSimond->deleteLater();
  }
  socket->deleteLater();
  timeoutWatcher->deleteLater();
}
