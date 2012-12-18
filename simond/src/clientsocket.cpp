/* *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include "synchronisationmanager.h"
#include "recognitioncontrol.h"
#include "recognitioncontrolfactory.h"

#include <simonddatabaseaccess/databaseaccess.h>

#ifdef bzero
#undef bzero
#endif

#include <simonscenarios/model.h>
#include <simonscenarios/languagedescriptioncontainer.h>
#include <simonscenarios/trainingcontainer.h>

#include <simonwav/wav.h>

#include <simoncontextadapter/contextadapter.h>

#include <QDir>
#include <QTime>
#include <KDateTime>
#include <QHostAddress>
#include <QMap>
#include <QMutexLocker>

#include <KDebug>
#include <KMessageBox>
#include <QString>
#include <KStandardDirs>
#include <KConfigGroup>

#include <KConfig>

ClientSocket::ClientSocket(int socketDescriptor, DatabaseAccess* databaseAccess, RecognitionControlFactory *factory, bool keepSamples, const QHostAddress& writeAccessHost, QObject *parent)
: QSslSocket(parent),
  m_keepSamples(keepSamples),
  synchronisationRunning(false),
  recognitionControlFactory(factory),
  recognitionControl(0),
  synchronisationManager(0),
  contextAdapter(0)
{
  qRegisterMetaType<RecognitionResultList>("RecognitionResultList");

  Q_ASSERT(databaseAccess);
  this->databaseAccess = databaseAccess;

  kDebug() << "Created ClientSocket with Descriptor " << socketDescriptor;

  this->setSocketDescriptor(socketDescriptor);
   connect(this, SIGNAL(readyRead()), this, SLOT(processRequest()));
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError()));

  //TODO: Implement encryption
  if (false) {                                    //Settings::getB("Encryption"))
    connect(this, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSocketError()));
    startServerEncryption();
  }
  
  this->m_writeAccess = (writeAccessHost == QHostAddress::Any)||(writeAccessHost == this->peerAddress());
  
  kDebug() << "Done constructing";
}


void ClientSocket::waitForMessage(qint64 length, QDataStream& stream, QByteArray& message)
{
  Q_ASSERT(stream.device());
  while (stream.device()->bytesAvailable() < length) {
    if (waitForReadyRead())
      message += readAll();
  }
}


void ClientSocket::processRequest()
{
  QByteArray msg = readAll();
  QDataStream stream(&msg, QIODevice::ReadOnly);
  qint32 type;

  while (!stream.atEnd()) {
    waitForMessage(sizeof(qint32), stream, msg);
    Simond::Request request;
    stream >> type;
    request = (Simond::Request) type;

    if ((request != Simond::Login) &&  (username.isEmpty())) {
      kDebug() << "Sending access denied because user sent request: " << request;
      sendCode(Simond::AccessDenied);
      break;
    } else if(!m_writeAccess) {
      bool skip_request = true;
      
      switch(request) {
        case Simond::Login:
        case Simond::StartRecognition:
        case Simond::RecognitionStartSample:
        case Simond::RecognitionSampleData:
        case Simond::RecognitionSampleFinished:
        case Simond::StopRecognition:
          skip_request = false;  
          break;
        default: 
          break;
      }
      
      if(skip_request) {
        sendCode(Simond::AccessDenied);
        break;
      }
      
    }
    
    switch (request) {
      case Simond::Login:
      {
        kDebug() << "Login requested";
        waitForMessage(sizeof(qint64), stream, msg);

        qint8 remoteProtocolVersion;
        QString user;
        QByteArray userBytes;
        QString pass;
        QByteArray passBytes;

        qint64 length;

        stream >> length;
        waitForMessage(length, stream, msg);

        stream >> remoteProtocolVersion;
        stream >> userBytes;
        stream >> passBytes;
        user = QString::fromUtf8(userBytes);
        pass = passBytes;

        if (remoteProtocolVersion !=  protocolVersion) {
          sendCode(Simond::VersionIncompatible);
          break;
        }

        if (databaseAccess->authenticateUser(user, pass)) {
          //store authentication data
          this->username = user;

          if (contextAdapter) contextAdapter->deleteLater();

          contextAdapter = new ContextAdapter(user, this);
          
          connect(contextAdapter, SIGNAL(modelCompiled(QString)),
                  this, SLOT(activeModelCompiled(QString)));
          connect(contextAdapter, SIGNAL(newModelReady()),
                  this, SLOT(initializeRecognitionSmartly()));
          connect(contextAdapter, SIGNAL(modelCompilationAborted()),
                  this, SLOT(activeModelCompilationAborted()));
          connect(contextAdapter, SIGNAL(status(QString,int,int)),
                  this, SLOT(slotModelCompilationStatus(QString,int,int)));
          connect(contextAdapter, SIGNAL(error(QString)),
                  this, SLOT(slotModelCompilationError(QString)));
          
          connect(contextAdapter, SIGNAL(classUndefined(QString)),
                  this, SLOT(slotModelCompilationClassUndefined(QString)));
          connect(contextAdapter, SIGNAL(wordUndefined(QString)),
                  this, SLOT(slotModelCompilationWordUndefined(QString)));
          connect(contextAdapter, SIGNAL(phonemeUndefined(QString)),
                  this, SLOT(slotModelCompilationPhonemeUndefined(QString)));

          if (recognitionControl)
            closeRecognitionControl();

          if (synchronisationManager)
              synchronisationManager->deleteLater();

          synchronisationManager = new SynchronisationManager(username, this);

          sendCode(Simond::LoginSuccessful);
          initializeRecognitionSmartly();
        } else
          sendCode(Simond::AuthenticationFailed);

        kDebug() << "Done with login";
        break;

      }

      case Simond::StartSynchronisation:
      {
        startSynchronisation();
        break;
      }

      case Simond::SynchronisationInformation:
      {
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        synchronisationRunning = true;

        if (!synchronisationManager->startSynchronisation()) {
          stream.skipRawData(length);
          sendCode(Simond::SynchronisationAlreadyRunning);
          break;
        }

        QDateTime baseModelDate;
        QDateTime activeModelDate;
        QDateTime languageDescriptionDate;
        QDateTime trainingModifiedDate;
        QStringList availableSamplesList;
        QStringList missingSamplesList;
        QStringList deletedScenarios;
        QStringList deletedScenarioTimesStrings;
        QDateTime selectedScenariosDate;
        qint32 allScenariosCount;
        QString scenarioId;
        QDateTime scenarioDate;

        //base models
        stream >> baseModelDate;
        QDateTime localBaseModelDate = synchronisationManager->getBaseModelDate();
	kDebug() << "Base models: " << baseModelDate << localBaseModelDate;
        if (baseModelDate != localBaseModelDate) {
          if ((baseModelDate > localBaseModelDate) || !sendBaseModel())
            sendCode(Simond::GetBaseModel);
        }
        //active model
        stream >> activeModelDate;
        QDateTime localActiveModelDate = synchronisationManager->getActiveModelDate();
	kDebug() << "Active model date: " << activeModelDate << localActiveModelDate;
        if (activeModelDate != localActiveModelDate) {
          if (activeModelDate > localActiveModelDate || !sendActiveModel())
            sendCode(Simond::GetActiveModel);
        }
        if (activeModelDate.isNull())
          sendCode(Simond::GetActiveModelSampleRate);

        //language description
        stream >> languageDescriptionDate;
        QDateTime localLanguageDescriptionDate=synchronisationManager->getLanguageDescriptionDate();
	kDebug() << "Language description date: " << languageDescriptionDate << localLanguageDescriptionDate;
        if (languageDescriptionDate != localLanguageDescriptionDate) {
          if (languageDescriptionDate > localLanguageDescriptionDate || !sendLanguageDescription())
            sendCode(Simond::GetLanguageDescription);
        }

        //training
        stream >> trainingModifiedDate;
        QDateTime localTrainingDate = synchronisationManager->getTrainingDate();
	kDebug() << "Training date: " << trainingModifiedDate << localTrainingDate;
        if (trainingModifiedDate != localTrainingDate) {
          if (localTrainingDate < trainingModifiedDate || !sendTraining()) {
            sendCode(Simond::GetTraining);
          }
        }

        //samples
        stream >> missingSamplesList;
	kDebug() << "Missing samples: " << missingSamplesList;
        foreach (const QString& missingSample, missingSamplesList)
          sendSample(missingSample);

        stream >> availableSamplesList;
	kDebug() << "Available samples: " << availableSamplesList;
        QStringList localSamples = synchronisationManager->getAvailableSamples();
        foreach (const QString& sampleOnClient, availableSamplesList) {
          if (!localSamples.contains(sampleOnClient)) {
            fetchTrainingSample(sampleOnClient);
          }
        }

        //deleted scenarios
        stream >> deletedScenarios;
        stream >> deletedScenarioTimesStrings;
	kDebug() << "Deleted scenarios: " << deletedScenarios;
        QList<QDateTime> deletedScenarioTimes;
        foreach (const QString& str, deletedScenarioTimesStrings)
          deletedScenarioTimes << QDateTime::fromString(str, "yyyy-MM-dd-hh-mm-ss");
        synchronisationManager->deletedScenarios(deletedScenarios, deletedScenarioTimes);

        //scenario selection
        stream >> selectedScenariosDate;
        QDateTime localSelectedScenarioDate = synchronisationManager->selectedScenariosDate();
	kDebug() << "Selected scenario dates: " << selectedScenariosDate << localSelectedScenarioDate;
        if (localSelectedScenarioDate != selectedScenariosDate)
          if (localSelectedScenarioDate < selectedScenariosDate || !sendSelectedScenarioList())
            sendCode(Simond::GetSelectedScenarioList);

        //all scenarios
        stream >> allScenariosCount;
        QStringList clientScenarios;
        for (int i = 0; i < allScenariosCount; i++) {
          stream >> scenarioId;
          stream >> scenarioDate;

          clientScenarios << scenarioId;

          QDateTime localScenarioDate = synchronisationManager->localScenarioDate(scenarioId);
	  kDebug() << "Scenario: " << scenarioId << " local date: " << localScenarioDate << " client date: " << scenarioDate;
          if (localScenarioDate != scenarioDate)
            if (localScenarioDate.isNull() || localScenarioDate < scenarioDate || !sendScenario(scenarioId))
              requestScenario(scenarioId);
        }
	kDebug() << "Local scenarios: " << synchronisationManager->getAllScenarioIds();
	kDebug() << "Client scenarios: " << synchronisationManager->getAllScenarioIds();
        foreach (const QString& localScenarioId, synchronisationManager->getAllScenarioIds()) 
          if (!clientScenarios.contains(localScenarioId))
            sendScenario(localScenarioId);

        sendCode(Simond::SynchronisationEndPending);
        break;
      }

      case Simond::AbortSynchronisation:
      {
        if (!synchronisationRunning) break;

        if (!synchronisationManager->abort())
          sendCode(Simond::AbortSynchronisationFailed);

        synchronisationDone();
      }
      case Simond::SynchronisationComplete:
      {
        if (!synchronisationRunning) break;
	synchronisationComplete();
      }

      case Simond::GetActiveModel:
      {
        sendActiveModel();
        break;
      }

      case Simond::ActiveModel:
      {
        Q_ASSERT(synchronisationManager);

        kDebug() << "Received Active model";

        waitForMessage(sizeof(qint64), stream, msg);

        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        qint32 sampleRate;
        QByteArray container;
        QDateTime changedDate;
        stream >> changedDate;
        stream >> sampleRate;
        stream >> container;

        if (!synchronisationManager->storeActiveModel( changedDate, sampleRate, container)) {
          sendCode(Simond::ActiveModelStorageFailed);
        }
        break;
      }

      case Simond::ErrorRetrievingActiveModel:
      {
        Q_ASSERT(synchronisationManager);

        kDebug() << "Client reported error during the retrieving of the active model";

        kDebug() << "Requesting sample rate";

        if (!sendActiveModel()) {
          sendCode(Simond::GetActiveModelSampleRate);
        }

        break;
      }

      case Simond::ActiveModelSampleRate:
      {
        Q_ASSERT(synchronisationManager);
        qint32 sampleRate;
        waitForMessage(sizeof(qint32), stream, msg);
        stream >> sampleRate;
        kDebug() << "Got sample rate: " << sampleRate;
        synchronisationManager->setActiveModelSampleRate(sampleRate);
        break;
      }

      case Simond::ErrorRetrievingBaseModel:
        kDebug() << "Client failed to retrieve base model!";
        break;

      case Simond::BaseModel:
      {
        Q_ASSERT(synchronisationManager);
        kDebug() << "Received base model";
        waitForMessage(sizeof(qint64), stream, msg);

        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        qint32 baseModelType;
        QByteArray container;
        QDateTime changedDate;
        stream >> changedDate;
        stream >> baseModelType;
        stream >> container;

        //if the new base model type is different from the old one, a new acoustic model should be compiled
        if (!synchronisationManager->storeBaseModel(changedDate, baseModelType, container)) {
          sendCode(Simond::BaseModelStorageFailed);
        }
        break;
      }

      case Simond::DeactivatedScenarioList:
      {
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);
        QStringList scenarioIds;
        stream >> scenarioIds;

        kDebug() << "Received list of scenarios to deactivate: " << scenarioIds;

        contextAdapter->updateDeactivatedScenarios(scenarioIds);
        break;
      }

      case Simond::DeactivatedSampleGroup:
      {
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        QStringList sampleGroups;
        stream >> sampleGroups;

        kDebug() << "Received Sample Groups: " << sampleGroups;

        contextAdapter->updateDeactivatedSampleGroups(sampleGroups);
        break;
      }

      case Simond::ErrorRetrievingScenario:
      {
        kDebug() << "Could not get scenario";
        break;
      }

      case Simond::Scenario:
      {
        kDebug() << "Received scenario";
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        QByteArray scenarioId;
        QByteArray scenario;
        stream >> scenarioId;
        stream >> scenario;

        kDebug() << "Client sent scenario: " << scenarioId;
        if (!synchronisationManager->storeScenario(scenarioId, scenario))
          sendCode(Simond::ScenarioStorageFailed);
        break;
      }

      case Simond::ScenarioStorageFailed:         // we should maybe do something here
      case Simond::ScenarioStored:
      {
        kDebug() << "Client stored scenario";
        break;
      }

      case Simond::ErrorRetrievingTraining:
      {
        kDebug() << "Could not get training";
        break;
      }

      case Simond::SelectedScenarioList:
      {
        kDebug() << "Received selected scenario list";
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);
        QDateTime modifiedDate;
        QStringList scenarioIds;
        stream >> modifiedDate;
        stream >> scenarioIds;

        kDebug() << "Selected scenario list: " << scenarioIds;
        if (!synchronisationManager->storeSelectedScenarioList(modifiedDate, scenarioIds)) {
          sendCode(Simond::SelectedScenarioListStorageFailed);
        }

        break;
      }

      case Simond::Training:
      {
        kDebug() << "Received Training";
        Q_ASSERT(synchronisationManager);

        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;

        waitForMessage(length, stream, msg);

        qint32 sampleRate;
        QByteArray prompts;
        QDateTime changedTime;
        stream >> changedTime;
        stream >> sampleRate;
        stream >> prompts;

        if (!synchronisationManager->storeTraining(changedTime, sampleRate, prompts))
          sendCode(Simond::TrainingStorageFailed);
        break;
      }

      case Simond::ErrorRetrievingLanguageDescription:
      {
        kDebug() << "Could not get languagedescription";
        break;
      }

      case Simond::LanguageDescription:
      {
        kDebug() << "Received languagedescription";
        Q_ASSERT(synchronisationManager);

        qint64 length;
        waitForMessage(sizeof(qint64), stream, msg);
        stream >> length;

        waitForMessage(length, stream, msg);

        QByteArray shadowVocab, languageProfile;
        QDateTime changedTime;

        stream >> changedTime;
        stream >> shadowVocab;
        stream >> languageProfile;

        if (!synchronisationManager->storeLanguageDescription(changedTime, shadowVocab, languageProfile))
          sendCode(Simond::LanguageDescriptionStorageFailed);
        break;
      }

      case Simond::ErrorRetrievingTrainingsSample:
      {
        Q_ASSERT(synchronisationManager);

        qint64 length;
        waitForMessage(sizeof(qint64), stream, msg);
        stream >> length;
        waitForMessage(length, stream, msg);
        QByteArray name;
        stream >> name;

        kWarning() << "WARNING: Not all samples available!";
        kWarning() << "Could not fetch: " << name;
        break;
      }

      case Simond::TrainingsSample:
      {
        Q_ASSERT(synchronisationManager);

        qint64 length;
        waitForMessage(sizeof(qint64), stream, msg);
        stream >> length;

        waitForMessage(length, stream, msg);

        QByteArray name;
        stream >> name;

        QByteArray sample;
        stream >> sample;

        if (!synchronisationManager->storeSample(QString::fromUtf8(name), sample)) {
          sendCode(Simond::TrainingsSampleStorageFailed);
        }

        break;
      }

      case Simond::AbortModelCompilation:
      {
        contextAdapter->abort();
        break;
      }

      case Simond::GetAvailableModels:
      {
        Q_ASSERT(synchronisationManager);
        sendAvailableModels();
        break;
      }

      case Simond::SwitchToModel:
      {
        Q_ASSERT(synchronisationManager);
        qint64 length;
        waitForMessage(sizeof(qint64), stream, msg);
        stream >> length;
        waitForMessage(length, stream, msg);

        QDateTime modelDate;
        stream >> modelDate;

        if (synchronisationManager->switchToModel(modelDate))
          sendCode(Simond::StartSynchronisation); //apply changes
        else
          sendCode(Simond::SwitchToModelFailed);

        break;
      }

      case Simond::GetModelCompilationProtocol:
      {
        if (!contextAdapter->hasBuildLog())
          sendCode(Simond::ErrorRetrievingModelCompilationProtocol);
        else sendModelCompilationLog();
        break;
      }

      case Simond::StartRecognition:
      {
        kDebug() << "Got start recognition";
	recognitionInitializationMutex.lock();
	if (recognitionControl) {
          ContextAdapter::BackendType type;
          QString modelPath;
          contextAdapter->currentModel(modelPath, type);
	  if (type != ContextAdapter::Null) // no null model
            recognitionControl->startRecognition();
	  else
            recognitionStarted();
	}
	recognitionInitializationMutex.unlock();
        break;
      }

      case Simond::StopRecognition:
      {
	if (recognitionControl)
          recognitionControl->stop();
        break;
      }

      case Simond::RecognitionStartSample:
      {
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        qint8 id;
        qint8 channels;
        qint32 sampleRate;
        stream >> id;
        stream >> channels;
        stream >> sampleRate;

        kDebug() << "Starting sample " << id << channels << sampleRate;

        if (currentSamples.contains(id)) {
          WAV* w = currentSamples.value(id);
          w->deleteLater();
          currentSamples.remove(id);
        }

        WAV *currentSample = new WAV(KStandardDirs::locateLocal("appdata", "models/"+username+"/recognitionsamples/"+
          KDateTime::currentUtcDateTime().dateTime().toString("yyyy-MM-dd_hh-mm-ss-zzzz")+'.'+QString::number(id)+".wav"),
          channels, sampleRate);
        currentSamples.insert(id, currentSample);
        currentSample->beginAddSequence();
        break;
      }
      case Simond::RecognitionSampleData:
      {
        //kDebug() << "Received sample data";
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        QByteArray sampleData;
        qint8 id;
        stream >> id;
        stream >> sampleData;
        WAV *w = currentSamples.value(id);
        if (w)
          w->write(sampleData);
        else
          kDebug() << "Received invalid id: " << id;
        break;
      }
      case Simond::RecognitionSampleFinished:
      {
        //kDebug() << "Recognizing on sample";
        waitForMessage(sizeof(qint8), stream, msg);
        qint8 id;
        stream >> id;
        WAV *w = currentSamples.value(id);
        if (w) {
          w->endAddSequence();
          w->writeFile();

          if (recognitionControl)
            recognitionControl->recognize(w->getFilename());

          w->deleteLater();
        } else
          kDebug() << "Received invalid id: " << id;
        currentSamples.remove(id);
        break;
      }

      default:
      {
        kDebug() << "Unknown request: " << request << msg;
      }
    }

    if (bytesAvailable())
      msg += readAll();
  }
}


void ClientSocket::startSynchronisation()
{
  kDebug() << "Entering startSynchronisation";
  if (synchronisationRunning) return;

  kDebug() << "Locking sync.";
  synchronisationRunning = true;

  if (!synchronisationManager->startSynchronisation())
    sendCode(Simond::SynchronisationAlreadyRunning);
  else
    sendCode(Simond::StartSynchronisation);
}

void ClientSocket::activeModelCompiled(const QString& path)
{
  Q_ASSERT(synchronisationManager);
  
  synchronisationManager->modelCompiled(path);
  sendCode(Simond::ModelCompilationCompleted);

  sendActiveModel();
}

void ClientSocket::activeModelCompilationAborted()
{
  sendCode(Simond::ModelCompilationAborted);
}

void ClientSocket::fetchTrainingSample(const QString& sample)
{
  QByteArray sampleByte = sample.toUtf8();

  kDebug() << "Fetching sample " << sample;

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) Simond::GetTrainingsSample
    << (qint64) sampleByte.count()+sizeof(qint32) /*separator*/
    << sampleByte;
  write(toWrite);
}


void ClientSocket::sendScenarioList()
{
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream << synchronisationManager->getAllScenarioIds();

  out << (qint32) Simond::ScenarioList
    << (qint64) body.count();
  write(toWrite);
  write(body);
}


void ClientSocket::requestScenario(const QString& scenarioId)
{
  QByteArray scenarioByte = scenarioId.toUtf8();
  kDebug() << "Fetching scenario " << scenarioId;

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) Simond::GetScenario
                                                  /*separator*/
    << (qint64) (scenarioByte.count()+sizeof(qint32))
    << scenarioByte;
  write(toWrite);
}


bool ClientSocket::sendScenario(const QString& scenarioId)
{
  kDebug() << "Sending scenario " << scenarioId;
  QByteArray scenarioByte = synchronisationManager->getScenario(scenarioId);
  if (scenarioByte.isNull()) {
    sendCode(Simond::ErrorRetrievingScenario);
    return false;
  }

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream << scenarioId.toUtf8() << scenarioByte;

  stream << (qint32) Simond::Scenario
    << (qint64) (body.count());
  write(toWrite);
  write(body);
  return true;
}

bool ClientSocket::sendSelectedScenarioList()
{
  kDebug() << "Sending selected scenario list";
  QStringList list = synchronisationManager->getLatestSelectedScenarioList();
  QByteArray body;

  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream <<  list;

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) Simond::SelectedScenarioList
    << (qint64) (body.count()) /*separator*/;
  write(toWrite);
  write(body);
  return true;
}


void ClientSocket::sendSample(QString sampleName)
{
  Q_ASSERT(synchronisationManager);

  QByteArray sample = synchronisationManager->getSample(sampleName);

  if (sample.isNull()) {
    kDebug() << "Cannot find sample! " << sampleName;

    QByteArray toWrite;
    QDataStream out(&toWrite, QIODevice::WriteOnly);
    QByteArray nameByte = sampleName.toUtf8();
    out << (qint32) Simond::ErrorRetrievingTrainingsSample
      << ((qint64) nameByte.count()) + sizeof(qint32)
      << nameByte;
    write(toWrite);
    return;
  }

  QByteArray toWrite=QByteArray();
  QDataStream out(&toWrite, QIODevice::WriteOnly);

  out << (qint32) Simond::TrainingsSample
    << (qint64) sample.count()
    << sampleName.toUtf8()
    << sample;
  write(toWrite);
}

void ClientSocket::sendCode(Simond::Request code)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) code;
  write(toWrite);
}


void ClientSocket::slotSocketError()
{
  QString error= errorString()+'\n';
  QList<QSslError> sslErrors;
  sslErrors = this->sslErrors();
  for (int i=0; i < sslErrors.count(); i++)
    error+=sslErrors[i].errorString()+'\n';
  kDebug() << error;

  //    ignoreSslErrors();
}


bool ClientSocket::sendActiveModel()
{
  kDebug() << "Sending active model...";
  Q_ASSERT(synchronisationManager);

  Model *model = synchronisationManager->getActiveModel();

  return sendModel(Simond::ActiveModel, synchronisationManager->getActiveModelDate(), model);
}


bool ClientSocket::sendBaseModel()
{
  kDebug() << "Sending base model...";
  Q_ASSERT(synchronisationManager);

  Model *model = synchronisationManager->getBaseModel();
  return sendModel(Simond::BaseModel, synchronisationManager->getBaseModelDate(), model);
}


bool ClientSocket::sendModel(Simond::Request request, const QDateTime& changedTime, Model *model)
{
  if (!model) return false;

  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream << changedTime
    << model->sampleRate()
    << model->container();

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream  << (qint32) request << (qint64) body.count();

  write(toWrite);
  write(body);

  delete model;
  return true;
}

void ClientSocket::synchronisationDone()
{
  kDebug() << "Synchronization done";
  synchronisationRunning=false;
  //reset modelsource

  updateModelCompilationParameters();
  initializeRecognitionSmartly();
}

void ClientSocket::updateModelCompilationParameters()
{
  contextAdapter->updateModelCompilationParameters(synchronisationManager->getModelSrcDate(), synchronisationManager->getBaseModelType(), 
                                                   synchronisationManager->getBaseModelPath(), synchronisationManager->getScenarioPaths(), 
                                                   synchronisationManager->getPromptsPath());
}


void ClientSocket::synchronisationComplete()
{
  kDebug() << "Synchronization complete";
  if (!synchronisationManager->commit()) {
    kDebug() << "Synchronization commit failed";
    sendCode(Simond::SynchronisationCommitFailed);
  }
  else {
    kDebug() << "Synchronization succeeded";
    sendCode(Simond::SynchronisationComplete);
  }

  synchronisationDone();
}


bool ClientSocket::sendLanguageDescription()
{
  kDebug() << "Sending Language Description";
  Q_ASSERT(synchronisationManager);
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  LanguageDescriptionContainer *languageDescription = synchronisationManager->getLanguageDescription();
  if (!languageDescription) return false;

  bodyStream << synchronisationManager->getLanguageDescriptionDate()
    << languageDescription->shadowVocab()
    << languageDescription->languageProfile();

  out << (qint32) Simond::LanguageDescription
    << (qint64) body.count();
  write(toWrite);
  write(body);

  delete languageDescription;
  return true;
}


bool ClientSocket::sendTraining()
{
  Q_ASSERT(synchronisationManager);
  QByteArray toWrite;
  QDataStream out(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  TrainingContainer *training = synchronisationManager->getTraining();
  if (!training) return false;

  bodyStream << synchronisationManager->getTrainingDate()
    << training->sampleRate()
    << training->prompts();
  out << (qint32) Simond::Training
    << (qint64) body.count();
  write(toWrite);
  write(body);

  delete training;
  return true;
}


void ClientSocket::sendAvailableModels()
{
  QMap<QDateTime,QString> models = synchronisationManager->getModels();
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);

  QList<QDateTime> dates = models.keys();
  bodyStream << dates;
  stream << (qint32) Simond::AvailableModels << (qint64) body.count();

  write(toWrite);
  write(body);
}


void ClientSocket::recognitionReady()
{
  sendCode(Simond::RecognitionReady);
}


void ClientSocket::recognitionError(const QString& error, const QByteArray& log)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  QByteArray errorByte = error.toUtf8();

  bodyStream << errorByte << log;
  stream << (qint32) Simond::RecognitionError << (qint64) body.count();

  write(toWrite);
  write(body);
}


void ClientSocket::recognitionWarning(const QString& warning)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray warningByte = warning.toUtf8();
  stream << (qint32) Simond::RecognitionError << (qint64) warningByte.count()+sizeof(qint32) /*separator*/ << warningByte;
  write(toWrite);
}


void ClientSocket::recognitionStarted()
{
  kDebug() << "Recognition started...";
  sendCode(Simond::RecognitionStarted);
}


void ClientSocket::recognitionStopped()
{
  sendCode(Simond::RecognitionStopped);
}

void ClientSocket::recognitionDone(const QString& fileName)
{
  if (!m_keepSamples) {
    kDebug() << "Removing: " << fileName;
    QFile::remove(fileName);
  }
}

void ClientSocket::processRecognitionResults(const QString& fileName, const RecognitionResultList& recognitionResults)
{
  if (m_keepSamples) {
    QFile f(fileName+"-log.txt");
    if (f.open(QIODevice::WriteOnly)) {
      foreach (const RecognitionResult& result, recognitionResults)
        f.write(result.toString().toUtf8()+'\n');

      f.close();
    } else
    kWarning() << "Cannot open output log for sample";
  }

  sendRecognitionResult(fileName, recognitionResults);
}

void ClientSocket::sendRecognitionResult(const QString& fileName, const RecognitionResultList& recognitionResults)
{
  Q_UNUSED(fileName);
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  qint8 recognitionCount = recognitionResults.count();
  bodyStream << recognitionCount;

  for (int i=0; i < recognitionResults.count(); i++) {
    bodyStream << recognitionResults[i].sentence().toUtf8()
      << recognitionResults[i].sampa().toUtf8()
      << recognitionResults[i].sampaRaw().toUtf8()
      << recognitionResults[i].confidenceScores();
  }

  stream << (qint32) Simond::RecognitionResult << (qint64) body.count();
  write(toWrite);
  write(body);
}

QString ClientSocket::getUsername()
{
  return username;
}

void ClientSocket::closeRecognitionControl()
{
  if (!recognitionControl)
    return;
  disconnect(recognitionControl, SIGNAL(recognitionReady()), this, SLOT(recognitionReady()));
  disconnect(recognitionControl, SIGNAL(recognitionError(QString,QByteArray)), this, SLOT(recognitionError(QString,QByteArray)));
  disconnect(recognitionControl, SIGNAL(recognitionWarning(QString)), this, SLOT(recognitionWarning(QString)));
  disconnect(recognitionControl, SIGNAL(recognitionStarted()), this, SLOT(recognitionStarted()));
  disconnect(recognitionControl, SIGNAL(recognitionStopped()), this, SLOT(recognitionStopped()));
  disconnect(recognitionControl, SIGNAL(recognitionResult(QString,RecognitionResultList)), this, SLOT(processRecognitionResults(QString,RecognitionResultList)));
  disconnect(recognitionControl, SIGNAL(recognitionDone(QString)), this, SLOT(recognitionDone(QString)));
  recognitionControlFactory->closeRecognitionControl(recognitionControl);
  recognitionControl = 0;
}

ClientSocket::~ClientSocket()
{
  kDebug() << "Deleting client";
  //leave databaseAccess alone since it is shared
  closeRecognitionControl();

  if (synchronisationManager)
    synchronisationManager->deleteLater();
  if (contextAdapter)
      contextAdapter->deleteLater();

  qDeleteAll(currentSamples);
}

void ClientSocket::sendModelCompilationLog()
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray log = contextAdapter->getGraphicBuildLog().toUtf8();

  stream << (qint32) Simond::ModelCompilationProtocol
    << (qint64) (log.count()+sizeof(qint32) /*separator*/)
    << log;
  write(toWrite);
}


void ClientSocket::slotModelCompilationStatus(QString status, int progressNow, int progressMax)
{
  QByteArray toWrite;
  QByteArray statusByte = status.toUtf8();
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  bodyStream <<  (qint32) progressNow
    << (qint32) progressMax
    << statusByte;

  stream << (qint32) Simond::ModelCompilationStatus
    << (qint64) body.count();

  write(toWrite);
  write(body);
}

void ClientSocket::slotModelCompilationError(QString error)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  QByteArray errorByte = error.toUtf8();
  QByteArray log = contextAdapter->getGraphicBuildLog().toUtf8();
  bodyStream << errorByte << log;

  stream << (qint32) Simond::ModelCompilationError
    << (qint64) body.count();
  write(toWrite);
  write(body);
}

void ClientSocket::slotModelCompilationWordUndefined(const QString& word)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray errorByte = word.toUtf8();
  stream << (qint32) Simond::ModelCompilationWordUndefined
    << (qint64) (errorByte.count()+sizeof(qint32) /*separator*/)
    << errorByte;
  write(toWrite);
}


void ClientSocket::slotModelCompilationClassUndefined(const QString& undefClass)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray classByte = undefClass.toUtf8();
  stream << (qint32) Simond::ModelCompilationClassUndefined
    << (qint64) (classByte.count()+sizeof(qint32) /*separator*/)
    << classByte;
  write(toWrite);
}

void ClientSocket::slotModelCompilationPhonemeUndefined(const QString& phoneme)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray classByte = phoneme.toUtf8();
  stream << (qint32) Simond::ModelCompilationPhonemeUndefined
    << (qint64) (classByte.count()+sizeof(qint32) /*separator*/)
    << classByte;
  write(toWrite);
}

void ClientSocket::initializeRecognitionSmartly()
{
  QMutexLocker l(&recognitionInitializationMutex);
  kDebug() << "Recognition is initialized: " << (recognitionControl ? recognitionControl->isInitialized() : false);
  kDebug() << "Synchronizationmanager has active model: " << synchronisationManager->hasActiveModel();

  QString modelPath;
  ContextAdapter::BackendType type;
  contextAdapter->currentModel(modelPath, type);
  kDebug() << "Called current model";

  if (modelPath.isNull() || type == ContextAdapter::Null) {
    kDebug() << "Null model" << modelPath << type;
    if (recognitionControl)
      recognitionControl->suspend();
    else
      recognitionReady(); // in that case doing nothing is "recognition"
  } else {
    RecognitionControl::BackendType recognitionType;
    QString backendName;
    switch (type) {
    case ContextAdapter::SPHINX:
      backendName = i18nc("Name of CMUs SPHINX speech recognition suite", "CMU SPHINX");
      recognitionType = RecognitionControl::SPHINX;
      break;
    case ContextAdapter::HTK:
      backendName = i18nc("Name of the Julius speech recognition software", "Julius");
      recognitionType = RecognitionControl::HTK;
      break;
    default:
      backendName = i18nc("Name of an unidentified speech recognition backend", "Unknown");
      recognitionType = RecognitionControl::SPHINX;
      break;
    }
    if (!recognitionControl || (recognitionControl && recognitionControl->type() != recognitionType)) {
      kDebug() << "Resetting";
      closeRecognitionControl();

      recognitionControl = recognitionControlFactory->recognitionControl(username, recognitionType);
      if(!recognitionControl)
      {
        recognitionError(i18n("<html><body><p>The required speech recognition backend for this model (\"%1\") is not available.</p><p>Please install it to "
	                      "continue.</p><p>(<a href=\"http://userbase.kde.org/Simon/Back_ends\">More information</a>).</html>"), QByteArray());
        return;
      }
      connect(recognitionControl, SIGNAL(recognitionReady()), this, SLOT(recognitionReady()), Qt::UniqueConnection);
      connect(recognitionControl, SIGNAL(recognitionError(QString,QByteArray)), this, SLOT(recognitionError(QString,QByteArray)), Qt::UniqueConnection);
      connect(recognitionControl, SIGNAL(recognitionWarning(QString)), this, SLOT(recognitionWarning(QString)), Qt::UniqueConnection);
      connect(recognitionControl, SIGNAL(recognitionStarted()), this, SLOT(recognitionStarted()), Qt::UniqueConnection);
      connect(recognitionControl, SIGNAL(recognitionStopped()), this, SLOT(recognitionStopped()), Qt::UniqueConnection);
      connect(recognitionControl, SIGNAL(recognitionResult(QString,RecognitionResultList)), this, SLOT(processRecognitionResults(QString,RecognitionResultList)), Qt::UniqueConnection);
      connect(recognitionControl, SIGNAL(recognitionDone(QString)), this, SLOT(recognitionDone(QString)), Qt::UniqueConnection);
    }
    kDebug() << "Initializing";
    recognitionControl->initializeRecognition(modelPath);
  }
}
