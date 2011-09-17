/* *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
#include "juliuscontrol.h"

#include <simonddatabaseaccess/databaseaccess.h>

#ifdef bzero
#undef bzero
#endif

#include <simonscenarios/model.h>
#include <simonscenarios/languagedescriptioncontainer.h>
#include <simonscenarios/trainingcontainer.h>

#include <simonwav/wav.h>

#include <speechmodelcompilation/modelcompilationmanager.h>
#include <speechmodelcompilationadapter/modelcompilationadapter.h>
#include <speechmodelcompilationadapter/modelcompilationadapterhtk.h>

#include <QDir>
#include <QTime>
#include <KDateTime>
#include <QHostAddress>
#include <QMap>

#include <KDebug>
#include <KMessageBox>
#include <QString>
#include <KStandardDirs>
#include <KConfigGroup>

#include <KConfig>

ClientSocket::ClientSocket(int socketDescriptor, DatabaseAccess* databaseAccess, bool keepSamples, const QHostAddress& writeAccessHost, QObject *parent)
: QSslSocket(parent),
  m_keepSamples(keepSamples),
  synchronisationRunning(false),
  recognitionControl(0),
  synchronisationManager(0),
  modelCompilationManager(0),
  modelCompilationAdapter(0),
  newLexiconHash(0),
  newGrammarHash(0),
  newVocaHash(0)
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

          if (modelCompilationManager) modelCompilationManager->deleteLater();

          modelCompilationManager = new ModelCompilationManager(user, this);
          connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(activeModelCompiled()));
          connect(modelCompilationManager, SIGNAL(activeModelCompilationAborted()), this, SLOT(activeModelCompilationAborted()));
          connect(modelCompilationManager, SIGNAL(status(QString, int, int)), this, SLOT(slotModelCompilationStatus(QString, int, int)));
          connect(modelCompilationManager, SIGNAL(error(QString)), this, SLOT(slotModelCompilationError(QString)));
          connect(modelCompilationManager, SIGNAL(classUndefined(const QString&)), this,
            SLOT(slotModelCompilationClassUndefined(const QString&)));
          connect(modelCompilationManager, SIGNAL(wordUndefined(const QString&)), this,
            SLOT(slotModelCompilationWordUndefined(const QString&)));
          connect(modelCompilationManager, SIGNAL(phonemeUndefined(const QString&)), this,
            SLOT(slotModelCompilationPhonemeUndefined(const QString&)));

          if (modelCompilationAdapter) modelCompilationAdapter->deleteLater();

          modelCompilationAdapter = new ModelCompilationAdapterHTK(user, this);
          connect(modelCompilationAdapter, SIGNAL(adaptionComplete()), this, SLOT(slotModelAdaptionComplete()));
          connect(modelCompilationAdapter, SIGNAL(adaptionAborted()), this, SLOT(slotModelAdaptionAborted()));
          connect(modelCompilationAdapter, SIGNAL(status(QString, int)), this, SLOT(slotModelAdaptionStatus(QString, int)));
          connect(modelCompilationAdapter, SIGNAL(error(QString)), this, SLOT(slotModelAdaptionError(QString)));

          if (recognitionControl)
            recognitionControl->deleteLater();

          recognitionControl = new JuliusControl(username, this);
          connect(recognitionControl, SIGNAL(recognitionReady()), this, SLOT(recognitionReady()));
          connect(recognitionControl, SIGNAL(recognitionError(const QString&, const QByteArray&)), this, SLOT(recognitionError(const QString&, const QByteArray&)));
          connect(recognitionControl, SIGNAL(recognitionWarning(const QString&)), this, SLOT(recognitionWarning(const QString&)));
          connect(recognitionControl, SIGNAL(recognitionStarted()), this, SLOT(recognitionStarted()));
          connect(recognitionControl, SIGNAL(recognitionStopped()), this, SLOT(recognitionStopped()));
          connect(recognitionControl, SIGNAL(recognitionResult(const QString&, const RecognitionResultList&)), this, SLOT(processRecognitionResults(const QString&, const RecognitionResultList&)));
          connect(recognitionControl, SIGNAL(recognitionDone(const QString&)), this, SLOT(recognitionDone(const QString&)));

          if (synchronisationManager )
            synchronisationManager->deleteLater();

          synchronisationManager = new SynchronisationManager(username, this);

          sendCode(Simond::LoginSuccessful);

          if (synchronisationManager->hasActiveModel())
            recognitionControl->initializeRecognition();
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

      case Simond::AbortSynchronisation:
      {
        if (!synchronisationRunning) break;

        if (!synchronisationManager->abort())
          sendCode(Simond::AbortSynchronisationFailed);

        synchronisationDone();
      }

      case Simond::ActiveModelDate:
      {
        kDebug() << "Received ActiveModelDate";
        QDateTime remoteModelDate;
        waitForMessage(sizeof(QDateTime), stream, msg);
        stream >> remoteModelDate;

        Q_ASSERT(synchronisationManager);

        QDateTime localModelDate = synchronisationManager->getActiveModelDate();
        if (remoteModelDate != localModelDate) {
          if (remoteModelDate > localModelDate)
            sendCode(Simond::GetActiveModel);
          else if (!sendActiveModel())
            sendCode(Simond::GetActiveModel);
        }
        else {
          kDebug() << "Active model is up-to-date";
          if (remoteModelDate.isNull())
            sendCode(Simond::GetActiveModelSampleRate);
          else
            sendCode(Simond::GetBaseModelDate);
        }

        break;
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
        QByteArray hmmdefs, tiedlist, dict, dfa;
        QDateTime changedDate;
        stream >> changedDate;
        stream >> sampleRate;
        stream >> hmmdefs;
        stream >> tiedlist;
        stream >> dict;
        stream >> dfa;

        if (!synchronisationManager->storeActiveModel( changedDate, sampleRate, hmmdefs,
        tiedlist, dict, dfa)) {
          sendCode(Simond::ActiveModelStorageFailed);
        }
        sendCode(Simond::GetBaseModelDate);
        break;
      }

      case Simond::ErrorRetrievingActiveModel:
      {
        Q_ASSERT(synchronisationManager);

        kDebug() << "Client reported error during the retrieving of the active model";

        kDebug() << "Requesting sample rate";

        if (!sendActiveModel()) {
          sendCode(Simond::GetActiveModelSampleRate);
          sendCode(Simond::NoActiveModelAvailable);
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

        sendCode(Simond::GetBaseModelDate);
        break;
      }

      case Simond::BaseModelDate:
      {
        Q_ASSERT(synchronisationManager);
        QDateTime remoteModelDate;
        waitForMessage(sizeof(QDateTime), stream, msg);
        stream >> remoteModelDate;

        QDateTime localModelDate = synchronisationManager->getBaseModelDate();
        if (remoteModelDate != localModelDate) {
          if (remoteModelDate > localModelDate)
            sendCode(Simond::GetBaseModel);
          else if (!sendBaseModel())
            sendCode(Simond::GetBaseModel);
        }
        else {
          kDebug() << "Base model is up-to-date";
          sendCode(Simond::GetScenariosToDelete);
        }
        break;
      }

      case Simond::ErrorRetrievingBaseModel:
        kDebug() << "Client failed to retrieve base model!";
        sendScenarioList();
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
        QByteArray hmmdefs, tiedlist, macros, stats;
        QDateTime changedDate;
        stream >> changedDate;
        stream >> baseModelType;
        stream >> hmmdefs;
        stream >> tiedlist;
        stream >> macros;
        stream >> stats;

        if (!synchronisationManager->storeBaseModel(changedDate, baseModelType,
        hmmdefs, tiedlist, macros, stats)) {
          sendCode(Simond::BaseModelStorageFailed);
        }
        sendCode(Simond::GetScenariosToDelete);
        break;
      }

      case Simond::ScenariosToDelete:
      {
        kDebug() << "Received deleted scenario list";
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        QStringList scenarioIds;
        stream >> scenarioIds;
        QStringList scenarioTimesString;
        stream >> scenarioTimesString;

        QList<QDateTime> scenarioTimes;
        foreach (const QString& str, scenarioTimesString)
          scenarioTimes << QDateTime::fromString(str, "yyyy-MM-dd-hh-mm-ss");

        synchronisationManager->deletedScenarios(scenarioIds, scenarioTimes);
        kDebug() << "Sending GetScenarioList";
        sendCode(Simond::GetScenarioList);
        break;
      }

      case Simond::ScenarioList:
      {
        kDebug() << "Received scenario list";
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);
        QStringList scenarioIds;
        stream >> scenarioIds;

        kDebug() << "Scenario list: " << scenarioIds;
        synchronisationManager->buildMissingScenarios(scenarioIds);

        sendScenarioList();
        break;
      }

      case Simond::StartScenarioSynchronisation:
      {
        kDebug() << "Starting scenario synchronization";
        fetchScenario();
        break;
      }

      case Simond::GetScenario:
      {
        kDebug() << "Received scenario";
        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        QByteArray scenarioId;
        stream >> scenarioId;

        kDebug() << "Client requested scenario: " << scenarioId;
        sendScenario(QString::fromUtf8(scenarioId));
        break;
      }

      case Simond::ErrorRetrievingScenario:
      {
        kDebug() << "Could not get scenario";
        synchronisationManager->couldntRetreiveScenario();

        fetchScenario();
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
        if (!synchronisationManager->storeScenario(scenario))
          sendCode(Simond::ScenarioStorageFailed);

        fetchScenario();
        break;
      }

      case Simond::ScenarioSynchronisationComplete:
      {
        kDebug() << "Both now have the same number of scenarios; We have to synchronize those already on both host";
        synchronizeAlreadyAvailableScenarios();
        break;
      }

      case Simond::ScenarioDate:
      {
        QDateTime remoteScenarioDate;
        waitForMessage(sizeof(QDateTime), stream, msg);
        stream >> remoteScenarioDate;
        kDebug() << "Received scenario date: " << remoteScenarioDate;

        //this is only triggered when we have a version of this scenario too
        //so lets check if we want to keep ours or accept the new one
        QString scenarioId = synchronisationManager->commonScenario();
        QDateTime localScenarioDate = synchronisationManager->localScenarioDate(scenarioId);

        kDebug() << "Remote date: " << remoteScenarioDate << "Local date: " << localScenarioDate;

        if (remoteScenarioDate < localScenarioDate) {
          //send our version
          kDebug() << "Our version is more current";
          sendScenario(scenarioId);

        }
        else if (remoteScenarioDate > localScenarioDate) {
          //request clients version
          kDebug() << "Clients version is more current";
          requestScenario(scenarioId);
        }
        else {
          //identical
          kDebug() << "Scenario is already up-to-date";
          synchronisationManager->scenarioUpToDate();
          synchronizeAlreadyAvailableScenarios();
        }
        break;
      }

      case Simond::ScenarioStorageFailed:         // we should maybe do something here
      case Simond::ScenarioStored:
      {
        kDebug() << "Client stored scenario";
        synchronisationManager->scenarioSynchronized();
        fetchScenario();
        break;
      }

      case Simond::SelectedScenarioDate:
      {
        QDateTime remoteSelectedScenarioDate;
        waitForMessage(sizeof(QDateTime), stream, msg);
        stream >> remoteSelectedScenarioDate;
        QDateTime localSelectedScenarioDate = synchronisationManager->selectedScenariosDate();

        kDebug() << "Received selected scenario date" << remoteSelectedScenarioDate << localSelectedScenarioDate;

        if (localSelectedScenarioDate < remoteSelectedScenarioDate) {
          //clients version is newer
          sendCode(Simond::GetSelectedScenarioList);
        }
        else if (localSelectedScenarioDate > remoteSelectedScenarioDate) {
          //my version is newer
          sendSelectedScenarioList();
        }
        else {
          //the same
          kDebug() << "SelectedScenarios up-to-date";
          sendCode(Simond::GetTrainingDate);
        }
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

        sendCode(Simond::GetTrainingDate);
        break;
      }

      case Simond::TrainingDate:
      {
        QDateTime remoteTrainingDate;
        QDateTime localTrainingDate = synchronisationManager->getTrainingDate();
        waitForMessage(sizeof(QDateTime), stream, msg);
        stream >> remoteTrainingDate;

        Q_ASSERT(synchronisationManager);

        //				kDebug() << "Received training date: " << remoteTrainingDate << synchronisationManager->getTrainingDate();
        if (remoteTrainingDate != localTrainingDate) {
          //Training changed
          if (localTrainingDate > remoteTrainingDate) {
            kDebug() << "Sending training";
            if (!sendTraining())
              sendCode(Simond::GetTraining);
          }
          else {
            kDebug() << "Retreiving training";
            sendCode(Simond::GetTraining);
          }
        }
        else {
          kDebug() << "Training is up-to-date";
          sendCode(Simond::GetLanguageDescriptionDate);
        }
        break;
      }

      case Simond::ErrorRetrievingTraining:
      {
        kDebug() << "Could not get training";
        if (!synchronisationManager->hasTraining())
          sendCode(Simond::NoTrainingAvailable);
        else sendTraining();
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
        QByteArray wavConfig, prompts;
        QDateTime changedTime;
        stream >> changedTime;
        stream >> sampleRate;
        stream >> wavConfig;
        stream >> prompts;

        if (!synchronisationManager->storeTraining(changedTime, sampleRate, wavConfig, prompts)) {
          sendCode(Simond::TrainingStorageFailed);
        }
        sendCode(Simond::GetLanguageDescriptionDate);

        break;
      }

      case Simond::LanguageDescriptionDate:
      {
        QDateTime remoteLanguageDescriptionDate;
        QDateTime localLanguageDescriptionDate=synchronisationManager->getLanguageDescriptionDate();
        waitForMessage(sizeof(QDateTime), stream, msg);
        stream >> remoteLanguageDescriptionDate;

        Q_ASSERT(synchronisationManager);

        if (remoteLanguageDescriptionDate.toTime_t() != localLanguageDescriptionDate.toTime_t()) {
          kDebug() << "Language description differs";
          if (localLanguageDescriptionDate.toTime_t() > remoteLanguageDescriptionDate.toTime_t()) {
            if (!sendLanguageDescription())
              sendCode(Simond::GetLanguageDescription);
          } else sendCode(Simond::GetLanguageDescription);
        }
        else {
          kDebug() << "LanguageDescription is up-to-date";
          synchronizeSamples();
        }

        break;
      }

      case Simond::ErrorRetrievingLanguageDescription:
      {
        kDebug() << "Could not get languagedescription";
        if (!synchronisationManager->hasLanguageDescription()) {
          sendCode(Simond::NoLanguageDescriptionAvailable);

          synchronizeSamples();
        } else sendLanguageDescription();
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

        QByteArray treeHed, shadowVocab, languageProfile;
        QDateTime changedTime;

        stream >> changedTime;
        stream >> treeHed;
        stream >> shadowVocab;
        stream >> languageProfile;

        if (!synchronisationManager->storeLanguageDescription(changedTime, shadowVocab, treeHed, languageProfile)) {
          sendCode(Simond::LanguageDescriptionStorageFailed);
        } else
        synchronizeSamples();
        break;
      }

      case Simond::StartTrainingsSampleSynchronisation:
      {
        synchronizeSamples();
        break;
      }

      case Simond::GetTrainingsSample:
      {
        Q_ASSERT(synchronisationManager);

        waitForMessage(sizeof(qint64), stream, msg);
        qint64 length;
        stream >> length;
        waitForMessage(length, stream, msg);

        QByteArray sampleNameByte;
        stream >> sampleNameByte;

        sendSample(QString::fromUtf8(sampleNameByte));

        break;
      }

      case Simond::ErrorRetrievingTrainingsSample:
      {
        Q_ASSERT(synchronisationManager);

        kDebug() << "Not all samples available; Aborting";

        //we can not continue without all the samples
        synchronisationManager->abort();

        synchronisationDone();

        break;
      }

      case Simond::TrainingsSample:
      {
        Q_ASSERT(synchronisationManager);

        qint64 length;
        waitForMessage(sizeof(qint64), stream, msg);
        stream >> length;

        waitForMessage(length, stream, msg);

        QByteArray sample;
        stream >> sample;

        if (!synchronisationManager->storeSample(sample)) {
          sendCode(Simond::TrainingsSampleStorageFailed);
          synchronisationDone();
        } else
        fetchTrainingSample();

        break;
      }

      case Simond::TrainingsSampleSynchronisationComplete:
      {
        synchronisationComplete();
        break;
      }

      case Simond::AbortModelCompilation:
      {
        modelCompilationManager->abort();
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
          startSynchronisation();                 //apply changes
        else
          sendCode(Simond::SwitchToModelFailed);

        break;
      }

      case Simond::GetModelCompilationProtocol:
      {
        Q_ASSERT(modelCompilationManager);
        if (!modelCompilationManager->hasBuildLog())
          sendCode(Simond::ErrorRetrievingModelCompilationProtocol);
        else sendModelCompilationLog();
        break;
      }

      case Simond::StartRecognition:
      {
        kDebug() << "Got start recognition";
        recognitionControl->startRecognition();
        break;
      }

      case Simond::StopRecognition:
      {
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

          recognitionControl->recognize(w->getFilename());
          kDebug() << "Returned from recognize";

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

  if (!synchronisationManager->startSynchronisation()) {
    sendCode(Simond::SynchronisationAlreadyRunning);
  }
  else {
    kDebug() << "Requesting ActiveModelDate";
    sendCode(Simond::GetActiveModelDate);
  }
}


void ClientSocket::activeModelCompiled()
{
  Q_ASSERT(synchronisationManager);
  synchronisationManager->modelCompiled();
  writeHashesToConfig();

  sendCode(Simond::ModelCompilationCompleted);

  startSynchronisation();
}


void ClientSocket::activeModelCompilationAborted()
{
  sendCode(Simond::ModelCompilationAborted);
}


void ClientSocket::synchronizeSamples()
{
  Q_ASSERT(synchronisationManager);
  synchronisationManager->buildMissingSamples();
  fetchTrainingSample();
}


void ClientSocket::fetchTrainingSample()
{
  Q_ASSERT(synchronisationManager);

  QString sample = synchronisationManager->missingSample();
  if (sample.isNull()) {
    kDebug() << "Done fetching samples";
    sendCode(Simond::TrainingsSampleSynchronisationComplete);
    return;
  }

  QByteArray sampleByte = sample.toUtf8();

  kDebug() << "Fetching sample " << sample;

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) Simond::GetTrainingsSample
    << (qint64) sampleByte.count()+sizeof(qint32) /*separator*/
    << sampleByte;
  write(toWrite);
}


void ClientSocket::fetchScenario()
{
  Q_ASSERT(synchronisationManager);

  QString scenario = synchronisationManager->missingScenario();
  if (scenario.isNull()) {
    kDebug() << "Done fetching scenarios";
    sendCode(Simond::ScenarioSynchronisationComplete);
    return;
  }

  requestScenario(scenario);
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


void ClientSocket::sendScenario(const QString& scenarioId)
{
  kDebug() << "Sending scenario " << scenarioId;
  QByteArray scenarioByte = synchronisationManager->getScenario(scenarioId);
  if (scenarioByte.isNull()) {
    sendCode(Simond::ErrorRetrievingScenario);
    return;
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
}


void ClientSocket::synchronizeAlreadyAvailableScenarios()
{
  QString askFor = synchronisationManager->commonScenario();
  if (askFor.isNull()) {
    //done
    sendCode(Simond::GetSelectedScenarioDate);
    return;
  }
  QByteArray scenarioByte = askFor.toUtf8();
  kDebug() << "Asking for date of " << scenarioByte;

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) Simond::GetScenarioDate
                                                  /*separator*/
    << (qint64) (scenarioByte.count()+sizeof(qint32))
    << scenarioByte;
  write(toWrite);
}


void ClientSocket::sendSelectedScenarioList()
{
  kDebug() << "Sending selected scenario list";
  QStringList list = synchronisationManager->getLatestSelectedScenarioList();
  /*	if (list.isNull()) {
      sendCode(Simond::ErrorRetrievingSelectedScenarioList);
      return;
    }*/
  QByteArray body;

  QDataStream bodyStream(&body, QIODevice::WriteOnly);
  bodyStream <<  list;

  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  stream << (qint32) Simond::SelectedScenarioList
    << (qint64) (body.count()) /*separator*/;
  write(toWrite);
  write(body);
}


void ClientSocket::sendSample(QString sampleName)
{
  Q_ASSERT(synchronisationManager);

  sampleName += ".wav";

  QByteArray sample = synchronisationManager->getSample(sampleName);

  if (sample.isNull()) {
    kDebug() << "Can not find sample! Sending error message";
    sendCode(Simond::ErrorRetrievingTrainingsSample);
    synchronisationManager->abort();
    synchronisationDone();
    return;
  }

  QByteArray toWrite=QByteArray();
  QDataStream out(&toWrite, QIODevice::WriteOnly);

  out << (qint32) Simond::TrainingsSample
    << (qint64) sample.count()+sizeof(qint32)     /*separator*/
    << sample;
  write(toWrite);
}


void ClientSocket::sendModelCompilationLog()
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray log = modelCompilationManager->getGraphicBuildLog().toUtf8();

  stream << (qint32) Simond::ModelCompilationProtocol
    << (qint64) (log.count()+sizeof(qint32) /*separator*/)
    << log;
  write(toWrite);
}


void ClientSocket::slotModelCompilationStatus(QString status, int progressNow, int progressMax)
{
  progressNow += modelCompilationAdapter->maxProgress();
  progressMax += modelCompilationAdapter->maxProgress();
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
  QByteArray log = modelCompilationManager->getGraphicBuildLog().toUtf8();
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
  /*	QByteArray toWrite;
    QDataStream stream(&toWrite, QIODevice::WriteOnly);
    QByteArray phonemeByte = phoneme.toUtf8();
    stream << (qint32) Simond::ModelCompilationPhonemeUndefined
      << (qint64) (phonemeByte.count()+sizeof(qint32)) //separator
      << phonemeByte;
    write(toWrite);
    */
  //try to fix it automatically
  modelCompilationAdapter->poisonPhoneme(phoneme);
  recompileModel();
}


void ClientSocket::startModelCompilation()
{
  modelCompilationAdapter->clearPoisonedPhonemes();
  recompileModel();
}


void ClientSocket::recompileModel()
{
  sendCode(Simond::ModelCompilationStarted);

  QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

  int baseModelType = synchronisationManager->getBaseModelType();
  if (baseModelType == -1) {
    kDebug() << "Failed to retrieve base model type: " << baseModelType;
    return;
  }
  kDebug() << "STARTING Adaption";

  switch (baseModelType) {
    case 0:
      //static model
      modelCompilationAdapter->startAdaption(
        (ModelCompilationAdapter::AdaptionType)
        (ModelCompilationAdapter::AdaptLanguageModel),
        activeDir+"lexicon", activeDir+"model.grammar",
        activeDir+"simple.voca", activeDir+"prompts",
        synchronisationManager->getScenarioPaths(),
        synchronisationManager->getPromptsPath());
      break;
    case 1:
      kDebug() << "adapting base model...";
      //adapted base model
      //let it run into dynamic model - no difference at this stage
    case 2:
      //dynamic model
      modelCompilationAdapter->startAdaption(
        (ModelCompilationAdapter::AdaptionType)
        (ModelCompilationAdapter::AdaptAcousticModel|ModelCompilationAdapter::AdaptLanguageModel),
        activeDir+"lexicon", activeDir+"model.grammar",
        activeDir+"simple.voca", activeDir+"prompts",
        synchronisationManager->getScenarioPaths(),
        synchronisationManager->getPromptsPath());
      break;
  }
}


bool ClientSocket::shouldRecompileModel()
{
  QDateTime activeModelDate = synchronisationManager->getActiveModelDate();

  int baseModelType = synchronisationManager->getBaseModelType();

  QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  KConfig config( activeDir+"activerc", KConfig::SimpleConfig );
  KConfigGroup cg(&config, "SerializedModel");
  uint lexiconHash = cg.readEntry("LexiconHash", 0);
  uint vocaHash = cg.readEntry("VocaHash", 0);
  uint grammarHash = cg.readEntry("GrammarHash", 0);

  //check if simple.voca, lexicon or model.grammar changed
  QFile lexiconF(activeDir+"lexicon");
  QFile vocaF(activeDir+"simple.voca");
  QFile grammarF(activeDir+"model.grammar");
  if (!lexiconF.open(QIODevice::ReadOnly) || !vocaF.open(QIODevice::ReadOnly) || !grammarF.open(QIODevice::ReadOnly))
    return true;                                  //technically this will most likely cause the compile to fail but this
  // will at least produce a proper error, AND it is not off the table
  // that some weird model compilation manager can work around this... somehow :)

  newLexiconHash = qHash(lexiconF.readAll());

  ////////
  lexiconF.seek(0);
  kDebug() << "Lexicon hash: " << newLexiconHash << lexiconHash << "Lexicon: " << lexiconF.readAll();
  ////////
  
  newVocaHash = qHash(vocaF.readAll());
  newGrammarHash = qHash(grammarF.readAll());

  if (!newVocaHash || !newGrammarHash || !newLexiconHash)
    return true;

  if ((newLexiconHash != lexiconHash) || (newVocaHash != vocaHash) || (newGrammarHash != grammarHash)) {
    kDebug() << "HASH IS DIFFERENT!";
    return true;
  } else

  if (baseModelType != 0 && (synchronisationManager->getTrainingDate() > activeModelDate))
    return true;

  if (synchronisationManager->getBaseModelDate() > activeModelDate)
    return true;

  kDebug() << "Hashes are the same...";

  return false;
}


void ClientSocket::writeHashesToConfig()
{
  kDebug() << "WRITING HASH: " << newLexiconHash;
  QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
  KConfig config( activeDir+"activerc", KConfig::SimpleConfig );
  KConfigGroup cg(&config, "SerializedModel");

  cg.writeEntry("LexiconHash", newLexiconHash);
  cg.writeEntry("VocaHash", newVocaHash);
  cg.writeEntry("GrammarHash", newGrammarHash);
  cg.sync();
}


void ClientSocket::slotModelAdaptionComplete()
{
  if (!shouldRecompileModel())
  {
    slotModelAdaptionAborted();
    return;
  }

  QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");

  QFileInfo fiGrammar(activeDir+"model.grammar");
  bool hasGrammar = (fiGrammar.size() > 0);

  if (!hasGrammar)
  {
    slotModelAdaptionAborted();
    return;
  }

  int baseModelType = synchronisationManager->getBaseModelType();

  QFileInfo fiPrompts(activeDir+"prompts");
  bool hasPrompts = (fiPrompts.size() > 0);
  if (!hasPrompts) {
    switch (baseModelType) {
      case 1:
        baseModelType = 0;
        break;
      case 2:                                     //do not bother creating the model without prompts
        slotModelAdaptionAborted();
        return;
    }
  }

  switch (baseModelType) {
    case 0:
      //static model
      modelCompilationManager->startCompilation(
        (ModelCompilationManager::CompileLanguageModel),
        activeDir+"hmmdefs", activeDir+"tiedlist",
        activeDir+"model.dict", activeDir+"model.dfa",
        activeDir+"basehmmdefs", activeDir+"basetiedlist",
        activeDir+"basemacros", activeDir+"basestats",
        KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
        activeDir+"lexicon", activeDir+"model.grammar", activeDir+"simple.voca",
        activeDir+"prompts", synchronisationManager->getTreeHedPath(),
        synchronisationManager->getWavConfigPath(),
	"simon/scripts");

      QFile::remove(activeDir+"hmmdefs");
      QFile::remove(activeDir+"tiedlist");
      QFile::copy(activeDir+"basehmmdefs", activeDir+"hmmdefs");
      QFile::copy(activeDir+"basetiedlist", activeDir+"tiedlist");
      break;
    case 1:
      //adapted base model
      kDebug() << "Starting modelcompilationmanager adapter";
      modelCompilationManager->startCompilation(
        (ModelCompilationManager::CompilationType)
        (ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::AdaptSpeechModel),
        activeDir+"hmmdefs", activeDir+"tiedlist",
        activeDir+"model.dict", activeDir+"model.dfa",
        activeDir+"basehmmdefs", activeDir+"basetiedlist",
        activeDir+"basemacros", activeDir+"basestats",
        KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
        activeDir+"lexicon", activeDir+"model.grammar", activeDir+"simple.voca",
        activeDir+"prompts", synchronisationManager->getTreeHedPath(),
        synchronisationManager->getWavConfigPath(),
	"simon/scripts");

      break;

    case 2:
      //dynamic model
      modelCompilationManager->startCompilation(
        (ModelCompilationManager::CompilationType)
        (ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::CompileSpeechModel),
        activeDir+"hmmdefs", activeDir+"tiedlist",
        activeDir+"model.dict", activeDir+"model.dfa",
        activeDir+"basehmmdefs", activeDir+"basetiedlist",
        activeDir+"basemacros", activeDir+"basestats",
        KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
        activeDir+"lexicon", activeDir+"model.grammar", activeDir+"simple.voca",
        activeDir+"prompts", synchronisationManager->getTreeHedPath(),
        synchronisationManager->getWavConfigPath(),
	"simon/scripts");
      break;
  }
}


void ClientSocket::slotModelAdaptionAborted()
{
  activeModelCompilationAborted();
}


void ClientSocket::slotModelAdaptionStatus(QString status, int progressNow)
{
  slotModelCompilationStatus(status, progressNow-modelCompilationAdapter->maxProgress(), 0);
}


void ClientSocket::slotModelAdaptionError(QString error)
{
  QByteArray toWrite;
  QDataStream stream(&toWrite, QIODevice::WriteOnly);
  QByteArray body;
  QDataStream bodyStream(&body, QIODevice::WriteOnly);

  QByteArray errorByte = error.toUtf8();
  bodyStream << errorByte << QByteArray();

  stream << (qint32) Simond::ModelCompilationError
    << (qint64) body.count();
  write(toWrite);
  write(body);
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

  // 	ignoreSslErrors();
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
    << model->hmmDefs()
    << model->tiedList()
    << model->data1()
    << model->data2();

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
  Q_ASSERT(recognitionControl);

  //simond(15006) ClientSocket::synchronisationDone: Restart:  false
  //simond(15006) ClientSocket::synchronisationDone: Restart:  QDateTime("Do. Apr 22 21:26:36 2010") QDateTime("Do. Apr 22 21:16:56 2010")

  //kDebug() << "Restart: " << (recognitionControl->lastSuccessfulStart() <  synchronisationManager->getActiveModelDate());
  //kDebug() << "Restart: " << recognitionControl->lastSuccessfulStart() <<  synchronisationManager->getActiveModelDate();
  kDebug() << "Recognition is initialized: " << recognitionControl->isInitialized();
  kDebug() << "Synchronizationmanager has active model: " << synchronisationManager->hasActiveModel();
  kDebug() << "Modelcompilationmanager is running: : " << modelCompilationManager->isRunning();

  /*
  if (synchronisationManager->hasActiveModel() && !modelCompilationManager->isRunning() &&
    ((recognitionControl->isInitialized() &&
    (recognitionControl->shouldTryToStart(synchronisationManager->getActiveModelDate())))
    //(recognitionControl->lastSuccessfulStart() <  synchronisationManager->getActiveModelDate()))
  || !recognitionControl->isInitialized())) {
  */

  if (synchronisationManager->hasActiveModel() && !modelCompilationManager->isRunning() &&
    recognitionControl->shouldTryToStart(synchronisationManager->getActiveModelDate())) {
    kDebug() << "Initialize recognition";
    recognitionControl->initializeRecognition();
  }
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

    bool shouldRecompileModel = synchronisationManager->shouldRecompileModel();
    kDebug() << "Should recompile model: " << shouldRecompileModel;

    if (shouldRecompileModel)
      startModelCompilation();
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
    << languageDescription->treeHed()
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
    << training->wavConfig()
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
    kWarning() << "Can not open output log for sample";
  }

  emit recognized(username, fileName, recognitionResults);
}

void ClientSocket::sendRecognitionResult(const QString& fileName, const RecognitionResultList& recognitionResults)
{
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

ClientSocket::~ClientSocket()
{
  kDebug() << "Deleting client";
  //leave databaseAccess alone since it is shared
  if (recognitionControl)
    recognitionControl->deleteLater();
  if (synchronisationManager)
    synchronisationManager->deleteLater();
  if (modelCompilationManager)
    modelCompilationManager->deleteLater();
  if (modelCompilationAdapter)
    modelCompilationAdapter->deleteLater();

  qDeleteAll(currentSamples);
}
