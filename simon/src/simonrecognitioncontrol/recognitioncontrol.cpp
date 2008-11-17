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

#include <stdio.h>

#include "recognitioncontrol.h"
#include <simoninfo/simoninfo.h>
#include <simonprotocol/simonprotocol.h>
#include <speechmodelmanagement/modelmanager.h>
#include <speechmodelbase/wordlistcontainer.h>
#include <speechmodelbase/grammarcontainer.h>
#include <speechmodelbase/languagedescriptioncontainer.h>
#include <speechmodelbase/trainingcontainer.h>
#include <speechmodelbase/model.h>
#include <simonprogresstracking/operation.h>
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

#define advanceStream(x) qint64 currentPos = msg.device()->pos()-x;\
					msgByte.remove(0,x);\
					msg.device()->seek(currentPos);
					
#define checkIfSynchronisationIsAborting() if (synchronisationOperation->aborting()) \
					{ \
						sendRequest(Simond::AbortSynchronisation); \
						synchronisationDone(); \
						return; \
					}
					
#define checkIfMessageFinished(length)	if ((unsigned) msg.device()->bytesAvailable() < length) \
					{ \
						messageNotYetFinished=true; \
						break; \
					}
					
#define parseLengthHeader()		checkIfMessageFinished(sizeof(qint64)); \
					qint64 length; \
					msg >> length; \
					checkIfMessageFinished(length);

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
	recognitionReady=false;

	synchronisationOperation=NULL;
	modelCompilationOperation=NULL;

	socket = new QSslSocket();
	timeoutWatcher = new QTimer(this);
	connect(timeoutWatcher, SIGNAL(timeout()), this, SLOT(timeoutReached()));
			
	connect(socket, SIGNAL(readyRead()), this, SLOT(messageReceived()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccured()));
	connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured()));


	connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	
	connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
	connect(this, SIGNAL(simondSystemError(const QString&)), this, SLOT(disconnectFromServer()));


	if ( RecognitionConfiguration::juliusdAutoConnect() )
		startConnecting();
	
	this->modelManager = new ModelManager();
}

void RecognitionControl::slotDisconnected()
{
	recognitionReady=false;
	if (synchronisationOperation)
	{
		if (synchronisationOperation->isRunning())
			synchronisationOperation->canceled();
		synchronisationOperation = NULL;
	}
	if (modelCompilationOperation)
	{
		if (modelCompilationOperation->isRunning())
			modelCompilationOperation->canceled();
		modelCompilationOperation = NULL;
	}
}

void RecognitionControl::startConnecting()
{
	if (socket->state() != QAbstractSocket::UnconnectedState)
		disconnectFromServer();

	serverConnectionErrors.clear();
	serverConnectionsToTry = RecognitionConfiguration::juliusdServers();
	
	if (serverConnectionsToTry.count() == 0) return;
	kDebug() << serverConnectionsToTry;
	
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
	if (synchronisationOperation)
	{
		synchronisationOperation->canceled();
		synchronisationOperation->deleteLater();
		synchronisationOperation=NULL;
	}
	if (modelCompilationOperation)
	{
		modelCompilationOperation->canceled();
		modelCompilationOperation->deleteLater();
		modelCompilationOperation=NULL;
	}
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
		emit synchronisationWarning(i18n("Konnte Modellcontainer nicht erstellen"));
		sendRequest(Simond::ErrorRetrievingActiveModel);
		return false;
	}
	
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);

	bodyStream << modelManager->getActiveContainerModifiedTime()
		<< model->sampleRate()
		<< model->hmmDefs()
		<< model->tiedList()
		<< model->dict()
		<< model->dfa();

	out << (qint32) Simond::ActiveModel
		<< (qint64) body.count();

		
	socket->write(toWrite);
	socket->write(body);
	
	delete model;
	return true;
}


void RecognitionControl::sendActiveModelSampleRate()
{
	int smpFreq = modelManager->getActiveModelSampleRate();
	
	QByteArray toWrite;
	QDataStream out(&toWrite, QIODevice::WriteOnly);
	out << Simond::ActiveModelSampleRate
		<< smpFreq;
		
	socket->write(toWrite);
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
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	WordListContainer *wordList = modelManager->getWordListContainer();
	
	bodyStream << modelManager->getWordListModifiedTime()
		<< wordList->simpleVocab()
		<< wordList->activeVocab()
		<< wordList->activeLexicon();

	out << Simond::WordList
		<< (qint64) body.count();

	socket->write(toWrite);
	socket->write(body);
	
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
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	GrammarContainer *grammar = modelManager->getGrammarContainer();
	
	bodyStream << modelManager->getGrammarModifiedTime()
		<< grammar->grammarStructures();

	out << Simond::Grammar
		<< (qint64) body.count();

	socket->write(toWrite);
	socket->write(body);
	
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
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	LanguageDescriptionContainer *languageDescription = modelManager->getLanguageDescriptionContainer();
	
	bodyStream << modelManager->getLanguageDescriptionModifiedTime()
		<< languageDescription->treeHed()
		<< languageDescription->shadowVocab();

	out << Simond::LanguageDescription
		<< (qint64) body.count();
		
	socket->write(toWrite);
	socket->write(body);
	socket->waitForBytesWritten(body.count());
	
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
	QByteArray body;
	QDataStream bodyStream(&body, QIODevice::WriteOnly);
	
	TrainingContainer *training = modelManager->getTrainingContainer();
	
	bodyStream << modelManager->getTrainingModifiedTime()
		<< training->sampleRate()
		<< training->wavConfig()
		<< training->prompts();
	
	out << Simond::Training
		<< (qint64) body.count();

	socket->write(toWrite);
	socket->write(body);
	
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
	
	QByteArray toWrite;
	QDataStream stream(&toWrite, QIODevice::WriteOnly);
	stream << (qint32) Simond::GetTrainingsSample
		<< (qint64) sampleByte.count()+sizeof(qint32) /*seperator*/
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
		emit synchronisationError(i18n("Konnte \"%1\" nicht übertragen", sampleName));
		sendRequest(Simond::ErrorRetrievingTrainingsSample);
		synchronisationOperation->canceled();
		return;
	}

	out << Simond::TrainingsSample
		<< sample.count()
		<< sample;
	socket->write(toWrite);
}




void RecognitionControl::startSynchronisation()
{
	if (!isConnected()) return;

	if (synchronisationOperation)
	{
		synchronisationOperation->deleteLater();
	}

	synchronisationOperation = new Operation(thread(), i18n("Modellsynchronisation"), i18n("Initialisiere..."), 0, 100, false);

	kDebug() << "Starting synchronisation";
	disconnect (modelManager, SIGNAL(modelChanged()), this, SLOT(startSynchronisation()));
	sendRequest(Simond::StartSynchronisation);
}

void RecognitionControl::synchronisationComplete()
{//successful
	kDebug() << "Synchronisation completed";
	synchronisationOperation->finished();
	synchronisationDone();
}

void RecognitionControl::synchronisationDone()
{
	kDebug() << "Finishing up synchronisation";

	if (synchronisationOperation)
	{
		if (!synchronisationOperation->isFinished())
		{
			synchronisationOperation->canceled();
		}
		synchronisationOperation=NULL;
	}
	
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
	qint32 type;
	Simond::Request request;
	QByteArray msgByte=stillToProcess;
	QDataStream msg(&msgByte, QIODevice::ReadOnly);
	bool messageNotYetFinished=false;
	while (socket->bytesAvailable())
	{
		msgByte += socket->readAll();
		while ((msg.device()->bytesAvailable() >= sizeof(qint32)) && !messageNotYetFinished)
		{
			messageNotYetFinished=false;
			msg >> type;
			request = (Simond::Request) type;
			kDebug() << type;
			switch (request)
			{
				////////////////////    LOGIN    //////////////////////////////////////
	
				case Simond::LoginSuccessful:
				{
					advanceStream(sizeof(qint32));
					emit loggedIn();
					startSynchronisation();
					break;
				}
	
				case Simond::VersionIncompatible:
				{
					advanceStream(sizeof(qint32));
					emit simondSystemError(i18n("Version nicht unterstützt"));
					break;
				}
	
				case Simond::AuthenticationFailed:
				{
					advanceStream(sizeof(qint32));
					emit simondSystemError(i18n("Benutzername oder Passwort falsch."));
					this->disconnectFromServer();
					break;
				}
	
				case Simond::AccessDenied:
				{
					advanceStream(sizeof(qint32));
					emit simondSystemError(i18n("Zugriff verweigert."));
					this->disconnectFromServer();
					break;
				}
	
				////////////////////    SYNCHRONISATION    ////////////////////////////
				
				case Simond::GetActiveModelDate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					synchronisationOperation->update(i18n("Synchronisiere Aktives Modell"), 1);
					kDebug() << "Server requested active Model modified date";
					sendActiveModelModifiedDate();
					break;
				}
	
				case Simond::GetActiveModel:
				{
					advanceStream(sizeof(qint32))
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Server requested active Model";
					sendActiveModel();
					break;
				}
	
				case Simond::ActiveModel:
				{
					if (!synchronisationOperation)
						synchronisationOperation = new Operation(thread(), i18n("Modellsynchronisation"), i18n("Synchronisiere Aktives Modell"), 1, 100, false);
					
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Server sent active Model";
					
					parseLengthHeader();
					
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
					
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					sendModelSrcModifiedDate();
					break;
				}
				
				case Simond::GetActiveModelSampleRate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					sendActiveModelSampleRate();
					break;
				}
	
				case Simond::NoActiveModelAvailable:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "No active model available";
					emit synchronisationWarning(i18n("Kein Sprachmodell verfügbar: Erkennung deaktiviert"));
					sendModelSrcModifiedDate();
					
					break;
				}
	
				case Simond::ActiveModelStorageFailed:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Couldn't store active model on server";
					emit synchronisationError(i18n("Konnte Server-Modell nicht aktualisieren"));
					sendModelSrcModifiedDate();
					
					break;
				}
				
				case Simond::GetModelSrcDate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
					
					synchronisationOperation->update(i18n("Überprüfe Modellquelle"), 9);
	
					kDebug() << "Server requested Model src modified date";
					sendModelSrcModifiedDate();
					break;
				}
				
				case Simond::GetTrainingDate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
					synchronisationOperation->update(i18n("Synchronisiere Training"), 10);
	
					kDebug() << "Server Requested training modified date";
					sendTrainingModifiedDate();
					break;
				}
				
				case Simond::GetTraining:
				{
					synchronisationOperation->update(i18n("Sende Training"), 11);
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Server requested training";
					sendTraining();
					break;
				}
				
				case Simond::Training:
				{
					synchronisationOperation->update(i18n("Lade Training"), 11);
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Server sent training";
					parseLengthHeader();
					int sampleRate;
					QByteArray wavConfig, prompts;
					
					QDateTime changedTime;
					msg >> changedTime;
					msg >> sampleRate;
					msg >> wavConfig;
					msg >> prompts;
					
					modelManager->storeTraining(changedTime, sampleRate,wavConfig,prompts);
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					
					synchronisationOperation->update(i18n("Synchronisiere Wortliste"), 3);
					sendWordListModifiedDate();
					break;
				}
				
				case Simond::NoTrainingAvailable:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					kDebug() << "No training available";
					sendWordListModifiedDate();
					break;
				}
				
				case Simond::TrainingStorageFailed:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Server could not store training";
					sendWordListModifiedDate();
					break;
				}
				
				
				case Simond::GetWordListDate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
					
					synchronisationOperation->update(i18n("Synchronisiere Wortliste"), 17);
	
					kDebug() << "Server Requested WordList modified date";
					sendWordListModifiedDate();
					break;
				}
				
				case Simond::GetWordList:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
	
					kDebug() << "Server Requested WordList";
					sendWordList();
					break;
				}
				
				case Simond::WordList:
				{
					checkIfSynchronisationIsAborting();
					kDebug() << "Server sent WordList";
					
					parseLengthHeader();

					QByteArray simple, vocab, lexicon;
					QDateTime changedTime;
					msg >> changedTime;
					msg >> simple;
					msg >> vocab;
					msg >> lexicon;
					
					modelManager->storeWordList(changedTime,simple,vocab,lexicon);
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					
					synchronisationOperation->update(i18n("Synchronisiere Grammatik"), 24);
					sendGrammarModifiedDate();
					break;
				}
				
				case Simond::NoWordListAvailable:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
		
					kDebug() << "No wordlist available";
					emit synchronisationError(i18n("Keine Wortliste verfügbar"));
					sendGrammarModifiedDate();
					break;
				}
				
				case Simond::WordListStorageFailed:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "Server could not store wordlist";
					emit synchronisationError(i18n("Konnte Wortliste nicht auf Server ablegen"));
					sendGrammarModifiedDate();
					break;
				}
				
				case Simond::GetGrammarDate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
					
					synchronisationOperation->update(i18n("Synchronisiere Grammatik"), 31);

					kDebug() << "Server Requested Grammar modified date";
					sendGrammarModifiedDate();
					break;
				}
				
				case Simond::GetGrammar:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "Server requested Grammar";
					sendGrammar();
					break;
				}
				
				case Simond::Grammar:
				{
					checkIfSynchronisationIsAborting();

					kDebug() << "Server sent grammar";
					
					parseLengthHeader();
					
					QByteArray grammar;
					QDateTime changedTime;
					msg >> changedTime;
					msg >> grammar;
					
					modelManager->storeGrammar(changedTime,grammar);
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					
					sendLanguageDescriptionModifiedDate();
					break;
				}
				
				case Simond::NoGrammarAvailable:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "No grammar available";
					sendLanguageDescriptionModifiedDate();
					break;
				}
				
				case Simond::GrammarStorageFailed:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "Server could not store grammar";
					sendLanguageDescriptionModifiedDate();
					break;
				}
				
				case Simond::GetLanguageDescriptionDate:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
					synchronisationOperation->update(i18n("Synchronisiere Sprachdefinition"), 38);

					kDebug() << "Server Requested lang. desc. modified date";
					sendLanguageDescriptionModifiedDate();
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

					kDebug() << "Server sent languagedescription";
					
					parseLengthHeader();
					
					QByteArray treeHed, shadowVocab;
					QDateTime changedTime;
					msg >> changedTime;
					msg >> treeHed;
					msg >> shadowVocab;
					
					modelManager->storeLanguageDescription(changedTime,shadowVocab, treeHed);
					synchronizeSamples();
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					break;
				}
				
				case Simond::NoLanguageDescriptionAvailable:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "No languagedescription available";
					synchronizeSamples();
					break;
				}
				
				case Simond::LanguageDescriptionStorageFailed:
				{
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "Server could not store languagedescription";
					synchronizeSamples();
					break;
				}

				case Simond::GetTrainingsSample:
				{
					checkIfSynchronisationIsAborting();

					kDebug() << "Server requested Trainings-Sample";
					synchronisationOperation->update(i18n("Synchronisiere Trainingsdaten"), 68);
					
					parseLengthHeader();
					
					QByteArray sampleNameByte;
					msg >> sampleNameByte;
					
					sendSample(QString::fromUtf8(sampleNameByte));
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					break;
				}

				case Simond::TrainingsSample:
				{
					synchronisationOperation->update(i18n("Synchronisiere Trainingsdaten"), 68);
					checkIfSynchronisationIsAborting();

					kDebug() << "Server sent Trainings-Sample";
					
					parseLengthHeader();
					
					QByteArray sample;
					msg >> sample;

					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					
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
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();

					kDebug() << "Server could not store trainings-sample";
					synchronisationDone();
					break;
				}


				case Simond::SynchronisationComplete:
				{
					advanceStream(sizeof(qint32));
					synchronisationComplete();
					break;
				}
				
				
				////////////////////    COMPILATION    ////////////////////////////////



				case Simond::ModelCompilationStarted: {
					advanceStream(sizeof(qint32));
					checkIfSynchronisationIsAborting();
					modelCompilationOperation = new Operation(thread(), i18n("Modellerstellung"), i18n("Initialisiere..."));
					break;
				}
				
				case Simond::ModelCompilationStatus: {
					
					parseLengthHeader();
					
					int progNow, progMax;
					msg >> progNow;
					msg >> progMax;
					
					QString statusMsg;
					QByteArray statusByte;
					msg >> statusByte;
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					statusMsg = QString::fromUtf8(statusByte);
					
					modelCompilationOperation->update(i18n("Modell: %1", statusMsg), progNow, progMax);
					break;
				}
				
				case Simond::ModelCompilationError: {
					parseLengthHeader();
					QString errorMsg;
					QByteArray errorByte;
					msg >> errorByte;
					errorMsg = QString::fromUtf8(errorByte);
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					
					modelCompilationOperation->canceled();
// 					modelCompilationOperation->deleteLater();
					modelCompilationOperation=NULL;
					emit compilationError(errorMsg);
					break;
				}

				case Simond::ModelCompilationCompleted: {
					advanceStream(sizeof(qint32));
					modelCompilationOperation->finished();
					modelCompilationOperation->deleteLater();
					modelCompilationOperation=NULL;
				}

				////////////////////    RECOGNITION    ////////////////////////////////

				case Simond::RecognitionReady:
				{
					advanceStream(sizeof(qint32));
					recognitionReady=true;
					emit recognitionStatusChanged(RecognitionControl::Ready);
					break;
				}

				case Simond::RecognitionError:
				{
					parseLengthHeader();
					
					QByteArray errormsgByte;
					msg >> errormsgByte;
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					QString errormsg = QString::fromUtf8(errormsgByte);
					emit recognitionError(errormsg);
					emit recognitionStatusChanged(RecognitionControl::Stopped);
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
					emit recognitionStatusChanged(RecognitionControl::Started);
					break;
				}

				case Simond::RecognitionStopped:
				{
					advanceStream(sizeof(qint32));
					emit recognitionStatusChanged(RecognitionControl::Stopped);
					break;
				}

				case Simond::RecognitionPaused:
				{
					advanceStream(sizeof(qint32));
					emit recognitionStatusChanged(RecognitionControl::Paused);
					break;
				}

				case Simond::RecognitionResumed:
				{
					advanceStream(sizeof(qint32));
					emit recognitionStatusChanged(RecognitionControl::Resumed);
					kDebug() << "Recognition has been resumed" << msgByte;
					break;
				}

				case Simond::RecognitionResult:
				{
					parseLengthHeader();
					
					QByteArray word, sampa, samparaw;
					msg >> word;
					msg >> sampa;
					msg >> samparaw;
					emit recognised(QString::fromUtf8(word), QString::fromUtf8(sampa), QString::fromUtf8(samparaw));
					advanceStream(sizeof(qint32)+sizeof(qint64)+length);
					break;
				}
				
				default:
					kDebug() << "Unknown request";
			}
		}

		messageNotYetFinished=false;

		//this is actually not the correct place (should be at the end of the function)
		//BUT: that way the very last thing the function does it check if there are still bytes available
		//this _ensures_ that we don't loose something
		stillToProcess=msgByte;
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

void RecognitionControl::pauseRecognition()
{
	sendRequest(Simond::PauseRecognition);
}

void RecognitionControl::resumeRecognition()
{
	kDebug() << "Sending resume request";
	sendRequest(Simond::ResumeRecognition);
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

