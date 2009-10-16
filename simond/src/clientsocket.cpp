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
#include "synchronisationmanager.h"
#include "juliuscontrol.h"

#include <simonddatabaseaccess/databaseaccess.h>

#ifdef bzero
#undef bzero
#endif

#include <speechmodelmanagement/model.h>
#include <speechmodelmanagement/wordlistcontainer.h>
#include <speechmodelmanagement/grammarcontainer.h>
#include <speechmodelmanagement/languagedescriptioncontainer.h>
#include <speechmodelmanagement/trainingcontainer.h>


#include <speechmodelcompilation/modelcompilationmanager.h>

#include <QDir>
#include <QDateTime>
#include <QHostAddress>
#include <QMutexLocker>
#include <QMap>

#include <KDebug>
#include <KMessageBox>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KConfigGroup>

#include <KConfig>


ClientSocket::ClientSocket(int socketDescriptor, DatabaseAccess* databaseAccess, QObject *parent) : QSslSocket(parent),
	synchronisationRunning(false),
	modelSource(ClientSocket::Undefined),
	recognitionControl(0),
	synchronisationManager(0),
	modelCompilationManager(0)
{
	qRegisterMetaType<RecognitionResultList>("RecognitionResultList");

	Q_ASSERT(databaseAccess);
	this->databaseAccess = databaseAccess;

	kDebug() << "Created ClientSocket with Descriptor " << socketDescriptor;

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

void ClientSocket::processRequest()
{
	QByteArray msg = readAll();
	QDataStream stream(&msg, QIODevice::ReadOnly);
	qint32 type;

	
	while (!stream.atEnd())
	{
		Simond::Request request;
		stream >> type;
		request = (Simond::Request) type;

		if ((request != Simond::Login) &&  (username.isEmpty()))
		{
			sendCode(Simond::AccessDenied);
			break;
		}
		
		switch (request)
		{
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
					
				if (remoteProtocolVersion !=  protocolVersion)
				{
					sendCode(Simond::VersionIncompatible);
					break;
				}

				if (databaseAccess->authenticateUser(user, pass))
				{
					//fprintf(stderr, "hier: %s\n", i18n("Test").toAscii().data());
					i18n("");

					//store authentication data
					this->username = user;
					
					if (modelCompilationManager) modelCompilationManager->deleteLater();
					
					modelCompilationManager = new ModelCompilationManager(user, this);
					connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(activeModelCompiled()));
					connect(modelCompilationManager, SIGNAL(activeModelCompilationAborted()), this, SLOT(activeModelCompilationAborted()));
					connect(modelCompilationManager, SIGNAL(status(const QString&, int, int)), this, SLOT(slotModelCompilationStatus(const QString&, int, int)));
					connect(modelCompilationManager, SIGNAL(error(const QString&)), this, SLOT(slotModelCompilationError(const QString&)));
					connect(modelCompilationManager, SIGNAL(classUndefined(const QString&)), this, 
							SLOT(slotModelCompilationClassUndefined(const QString&)));
					connect(modelCompilationManager, SIGNAL(wordUndefined(const QString&)), this, 
							SLOT(slotModelCompilationWordUndefined(const QString&)));
					connect(modelCompilationManager, SIGNAL(phonemeUndefined(const QString&)), this, 
							SLOT(slotModelCompilationPhonemeUndefined(const QString&)));
					
					
					if (recognitionControl) 
						recognitionControl->deleteLater();

					recognitionControl = new JuliusControl(username, this);
					connect(recognitionControl, SIGNAL(recognitionReady()), this, SLOT(recognitionReady()));
					connect(recognitionControl, SIGNAL(recognitionError(const QString&)), this, SLOT(recognitionError(const QString&)));
					connect(recognitionControl, SIGNAL(recognitionWarning(const QString&)), this, SLOT(recognitionWarning(const QString&)));
					connect(recognitionControl, SIGNAL(recognitionAwaitingStream(qint32, qint32)), this, SLOT(recognitionAwaitingStream(qint32, qint32)));
					connect(recognitionControl, SIGNAL(recognitionStarted()), this, SLOT(recognitionStarted()));
					connect(recognitionControl, SIGNAL(recognitionStopped()), this, SLOT(recognitionStopped()));
					connect(recognitionControl, SIGNAL(recognitionPaused()), this, SLOT(recognitionPaused()));
					connect(recognitionControl, SIGNAL(recognitionResumed()), this, SLOT(recognitionResumed()));
					connect(recognitionControl, SIGNAL(recognitionResult(const RecognitionResultList&)), this, SLOT(sendRecognitionResult(const RecognitionResultList&)));

					if (synchronisationManager ) 
						synchronisationManager->deleteLater();

					synchronisationManager = new SynchronisationManager(username, this);
					
					sendCode(Simond::LoginSuccessful);

					if (synchronisationManager->hasActiveModel())
						recognitionControl->initializeRecognition(peerAddress() == QHostAddress::LocalHost);
				} else
					sendCode(Simond::AuthenticationFailed);
				
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
				if (remoteModelDate != localModelDate)
				{
					if (remoteModelDate > localModelDate)
						sendCode(Simond::GetActiveModel);
					else if (!sendActiveModel())
						sendCode(Simond::GetActiveModel);
				} else {
					kDebug() << "Active model is up-to-date";
					if (remoteModelDate.isNull())
						sendCode(Simond::GetActiveModelSampleRate);
					else 
						sendCode(Simond::GetModelSrcDate);
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
				sendCode(Simond::GetModelSrcDate);
				break;
			}

			case Simond::ErrorRetrievingActiveModel:
			{
				Q_ASSERT(synchronisationManager);
				
				kDebug() << "Client reported error during the retrieving of the active model";
				
				kDebug() << "Requesting sample rate";
				
				if (!sendActiveModel())
				{
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
				
				sendCode(Simond::GetModelSrcDate);
				break;
			}

			case Simond::ModelSrcDate:
			{
				kDebug() << "Getting model-src-date";
				QDateTime remoteModelDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteModelDate;
				QDateTime serverModelDate = synchronisationManager->getModelSrcDate();
				kDebug() << "Received modelsrc date: " << remoteModelDate << serverModelDate ;
				
				Q_ASSERT(synchronisationManager);
				if (remoteModelDate > serverModelDate)
				{
					modelSource = ClientSocket::Client;
				} else {
					if (remoteModelDate < serverModelDate)
						modelSource = ClientSocket::Server;
				}
				kDebug() << modelSource;

				if (remoteModelDate != serverModelDate)
				{
					sendCode(Simond::GetTrainingDate);
				} else synchronisationComplete();
				
				break;
			}
			
			
			case Simond::TrainingDate:
			{
				QDateTime remoteTrainingDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteTrainingDate;
				
				Q_ASSERT(synchronisationManager);
				Q_ASSERT(modelSource != ClientSocket::Undefined);
				
//				kDebug() << "Received training date: " << remoteTrainingDate << synchronisationManager->getTrainingDate();
				if (remoteTrainingDate != synchronisationManager->getTrainingDate())
				{
					//Training changed
					if (modelSource == ClientSocket::Server)
					{
						kDebug() << "Sending training";
						if (!sendTraining())
							sendCode(Simond::GetTraining);
					} else {
						kDebug() << "Retreiving training";
						sendCode(Simond::GetTraining);
					}
				} else {
					kDebug() << "Training is up-to-date";
					sendCode(Simond::GetWordListDate);
				}
				break;
			}
			
			case Simond::ErrorRetrievingTraining:
			{
				kDebug() << "Couldn't get training";
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
				
				if (!synchronisationManager->storeTraining(changedTime, sampleRate, wavConfig, prompts))
				{
					sendCode(Simond::TrainingStorageFailed);
				}
				sendCode(Simond::GetWordListDate);
				
				break;
			}
			
			
			
			case Simond::WordListDate:
			{
				kDebug() << "Received Wordlist-Date";
				QDateTime remoteWordListDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteWordListDate;
				
				Q_ASSERT(synchronisationManager);
				Q_ASSERT(modelSource != ClientSocket::Undefined);
				
				kDebug() << "Wordlist: " << remoteWordListDate << synchronisationManager->getWordListDate();
			
				
				if (remoteWordListDate != synchronisationManager->getWordListDate())
				{
					kDebug() << "Remote wordlist not synchronized";
					//wordlist changed
					if (modelSource == ClientSocket::Server)
					{
						if (!sendWordList())
							sendCode(Simond::GetWordList);
					} else {
						kDebug() << "Requesting Wordlist";
						sendCode(Simond::GetWordList);
					}
				} else {
					kDebug() << "Wordlist is up-to-date";
					sendCode(Simond::GetGrammarDate);
				}
				
				break;
			}
			
			case Simond::ErrorRetrievingWordList:
			{
				kDebug() << "Couldn't get wordlist";
				if (!synchronisationManager->hasWordList())
					sendCode(Simond::NoWordListAvailable);
				else sendWordList();
				
				break;
			}
			
			case Simond::WordList:
			{
				kDebug() << "Received Wordlist";
				Q_ASSERT(synchronisationManager);
				
				qint64 length;
				waitForMessage(sizeof(qint64), stream, msg);
				stream >> length;
				
				waitForMessage(length, stream, msg);
				
				QByteArray simple, vocab, lexicon;
				QDateTime changedTime;
				stream >> changedTime;
				stream >> simple;
				stream >> vocab;
				stream >> lexicon;
				
				
				if (!synchronisationManager->storeWordList(changedTime, simple, vocab, lexicon))
				{
					sendCode(Simond::WordListStorageFailed);
				}
				
				sendCode(Simond::GetGrammarDate);
				break;
			}
			
			
			case Simond::GrammarDate:
			{
				kDebug() << "Received grammar date";
				QDateTime remoteGrammarDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteGrammarDate;
				
				Q_ASSERT(synchronisationManager);
				Q_ASSERT(modelSource != ClientSocket::Undefined);

				kDebug() << "Grammar: " << remoteGrammarDate << synchronisationManager->getGrammarDate();

				if (remoteGrammarDate != synchronisationManager->getGrammarDate())
				{
					//grammar changed
					if (modelSource == ClientSocket::Server)
					{
						if (!sendGrammar())
							sendCode(Simond::GetGrammar);
					} else sendCode(Simond::GetGrammar);
				} else  {
					kDebug() << "Grammar is up-to-date";
					sendCode(Simond::GetLanguageDescriptionDate);
				}
					
				break;
			}
			
			case Simond::ErrorRetrievingGrammar:
			{
				kDebug() << "Couldn't get grammar";
				if (!synchronisationManager->hasGrammar())
					sendCode(Simond::NoGrammarAvailable);
				else sendGrammar();
				break;
			}
			
			case Simond::Grammar:
			{
				kDebug() << "Received grammar";
				Q_ASSERT(synchronisationManager);
				
				qint64 length;
				waitForMessage(sizeof(qint64), stream, msg);
				stream >> length;
				
				waitForMessage(length, stream, msg);
				
				QByteArray grammar;
				QDateTime changedTime;
				stream >> changedTime;
				stream >> grammar;
				
				
				if (!synchronisationManager->storeGrammar(changedTime, grammar)) {
					sendCode(Simond::GrammarStorageFailed);
				}
				
				sendCode(Simond::GetLanguageDescriptionDate);
				break;
			}
			
			
			
			case Simond::LanguageDescriptionDate:
			{
				QDateTime remoteLanguageDescriptionDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteLanguageDescriptionDate;
				
				Q_ASSERT(synchronisationManager);
				Q_ASSERT(modelSource != ClientSocket::Undefined);
				
				kDebug() << remoteLanguageDescriptionDate;
				kDebug() << synchronisationManager->getLanguageDescriptionDate();
				
				if (remoteLanguageDescriptionDate != synchronisationManager->getLanguageDescriptionDate())
				{
					//LanguageDescription changed
					if (modelSource == ClientSocket::Server)
					{
						if (!sendLanguageDescription())
							sendCode(Simond::GetLanguageDescription);
					} else sendCode(Simond::GetLanguageDescription);
				} else {
					kDebug() << "Language desc u-t-d";
					kDebug() << "LanguageDescription is up-to-date";
					synchronizeSamples();
				}
				
				break;
			}
			
			
			case Simond::ErrorRetrievingLanguageDescription:
			{
				kDebug() << "Couldn't get languagedescription";
				if (!synchronisationManager->hasLanguageDescription())
				{
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
				
				QByteArray treeHed, shadowVocab;
				QDateTime changedTime;

				stream >> changedTime;
				stream >> treeHed;
				stream >> shadowVocab;
				
				if (!synchronisationManager->storeLanguageDescription(changedTime, shadowVocab, treeHed))
				{
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

				//we can't continue without all the samples
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
				
				if (!synchronisationManager->storeSample(sample))
				{
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
					startSynchronisation(); //apply changes
				else 
					sendCode(Simond::SwitchToModelFailed);

				break;
			}
		
			case Simond::GetModelCompilationProtocol: {
				Q_ASSERT(modelCompilationManager);
				if (!modelCompilationManager->hasBuildLog())
					sendCode(Simond::ErrorRetrievingModelCompilationProtocol);
				else sendModelCompilationLog();
				break;
			}

			case Simond::StartRecognition:
			{
				recognitionControl->start();
				break;
			}

			case Simond::StopRecognition:
			{
				recognitionControl->stop();
				break;
			}

			case Simond::PauseRecognition:
			{
				recognitionControl->pause();
				break;
			}

			case Simond::ResumeRecognition:
			{
				recognitionControl->resume();
				break;
			}
			
			default:
			{
				kDebug() << "Unknown request: " << msg;
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
	{
		sendCode(Simond::SynchronisationAlreadyRunning);
	} else {
		kDebug() << "Requesting ActiveModelDate";
		sendCode(Simond::GetActiveModelDate);
	}
}

void ClientSocket::activeModelCompiled()
{
	Q_ASSERT(synchronisationManager);
	synchronisationManager->modelCompiled();
	sendCode(Simond::ModelCompilationCompleted);
	startSynchronisation();
	
 	recognitionControl->initializeRecognition(peerAddress() == QHostAddress::LocalHost);
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
	if (sample.isNull())
	{
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



void ClientSocket::sendSample(QString sampleName)
{
	Q_ASSERT(synchronisationManager);
	
	sampleName += ".wav";
	
	QByteArray sample = synchronisationManager->getSample(sampleName);
	
	if (sample.isNull())
	{
		kDebug() << "Can not find sample! Sending error message";
		sendCode(Simond::ErrorRetrievingTrainingsSample);
		synchronisationManager->abort();
		synchronisationDone();
		return;
	}

	QByteArray toWrite=QByteArray();
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	out << (qint32) Simond::TrainingsSample
		<< (qint64) sample.count()+sizeof(qint32) /*seperator*/
		<< sample;
	write(toWrite);
}

void ClientSocket::sendModelCompilationLog()
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray log = modelCompilationManager->getGraphicBuildLog().toUtf8();
	
	stream << (qint32) Simond::ModelCompilationProtocol
		<< (qint64) (log.count()+sizeof(qint32) /*seperator*/)
		<< log;
	write(toWrite);
}

void ClientSocket::slotModelCompilationStatus(const QString& status, int progressNow, int progressMax)
{
// 	QMutexLocker l(&messageLocker);
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
// 	waitForBytesWritten(toWrite.count()+body.count());
}

void ClientSocket::slotModelCompilationError(const QString& error)
{
// 	QMutexLocker l(&messageLocker);
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
// 	QMutexLocker l(&messageLocker);
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray errorByte = word.toUtf8();
	stream << (qint32) Simond::ModelCompilationWordUndefined
		<< (qint64) (errorByte.count()+sizeof(qint32) /*seperator*/)
		<< errorByte;
	write(toWrite);
}


void ClientSocket::slotModelCompilationClassUndefined(const QString& undefClass)
{
// 	QMutexLocker l(&messageLocker);
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray classByte = undefClass.toUtf8();
	stream << (qint32) Simond::ModelCompilationClassUndefined
		<< (qint64) (classByte.count()+sizeof(qint32) /*seperator*/)
		<< classByte;
	write(toWrite);
}

void ClientSocket::slotModelCompilationPhonemeUndefined(const QString& phoneme)
{
// 	QMutexLocker l(&messageLocker);
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray phonemeByte = phoneme.toUtf8();
	stream << (qint32) Simond::ModelCompilationPhonemeUndefined
		<< (qint64) (phonemeByte.count()+sizeof(qint32) /*seperator*/)
		<< phonemeByte;
	write(toWrite);
}



void ClientSocket::recompileModel()
{
	sendCode(Simond::ModelCompilationStarted);
	
	QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
									 


	modelCompilationManager->startCompilation(activeDir+"hmmdefs", activeDir+"tiedlist",
				activeDir+"model.dict", activeDir+"model.dfa",
				KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
				synchronisationManager->getLexiconPath(), synchronisationManager->getGrammarPath(),
				synchronisationManager->getSimpleVocabPath(), synchronisationManager->getPromptsPath(), 
				synchronisationManager->getTreeHedPath(), synchronisationManager->getWavConfigPath());
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
	QString error= errorString()+"\n";
	QList<QSslError> sslErrors;
	sslErrors = this->sslErrors();
	for (int i=0; i < sslErrors.count(); i++)
		error+=sslErrors[i].errorString()+"\n";
	kDebug() << error;

// 	ignoreSslErrors();
}


		
bool ClientSocket::sendActiveModel()
{
	kDebug() << "Sending active model...";
	Q_ASSERT(synchronisationManager);
	
	Model *model = synchronisationManager->getActiveModel();
	
	if (!model) return false;
	
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	bodyStream << synchronisationManager->getActiveModelDate()
		<< model->sampleRate()
		<< model->hmmDefs()
		<< model->tiedList()
		<< model->dict()
		<< model->dfa();
			
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream 	<< (qint32) Simond::ActiveModel << (qint64) body.count();

	write(toWrite);
	write(body);
	
	delete model;
	
	return true;
}

void ClientSocket::synchronisationDone()
{
	kDebug() << "Synchronisation done";
	synchronisationRunning=false;
	//reset modelsource
	modelSource = ClientSocket::Undefined;
	
	Q_ASSERT(recognitionControl);
	
	if (!recognitionControl->isInitialized() && !modelCompilationManager->isRunning() && 
			synchronisationManager->hasActiveModel())
		recognitionControl->initializeRecognition(peerAddress() == QHostAddress::LocalHost);
}


void ClientSocket::synchronisationComplete()
{
	kDebug() << "Synchronisation complete";
	if (!synchronisationManager->commit())
	{
		kDebug() << "Synchronisation commit failed";
		sendCode(Simond::SynchronisationCommitFailed);
	} else {
		kDebug() << "Synchronisation succeded";
		sendCode(Simond::SynchronisationComplete);
	
		kDebug() << "Src Date: " << synchronisationManager->getCompileModelSrcDate();
		kDebug() << "Active Date: " << synchronisationManager->getActiveModelDate();
		if ((synchronisationManager->getActiveModelDate() < synchronisationManager->getCompileModelSrcDate()) && (synchronisationManager->hasModelSrc()))
			recompileModel();
	}
	
	synchronisationDone();
}

bool ClientSocket::sendWordList()
{
	Q_ASSERT(synchronisationManager);
	
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	WordListContainer *wordList = synchronisationManager->getWordList();
	if (!wordList) return false;
	
	bodyStream << synchronisationManager->getWordListDate()
		<< wordList->simpleVocab()
		<< wordList->activeVocab()
		<< wordList->activeLexicon();

	out << (qint32) Simond::WordList
		<< (qint64) body.count();

	write(toWrite);
	write(body);
	
	delete wordList;
	return true;
}


bool ClientSocket::sendGrammar()
{
	kDebug() << "Sending grammar...";
	Q_ASSERT(synchronisationManager);
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	GrammarContainer *grammar = synchronisationManager->getGrammar();
	if (!grammar) return false;
	
	bodyStream << synchronisationManager->getGrammarDate()
		<< grammar->grammarStructures();
	out << (qint32) Simond::Grammar
		<< (qint64) body.count();
	write(toWrite);
	write(body);
	
	kDebug() << "Done";
	delete grammar;
	return true;
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
		<< languageDescription->shadowVocab();

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


void ClientSocket::recognitionAwaitingStream(qint32 port, qint32 samplerate)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::RecognitionAwaitingStream << port << samplerate;
	write(toWrite);
}

void ClientSocket::recognitionError(const QString& error)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray errorByte = error.toUtf8();
	stream << (qint32) Simond::RecognitionError << (qint64) errorByte.count()+sizeof(qint32) /*separator*/ << errorByte;
	write(toWrite);
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
	sendCode(Simond::RecognitionStarted);
}

void ClientSocket::recognitionStopped()
{
	sendCode(Simond::RecognitionStopped);
}

void ClientSocket::recognitionPaused()
{
	sendCode(Simond::RecognitionPaused);
}

void ClientSocket::recognitionResumed()
{
	sendCode(Simond::RecognitionResumed);
}


void ClientSocket::sendRecognitionResult(const RecognitionResultList& recognitionResults)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	qint8 recognitionCount = recognitionResults.count();
	bodyStream << recognitionCount;
	
	for (int i=0; i < recognitionResults.count(); i++)
	{
		bodyStream << recognitionResults[i].sentence().toUtf8() 
			<< recognitionResults[i].sampa().toUtf8()
			<< recognitionResults[i].sampaRaw().toUtf8()
			<< recognitionResults[i].confidenceScores();
	}

	stream << (qint32) Simond::RecognitionResult << (qint64) body.count();
	write(toWrite);
	write(body);
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
}
