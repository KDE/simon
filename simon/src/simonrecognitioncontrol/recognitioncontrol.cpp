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


#include "recognitioncontrol.h"
#include <simoninfo/simoninfo.h>
#include <simonprotocol/simonprotocol.h>
#include <speechmodelmanagement/modelmanager.h>
// #include <speechmodelbase/modelcontainer.h>
#include <speechmodelbase/wordlistcontainer.h>
#include <speechmodelbase/grammarcontainer.h>
#include <speechmodelbase/languagedescriptioncontainer.h>
#include <speechmodelbase/trainingcontainer.h>
#include <speechmodelbase/model.h>
#include "recognitionconfiguration.h"

#include <QByteArray>
#include <QSslSocket>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QStringList>
#include <KMessageBox>
#include <KLocalizedString>
#include <KDebug>
#include <KPasswordDialog>

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
RecognitionControl::RecognitionControl(QWidget *parent) : QObject(parent)
{
	socket = new QSslSocket();
	timeoutWatcher = new QTimer(this);
	connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
			
	connect(socket, SIGNAL(readyRead()), this, SLOT(messageReceived()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()));
	connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured()));


	connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	
	connect(this, SIGNAL(error(QString, bool)), this, SLOT(disconnectFromServer()));


	if ( RecognitionConfiguration::juliusdAutoConnect() )
		startConnecting();
	
	this->modelManager = new ModelManager();
}

void RecognitionControl::startConnecting()
{
	if (socket->state() != QAbstractSocket::UnconnectedState)
		disconnectFromServer();

	serverConnectionErrors.clear();
	serverConnectionsToTry = RecognitionConfiguration::juliusdServers();
	
	if (serverConnectionsToTry.count() == 0) return;
	
	QStringList address = serverConnectionsToTry.takeAt(0).split(":");
	connectTo(address[0], address[1].toInt());
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

	if (RecognitionConfiguration::juliusdEncrypted())
	{
		socket->setProtocol(QSsl::TlsV1);
		connect(socket, SIGNAL(encrypted()), this, SLOT(connectedTo()));
		socket->connectToHostEncrypted( server, port );
	} else {
		connect(socket, SIGNAL(connected()), this, SLOT(connectedTo()));
		socket->connectToHost( server, port );
	}
	timeoutWatcher->start(RecognitionConfiguration::juliusdConnectionTimeout());
	
}

void RecognitionControl::errorOccured()
{
	if (socket->state()== QAbstractSocket::UnconnectedState) return;
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	QList<QSslError> errors = socket->sslErrors();
	if ((errors.count() == 1) && (errors[0].error() == QSslError::SelfSignedCertificate) && (RecognitionConfiguration::juliusdEncrypted()))
	{
		if (KMessageBox::questionYesNoCancel(0, i18n("Das Zertifikat der Gegenstelle ist selbst-signiert und nicht vertrauenswürdig.\n\nWollen Sie die Verbindung trozdem fortsetzen?"), i18n("Selbst-Signiertes Zertifikat"))==KMessageBox::Yes)
		{
			socket->ignoreSslErrors();
			return;
		} else {
			socket->abort();
			socket->flush();
			socket->close();
		}
	}
	if (socket->error() != QAbstractSocket::UnknownSocketError)
		serverConnectionErrors << socket->errorString();
	else 
	{ //build ssl error list
		for (int i=0; i < errors.count(); i++)
			serverConnectionErrors << errors[i].errorString();
	}

	if (serverConnectionsToTry.isEmpty())
		emit connectionError(serverConnectionErrors.join("\n"));
	else {
		QStringList address = serverConnectionsToTry.takeAt(0).split(":");
		connectTo(address[0], address[1].toInt());
	}
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
	timeoutWatcher->stop();
	emit connectionError(i18n("Zeitüberschreitung der Anforderung (%1 ms)", RecognitionConfiguration::juliusdConnectionTimeout()));
	socket->abort();
}

/**
 *	@brief Disconnects the socket from julius
 *	
 *	@author Peter Grasch
 */
void RecognitionControl::disconnectFromServer()
{
	serverConnectionsToTry.clear();
	
	if (timeoutWatcher->isActive())
		timeoutWatcher->stop();
	
	this->socket->disconnectFromHost();
	if ((socket->state() != QAbstractSocket::UnconnectedState) &&
		     (!socket->waitForDisconnected(1000)))
		this->socket->abort();
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
	socket->write(toWrite);
}

void RecognitionControl::login()
{
	QString user = RecognitionConfiguration::juliusdUsername();
	QString pass = RecognitionConfiguration::juliusdPassword();

	if (user.isEmpty())
	{
		KPasswordDialog dlg( dynamic_cast<QWidget*>(parent()), 
					KPasswordDialog::ShowUsernameLine|KPasswordDialog::ShowKeepPassword );
		dlg.setPrompt( i18n( "Bitte geben Sie die Benutzerdaten für die Verbindung zum Juliusd an." ));
		if( !dlg.exec() || dlg.username().isEmpty() )
		{
			disconnectFromServer(); //the user canceled
			return;
		}
		user = dlg.username();
		pass = dlg.password();

		if (dlg.keepPassword())
		{
			RecognitionConfiguration::setJuliusdUsername(user);
			RecognitionConfiguration::setJuliusdPassword(pass);
		}
	}


	QByteArray userBytes = user.toUtf8();
	QByteArray passBytes = QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Sha1);
	
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	qint64 size = (userBytes.count()+passBytes.count()+sizeof(qint8)+sizeof(Simond::Request));
	out << Simond::Login << protocolVersion << size << userBytes << passBytes;
	socket->write(toWrite);
}

void RecognitionControl::sendActiveModelModifiedDate()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::ActiveModelDate
		<< modelManager->getActiveContainerModifiedTime();
	socket->write(toWrite);
}
	
bool RecognitionControl::sendActiveModel()
{
	Model *model = modelManager->createActiveContainer();
	if (!model) {
		emit warning(i18n("Konnte Modellcontainer nicht erstellen"));
		sendRequest(Simond::ErrorRetrievingActiveModel);
		return false;
	}
	
	
	qint64 size = sizeof(int)+sizeof(int)
			+ model->hmmDefs().count()
			+ model->tiedList().count()
			+ model->dict().count()
			+ model->dfa().count()
			+ sizeof(QDateTime);
	
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::ActiveModel
		<< size
		<< modelManager->getActiveContainerModifiedTime()
		<< model->sampleRate()
		<< model->hmmDefs()
		<< model->tiedList()
		<< model->dict()
		<< model->dfa();
		
	socket->write(toWrite);
	
	delete model;
	return true;
}


void RecognitionControl::sendModelSrcModifiedDate()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::ModelSrcDate
		<< modelManager->getSrcContainerModifiedTime();
	socket->write(toWrite);
}


void RecognitionControl::sendWordListModifiedDate()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::WordListDate
		<< modelManager->getWordListModifiedTime();
	socket->write(toWrite);
}

void RecognitionControl::sendWordList()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	WordListContainer *wordList = modelManager->getWordListContainer();
	
	qint64 size = wordList->simpleVocab().count()+
			wordList->activeVocab().count()+
			wordList->activeLexicon().count()+
			sizeof(qint64)+sizeof(QDateTime);
			
	out << Simond::WordList
		<< size
		<< modelManager->getWordListModifiedTime()
		<< wordList->simpleVocab()
		<< wordList->activeVocab()
		<< wordList->activeLexicon();
	socket->write(toWrite);
	
	delete wordList;
}



void RecognitionControl::sendGrammarModifiedDate()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::GrammarDate
		<< modelManager->getGrammarModifiedTime();
	socket->write(toWrite);
}


void RecognitionControl::sendGrammar()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	GrammarContainer *grammar = modelManager->getGrammarContainer();
	
	qint64 size = grammar->grammarStructures().count()+
			sizeof(QDateTime);
	
	out << Simond::Grammar
		<< size
		<< modelManager->getGrammarModifiedTime()
		<< grammar->grammarStructures();
	socket->write(toWrite);
	
	delete grammar;
}



void RecognitionControl::sendLanguageDescriptionModifiedDate()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::LanguageDescriptionDate
		<< modelManager->getLanguageDescriptionModifiedTime();
	socket->write(toWrite);
}


void RecognitionControl::sendLanguageDescription()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	LanguageDescriptionContainer *languageDescription = modelManager->getLanguageDescriptionContainer();
	
	qint64 size = languageDescription->treeHed().count()+languageDescription->shadowVocab().count()+
			languageDescription->shadowLexicon().count()+sizeof(QDateTime);
	
	out << Simond::LanguageDescription
		<< size
		<< modelManager->getLanguageDescriptionModifiedTime()
		<< languageDescription->treeHed()
		<< languageDescription->shadowVocab()
		<< languageDescription->shadowLexicon();
	socket->write(toWrite);
	
	delete languageDescription;
}



void RecognitionControl::sendTrainingModifiedDate()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::TrainingDate
		<< modelManager->getTrainingModifiedTime();
	socket->write(toWrite);
}


void RecognitionControl::sendTraining()
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	TrainingContainer *training = modelManager->getTrainingContainer();
	
	qint64 size = sizeof(int)+sizeof(int)+
			training->wavConfig().count()+
			training->prompts().count()+
			sizeof(QDateTime);
	
	out << Simond::Training
		<< size
		<< modelManager->getTrainingModifiedTime()
		<< training->sampleRate()
		<< training->wavConfig()
		<< training->prompts();
	socket->write(toWrite);
	
	delete training;
}

void RecognitionControl::synchronizeSamples()
{
	Q_ASSERT(modelManager);
	
	modelManager->buildMissingSamplesList();
	fetchMissingSamples();
}

void RecognitionControl::fetchMissingSamples()
{
	Q_ASSERT(modelManager);
	
	QString sample = modelManager->missingSample();
	if (sample.isNull())
	{
		kDebug() << "Done fetching samples";
		synchronisationComplete();
		return;
	}
	
	QByteArray sampleByte = sample.toUtf8();
	
	kDebug() << "Fetching sample " << sample;

	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::GetTrainingsSample
		<< (qint64) sampleByte.count()
		<< sampleByte;
	socket->write(toWrite);
}


void RecognitionControl::sendSample(QString sampleName)
{
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	
	sampleName += ".wav";
	
	QByteArray sample = modelManager->getSample(sampleName);
	
	if (sample.isNull())
	{
		emit warning(i18n("Konnte \"%1\" nicht übertragen", sampleName));
		sendRequest(Simond::ErrorRetrievingTrainingsSample);
		return;
	}

	qint64 size = sample.count();
	
	out << Simond::TrainingsSample
		<< size
		<< sample;
	socket->write(toWrite);
}




void RecognitionControl::startSynchronisation()
{
	kDebug() << "Starting synchronisation";
	disconnect (modelManager, SIGNAL(modelChanged()), this, SLOT(startSynchronisation()));
	sendRequest(Simond::StartSynchronisation);
}

void RecognitionControl::synchronisationComplete()
{//successful
	kDebug() << "Synchronisation completed";
	synchronisationDone();
}

void RecognitionControl::synchronisationDone()
{
	kDebug() << "Finishing up synchronisation";
	
	if (RecognitionConfiguration::automaticSync())
		connect(modelManager, SIGNAL(modelChanged()), this, SLOT(startSynchronisation()));
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
	QByteArray msgByte = socket->readAll();
	QDataStream msg(&msgByte, QIODevice::ReadOnly);
	qint32 type;
	Simond::Request request;
	
	while (!msg.atEnd())
	{
		msg >> type;
		request = (Simond::Request) type;
		kDebug() << type;
		switch (request)
		{
			////////////////////    LOGIN    //////////////////////////////////////

			case Simond::LoginSuccessful:
			{
				emit loggedIn();
				startSynchronisation();
				break;
			}

			case Simond::VersionIncompatible:
			{
				emit error(i18n("Version nicht unterstützt"));
				break;
			}

			case Simond::AuthenticationFailed:
			{
				emit error(i18n("Benutzername oder Passwort falsch."));
				this->disconnectFromServer();
				break;
			}

			case Simond::AccessDenied:
			{
				emit error(i18n("Zugriff verweigert."));
				this->disconnectFromServer();
				break;
			}



			////////////////////    SYNCHRONISATION    ////////////////////////////
			

			case Simond::GetActiveModelDate:
			{
				kDebug() << "Server requested active Model modified date";
				sendActiveModelModifiedDate();
				break;
			}

			case Simond::GetActiveModel:
			{
				kDebug() << "Server requested active Model";
				sendActiveModel();
				break;
			}

			case Simond::ActiveModel:
			{
				kDebug() << "Server sent active Model";
				
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				int sampleRate;
				QByteArray hmmDefs, tiedList, dict, dfa;
				
				QDateTime changedTime;
				msg >> changedTime;
				msg >> sampleRate;
				msg >> hmmDefs;
				msg >> tiedList;
				msg >> dict;
				msg >> dfa;
				
				modelManager->storeActiveModel(changedTime, sampleRate, 
								hmmDefs, tiedList, dict, dfa);
				
				sendModelSrcModifiedDate();
				break;
			}

			case Simond::NoActiveModelAvailable:
			{
				kDebug() << "No active model available";
				emit warning(i18n("Kein Sprachmodell verfügbar: Erkennung deaktiviert"));
				sendModelSrcModifiedDate();
				
				break;
			}

			case Simond::ActiveModelStorageFailed:
			{
				kDebug() << "Couldn't store active model on server";
				emit warning(i18n("Konnte Server-Modell nicht aktualisieren"));
				sendModelSrcModifiedDate();
				
				break;
			}
			
			

			case Simond::GetModelSrcDate:
			{
				kDebug() << "Server requested Model src modified date";
				sendModelSrcModifiedDate();
				break;
			}
			
			
			
			case Simond::GetTrainingDate:
			{
				kDebug() << "Server Requested training modified date";
				sendTrainingModifiedDate();
				break;
			}
			
			case Simond::GetTraining:
			{
				kDebug() << "Server requested training";
				sendTraining();
				break;
			}
			
			case Simond::Training:
			{
				kDebug() << "Server sent training";
				waitForMessage(sizeof(qint64), msg, msgByte);
				
				qint64 length;
				msg >> length;
				
				waitForMessage(length, msg, msgByte);
				
				int sampleRate;
				QByteArray wavConfig, prompts;
				
				QDateTime changedTime;
				msg >> changedTime;
				msg >> sampleRate;
				msg >> wavConfig;
				msg >> prompts;
				
				modelManager->storeTraining(changedTime, sampleRate,wavConfig,prompts);
				
				sendWordListModifiedDate();
				break;
			}
			
			case Simond::NoTrainingAvailable:
			{
				kDebug() << "No training available";
				sendWordListModifiedDate();
				break;
			}
			
			case Simond::TrainingStorageFailed:
			{
				kDebug() << "Server could not store training";
				sendWordListModifiedDate();
				break;
			}
			
			
			case Simond::GetWordListDate:
			{
				kDebug() << "Server Requested WordList modified date";
				sendWordListModifiedDate();
				break;
			}
			
			case Simond::GetWordList:
			{
				kDebug() << "Server Requested WordList";
				sendWordList();
				break;
			}
			
			case Simond::WordList:
			{
				kDebug() << "Server sent WordList";
				
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				
				waitForMessage(length, msg, msgByte);
				
				QByteArray simple, vocab, lexicon;
				QDateTime changedTime;
				msg >> changedTime;
				msg >> simple;
				msg >> vocab;
				msg >> lexicon;
				
				modelManager->storeWordList(changedTime,simple,vocab,lexicon);
				
				sendGrammarModifiedDate();
				break;
			}
			
			case Simond::NoWordListAvailable:
			{
				kDebug() << "No wordlist available";
				emit warning(i18n("Keine Wortliste verfügbar"));
				sendGrammarModifiedDate();
				break;
			}
			
			case Simond::WordListStorageFailed:
			{
				kDebug() << "Server could not store wordlist";
				emit warning(i18n("Konnte Wortliste nicht auf Server ablegen"));
				sendGrammarModifiedDate();
				break;
			}
			
			
			
			case Simond::GetGrammarDate:
			{
				kDebug() << "Server Requested Grammar modified date";
				sendGrammarModifiedDate();
				break;
			}
			
			case Simond::GetGrammar:
			{
				kDebug() << "Server requested Grammar";
				sendGrammar();
				break;
			}
			
			case Simond::Grammar:
			{
				kDebug() << "Server sent grammar";
				
				waitForMessage(sizeof(qint64), msg, msgByte);
				
				qint64 length;
				msg >> length;
				
				waitForMessage(length, msg, msgByte);
				
				QByteArray grammar;
				QDateTime changedTime;
				msg >> changedTime;
				msg >> grammar;
				
				modelManager->storeGrammar(changedTime,grammar);
				
				sendLanguageDescriptionModifiedDate();
				break;
			}
			
			case Simond::NoGrammarAvailable:
			{
				kDebug() << "No grammar available";
				sendLanguageDescriptionModifiedDate();
				break;
			}
			
			case Simond::GrammarStorageFailed:
			{
				kDebug() << "Server could not store grammar";
				sendLanguageDescriptionModifiedDate();
				break;
			}
			
			
			
			case Simond::GetLanguageDescriptionDate:
			{
				kDebug() << "Server Requested lang. desc. modified date";
				sendLanguageDescriptionModifiedDate();
				break;
			}
			
			case Simond::GetLanguageDescription:
			{
				kDebug() << "Server requested lang. desc.";
				sendLanguageDescription();
				break;
			}
			
			case Simond::LanguageDescription:
			{
				kDebug() << "Server sent languagedescription";
				
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QByteArray treeHed, shadowVocab, shadowLexicon;
				QDateTime changedTime;
				msg >> changedTime;
				msg >> treeHed;
				msg >> shadowVocab;
				msg >> shadowLexicon;
				
				modelManager->storeLanguageDescription(changedTime,shadowVocab, shadowLexicon, treeHed);
				synchronizeSamples();
				break;
			}
			
			case Simond::NoLanguageDescriptionAvailable:
			{
				kDebug() << "No languagedescription available";
				synchronizeSamples();
				break;
			}
			
			case Simond::LanguageDescriptionStorageFailed:
			{
				kDebug() << "Server could not store languagedescription";
				synchronizeSamples();
				break;
			}

			case Simond::GetTrainingsSample:
			{
				kDebug() << "Server requested Trainings-Sample";
				
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QByteArray sampleNameByte;
				msg >> sampleNameByte;
				
				sendSample(QString::fromUtf8(sampleNameByte));
				break;
			}

			case Simond::TrainingsSample:
			{
				kDebug() << "Server sent Trainings-Sample";
				
				Q_ASSERT(modelManager);
				
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QByteArray sample;
				msg >> sample;
				
				if (!modelManager->storeSample(sample))
				{
					sendRequest(Simond::TrainingsSampleStorageFailed);
					synchronisationDone();
				} else
					fetchMissingSamples();
				break;
			}
			
			
			case Simond::TrainingsSampleStorageFailed:
			{
				kDebug() << "Server could not store trainings-sample";
				synchronisationDone();
				break;
			}


			case Simond::SynchronisationComplete:
			{
				synchronisationComplete();
				break;
			}
			
			
			////////////////////    COMPILATION    ////////////////////////////////



			case Simond::ModelCompilationStarted: {
				emit status(i18n("Modell wird erstellt"));
				break;
			}
			case Simond::ModelCompilationStatus: {
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				int progNow, progMax;
				msg >> progNow;
				msg >> progMax;
				
				QString statusMsg;
				QByteArray statusByte;
				msg >> statusByte;
				statusMsg = QString::fromUtf8(statusByte);
				
				emit status(i18n("Modell: %1", statusMsg), progNow, progMax);
				break;
			}
			
			case Simond::ModelCompilationError: {
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QString errorMsg;
				QByteArray errorByte;
				msg >> errorByte;
				errorMsg = QString::fromUtf8(errorByte);
				
				emit warning(errorMsg);
				break;
			}

			////////////////////    RECOGNITION    ////////////////////////////////

			case Simond::RecognitionReady:
			{
				emit recognitionStatusChanged(RecognitionControl::Ready);
				break;
			}

			case Simond::RecognitionError:
			{
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QByteArray errormsgByte;
				msg >> errormsgByte;
				QString errormsg = QString::fromUtf8(errormsgByte);
				emit warning(i18n("Erkennungsfehler: %1", errormsg));
				emit recognitionStatusChanged(RecognitionControl::TemporarilyUnavailable);
				break;
			}

			case Simond::RecognitionWarning:
			{
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QByteArray warningmsgByte;
				msg >> warningmsgByte;
				QString warningmsg = QString::fromUtf8(warningmsgByte);
				emit warning(i18n("Erkennungswarnung: ")+warningmsg);
				break;
			}

			case Simond::RecognitionStarted:
			{
				emit recognitionStatusChanged(RecognitionControl::Started);
				break;
			}

			case Simond::RecognitionStopped:
			{
				emit recognitionStatusChanged(RecognitionControl::Stopped);
				break;
			}

			case Simond::RecognitionTemporarilyUnavailable:
			{
				emit recognitionStatusChanged(RecognitionControl::TemporarilyUnavailable);
				break;
			}

			case Simond::RecognitionResult:
			{
				waitForMessage(sizeof(qint64), msg, msgByte);
				qint64 length;
				msg >> length;
				waitForMessage(length, msg, msgByte);
				
				QByteArray word, sampa, samparaw;
				msg >> word;
				msg >> sampa;
				msg >> samparaw;
				emit recognised(QString::fromUtf8(word), QString::fromUtf8(sampa), QString::fromUtf8(samparaw));
				break;
			}
			
			default:
			{
				kDebug() << "Unknown request";
			}
		}
		if (socket->bytesAvailable())
			msgByte += socket->readAll();
	}
}



void RecognitionControl::waitForMessage(qint64 length, QDataStream& stream, QByteArray& message)
{
	Q_ASSERT(stream.device());
	while (stream.device()->bytesAvailable() < length)
	{
		if (socket->waitForReadyRead())
			message += socket->readAll();
	}
}


void RecognitionControl::startRecognition()
{
	sendRequest(Simond::StartRecognition);
}

void RecognitionControl::stopRecognition()
{
	sendRequest(Simond::StopRecognition);
}


/**
 *	@brief Destructor
 *	
 *	@author Peter Grasch
 */
RecognitionControl::~RecognitionControl()
{
    socket->deleteLater();
    timeoutWatcher->deleteLater();
    
    delete modelManager;
}


