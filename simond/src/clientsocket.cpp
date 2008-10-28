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
#include "databaseaccess.h"
#include "juliuscontrol.h"

#include <speechmodelbase/model.h>
#include <speechmodelbase/wordlistcontainer.h>
#include <speechmodelbase/grammarcontainer.h>
#include <speechmodelbase/languagedescriptioncontainer.h>
#include <speechmodelbase/trainingcontainer.h>

#include <modelcompilation/modelcompilationmanager.h>

#include <QDir>
#include <QDateTime>

#include <KDebug>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KConfigGroup>

#include <KConfig>


ClientSocket::ClientSocket(int socketDescriptor, DatabaseAccess *databaseAccess, QObject *parent) : QSslSocket(parent)
{
	username ="";
	synchronisationRunning = false;

	Q_ASSERT(databaseAccess);

	this->databaseAccess = databaseAccess;

	synchronisationManager = 0;
	modelSource = ClientSocket::Undefined;
	
	modelCompilationManager=0;
	

	kDebug() << "Created ClientSocket with Descriptor " << socketDescriptor;

	this->setSocketDescriptor(socketDescriptor);
	connect(this, SIGNAL(readyRead()), this, SLOT(processRequest()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError()));


	recognitionControl = new JuliusControl(this);
	connect(recognitionControl, SIGNAL(recognitionReady()), this, SLOT(recognitionReady()));
	connect(recognitionControl, SIGNAL(recognitionError(const QString&)), this, SLOT(recognitionError(const QString&)));
	connect(recognitionControl, SIGNAL(recognitionWarning(const QString&)), this, SLOT(recognitionWarning(const QString&)));
	connect(recognitionControl, SIGNAL(recognitionStarted()), this, SLOT(recognitionStarted()));
	connect(recognitionControl, SIGNAL(recognitionStopped()), this, SLOT(recognitionStopped()));
	connect(recognitionControl, SIGNAL(recognitionTemporarilyUnavailable(const QString&)), this, SLOT(recognitionTemporarilyUnavailable(const QString&)));

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
		
		switch (request)
		{
			case Simond::Login:
			{
				kDebug() << "Login requested";
				waitForMessage(sizeof(qint8)+sizeof(qint64), stream, msg);

				qint8 remoteProtocolVersion;
				QString user;
				QByteArray userBytes;
				QString pass;
				QByteArray passBytes;
				
				qint64 length;
				
				stream >> remoteProtocolVersion;
				stream >> length;
				waitForMessage(length, stream, msg);
				
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
					//store authentication data
					this->username = user;
					
					if (modelCompilationManager) modelCompilationManager->deleteLater();
					
					QString srcDir = KStandardDirs::locateLocal("appdata", "models/"+user+"/src/");
					QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+user+"/active/");
					QString sampleDir = KStandardDirs::locateLocal("appdata", "models/"+user+"/samples/");
					modelCompilationManager = new ModelCompilationManager(user, sampleDir, srcDir+"lexicon", srcDir+"model.grammar", 
											srcDir+"simplevocab", srcDir+"prompts",
											srcDir+"tree1.hed", srcDir+"wav_config",
											activeDir+"hmmdefs", activeDir+"tiedlist",
											activeDir+"model.dict", activeDir+"model.dfa",
												this);
					connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(sendActiveModel()));
					connect(modelCompilationManager, SIGNAL(status(const QString&, int, int)), this, SLOT(slotModelCompilationStatus(const QString&, int, int)));
					connect(modelCompilationManager, SIGNAL(error(const QString&)), this, SLOT(slotModelCompilationError(const QString&)));
					
					sendCode(Simond::LoginSuccessful);
				} else {
					sendCode(Simond::AuthenticationFailed);
				}
				
				break;
				
			}
			
			case Simond::StartSynchronisation:
			{
				if (synchronisationRunning) break;
				
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				synchronisationRunning = true;
				if (!synchronisationManager)
					synchronisationManager = new SynchronisationManager(username, this);
				
				sendCode(Simond::GetActiveModelDate);
				break;
			}
			

			case Simond::ActiveModelDate:
			{
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
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				
				Q_ASSERT(synchronisationManager);
				
				kDebug() << "Received Active model";
				
				waitForMessage(sizeof(qint64), stream, msg);
				
				qint64 length;
				stream >> length;
				waitForMessage(length, stream, msg);
				
				int sampleRate;
				stream >> sampleRate;
				
				QByteArray hmmdefs, tiedlist, dict, dfa;
				QDateTime changedDate;
				stream >> changedDate;
				stream >> hmmdefs;
				stream >> tiedlist;
				stream >> dict;
				stream >> dfa;
				
				synchronisationManager->storeActiveModel( changedDate, sampleRate, hmmdefs,
					tiedlist, dict, dfa);
				
				break;
			}

			case Simond::ErrorRetrievingActiveModel:
			{
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				Q_ASSERT(synchronisationManager);
				
				kDebug() << "Client reported error during the retrieving of the active model";
				
				if (!sendActiveModel())
					sendCode(Simond::NoActiveModelAvailable);
				
				
				break;
			}

			case Simond::ModelSrcDate:
			{
				QDateTime remoteModelDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteModelDate;
				
				Q_ASSERT(synchronisationManager);
				
				if (remoteModelDate > synchronisationManager->getModelSrcDate())
				{
					modelSource = ClientSocket::Client;
				} else {
					if (remoteModelDate < synchronisationManager->getModelSrcDate())
						modelSource = ClientSocket::Server;
				}
					kDebug() << "Modelsource: " << modelSource;
				if (remoteModelDate != synchronisationManager->getModelSrcDate())
					sendCode(Simond::GetTrainingDate);
				else
					synchronisationComplete();
				
				break;
			}
			
			
			case Simond::TrainingDate:
			{
				QDateTime remoteTrainingDate;
				waitForMessage(sizeof(QDateTime), stream, msg);
				stream >> remoteTrainingDate;
				
				Q_ASSERT(synchronisationManager);
				Q_ASSERT(modelSource != ClientSocket::Undefined);
				
				if (remoteTrainingDate != synchronisationManager->getTrainingDate())
				{
					//Training changed
					if (modelSource == ClientSocket::Server)
					{
						if (!sendTraining())
							sendCode(Simond::GetTraining);
					} else sendCode(Simond::GetTraining);
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
				
				int sampleRate;
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
				
				kDebug() << remoteWordListDate << synchronisationManager->getWordListDate();
				
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
				
				
				if (!synchronisationManager->storeGrammar(changedTime, grammar))
				{
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
				
				
				if (remoteLanguageDescriptionDate != synchronisationManager->getLanguageDescriptionDate())
				{
					//LanguageDescription changed
					if (modelSource == ClientSocket::Server)
					{
						if (!sendLanguageDescription())
							sendCode(Simond::GetLanguageDescription);
					} else sendCode(Simond::GetLanguageDescription);
				} else {
					kDebug() << "LanguageDescription is up-to-date";
					fetchTrainingSample();
				}
				
				break;
			}
			
			
			case Simond::ErrorRetrievingLanguageDescription:
			{
				kDebug() << "Couldn't get languagedescription";
				if (!synchronisationManager->hasLanguageDescription())
				{
					sendCode(Simond::NoLanguageDescriptionAvailable);
					synchronisationDone();
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
				
				QByteArray treeHed, shadowVocab, shadowLexicon;
				QDateTime changedTime;
				stream >> changedTime;
				stream >> treeHed;
				stream >> shadowVocab;
				stream >> shadowLexicon;
				
				if (!synchronisationManager->storeLanguageDescription(changedTime, shadowVocab, shadowLexicon, treeHed))
				{
					sendCode(Simond::LanguageDescriptionStorageFailed);
				} else fetchTrainingSample();
				break;
			}
			

			case Simond::GetTrainingsSample:
			{
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				
				Q_ASSERT(synchronisationManager);
				
				
				break;
			}

			case Simond::ErrorRetrievingTrainingsSample:
			{
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				
				Q_ASSERT(synchronisationManager);
				
				
				break;
			}

			case Simond::TrainingsSample:
			{
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				
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

			case Simond::StartRecognition:
			{
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				
				
				break;
			}

			case Simond::StopRecognition:
			{
				if (username.isEmpty()) {
					sendCode(Simond::AccessDenied);
					break;
				}
				
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

void ClientSocket::fetchTrainingSample()
{
	Q_ASSERT(synchronisationManager);
	
	QString sample = synchronisationManager->missingSample();
	if (sample.isNull())
	{
		kDebug() << "Done fetching samples";
		synchronisationComplete();
		return;
	}
	
	kDebug() << "Fetching sample " << sample;

	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::GetTrainingsSample
		<< (qint64) sample.count()
		<< sample;
	write(toWrite);
}


void ClientSocket::slotModelCompilationStatus(const QString& status, int progressNow, int progressMax)
{
	QByteArray toWrite;
	QByteArray statusByte = status.toUtf8();
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::ModelCompilationStatus
		<< (qint64) (statusByte.count()+sizeof(int)+sizeof(int))
		<< progressNow
		<< progressMax
		<< statusByte;
		
	write(toWrite);
	kDebug() << progressNow << progressMax << status;
}

void ClientSocket::slotModelCompilationError(const QString& error)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray errorByte = error.toUtf8();
	stream << (qint32) Simond::ModelCompilationError
		<< (qint64) errorByte.count()
		<< errorByte;
		
	write(toWrite);
	kDebug() << error;
}


void ClientSocket::recompileModel()
{
	kDebug() << "Compiling model...";
	modelCompilationManager->startCompilation();
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
// 	Logger::log(i18n("[ERR] %1", error));
}


		
bool ClientSocket::sendActiveModel()
{
	Q_ASSERT(synchronisationManager);
	
	if (username.isEmpty()) return false;
	
	Model *model = synchronisationManager->getActiveModel();
	
	if (!model) return false;
	
	
	qint32 size = model->hmmDefs().count()+
			model->tiedList().count()+
			model->dict().count()+
			model->dfa().count()+
			sizeof(Simond::Request)+sizeof(QDateTime)+
			sizeof(int)+sizeof(int);
			
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::ActiveModel
		<< size
		<< synchronisationManager->getActiveModelDate()
		<< model->sampleRate()
		<< model->hmmDefs()
		<< model->tiedList()
		<< model->dict()
		<< model->dfa();
	write(toWrite);
	
	delete model;
	
	return true;
}

void ClientSocket::synchronisationDone()
{
	synchronisationRunning=false;
	//reset modelsource
	modelSource = ClientSocket::Undefined;
	
	//send queued messages
	if (messagesToSend.count() > 0)
	{
		write(messagesToSend.takeAt(0));
	}
}


void ClientSocket::synchronisationComplete()
{
	sendCode(Simond::SynchronisationComplete);
	
	if (modelSource == ClientSocket::Client)
		//if we got our model from the client modelsrc has changed
		recompileModel();
	
	synchronisationDone();
}

bool ClientSocket::sendWordList()
{
	Q_ASSERT(synchronisationManager);
	
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	WordListContainer *wordList = synchronisationManager->getWordList();
	if (!wordList) return false;
	
	qint32 size = wordList->simpleVocab().count()+
			wordList->activeVocab().count()+
			wordList->activeLexicon().count()+
			sizeof(QDateTime);
			
	out << Simond::WordList
		<< size
		<< synchronisationManager->getWordListDate()
		<< wordList->simpleVocab()
		<< wordList->activeVocab()
		<< wordList->activeLexicon();
	write(toWrite);
	
	delete wordList;
	return true;
}


bool ClientSocket::sendGrammar()
{
	kDebug() << "Sending grammar";
	Q_ASSERT(synchronisationManager);
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	GrammarContainer *grammar = synchronisationManager->getGrammar();
	if (!grammar) return false;
	
	qint32 size = grammar->grammarStructures().count()+
			sizeof(QDateTime);
	
	out << Simond::Grammar
		<< size
		<< synchronisationManager->getGrammarDate()
		<< grammar->grammarStructures();
	write(toWrite);
	
	delete grammar;
	return true;
}


bool ClientSocket::sendLanguageDescription()
{
	Q_ASSERT(synchronisationManager);
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	LanguageDescriptionContainer *languageDescription = synchronisationManager->getLanguageDescription();
	if (!languageDescription) return false;
	
	qint64 size = languageDescription->treeHed().count()+languageDescription->shadowVocab().count()+
			languageDescription->shadowLexicon().count()+sizeof(QDateTime);
	
	out << Simond::LanguageDescription
		<< size
		<< synchronisationManager->getGrammarDate()
		<< languageDescription->treeHed()
		<< languageDescription->shadowVocab()
		<< languageDescription->shadowLexicon();
	write(toWrite);
	
	delete languageDescription;
	return true;
}


bool ClientSocket::sendTraining()
{
	Q_ASSERT(synchronisationManager);
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	TrainingContainer *training = synchronisationManager->getTraining();
	if (!training) return false;
	
	qint64 size = sizeof(int)+sizeof(int)+
			training->wavConfig().count()+
			training->prompts().count()+sizeof(QDateTime);
	
	out << Simond::Training
		<< size
		<< synchronisationManager->getTrainingDate()
		<< training->sampleRate()
		<< training->wavConfig()
		<< training->prompts();
	write(toWrite);
	
	delete training;
	return true;
}


void ClientSocket::recognitionReady()
{
	sendCode(Simond::RecognitionReady);
}

void ClientSocket::recognitionError(const QString& error)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray errorByte = error.toUtf8();
	stream << (qint32) Simond::RecognitionError << (qint64) errorByte.count() << errorByte;
	write(toWrite);
}

void ClientSocket::recognitionWarning(const QString& warning)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	QByteArray warningByte = warning.toUtf8();
	stream << (qint32) Simond::RecognitionError << (qint64) warningByte.count() << warningByte;
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

void ClientSocket::recognitionTemporarilyUnavailable(const QString& reason)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::RecognitionTemporarilyUnavailable << reason.toUtf8();
	write(toWrite);
}


void ClientSocket::sendRecognitionResult(const QString& data, const QString& sampa, const QString& samparaw)
{
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	
	QByteArray dataByte = data.toUtf8();
	QByteArray sampaByte = sampa.toUtf8();
	QByteArray sampaRawByte = samparaw.toUtf8();
	
	qint64 size = dataByte.count() + sampaByte.count() + sampaRawByte.count();
	
	stream << Simond::RecognitionResult << size << dataByte << sampaByte << sampaRawByte;
	write(toWrite);
}

ClientSocket::~ClientSocket()
{
	//leave databaseAccess alone since it is shared
	if (recognitionControl) recognitionControl->deleteLater();
	if (synchronisationManager) synchronisationManager->deleteLater();
	if (modelCompilationManager) modelCompilationManager->deleteLater();
}
