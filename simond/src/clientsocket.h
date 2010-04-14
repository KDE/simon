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

#ifndef SIMON_CLIENTSOCKET_H_4485408A4C1743CDB368CCC3616AC16F
#define SIMON_CLIENTSOCKET_H_4485408A4C1743CDB368CCC3616AC16F

#include "recognitioncontrol.h"
#include <simonddatabaseaccess/databaseaccess.h>
#include <simonprotocol/simonprotocol.h>
#include <QSslSocket>
#include <QList>
#include <QMutex>
#include <QString>


const qint8 protocolVersion=3;

class DatabaseAccess;
class RecognitionControl;
class SynchronisationManager;
class ModelCompilationManager;
class ModelCompilationAdapter;
class Model;
class WAV;

class ClientSocket : public QSslSocket
{
	Q_OBJECT

	private:
		bool synchronisationRunning;
		
		QString username;
		QMutex messageLocker;
		
		DatabaseAccess *databaseAccess;
		RecognitionControl *recognitionControl;
		SynchronisationManager *synchronisationManager;
		ModelCompilationManager *modelCompilationManager;
		ModelCompilationAdapter *modelCompilationAdapter;

		uint newLexiconHash;
		uint newGrammarHash;
		uint newVocaHash;

		WAV *currentSample;
		
		bool shouldRecompileModel();
		void waitForMessage(qint64 length, QDataStream& stream, QByteArray& message);
		void writeHashesToConfig();

	public slots:
		void sendRecognitionResult(const QString& fileName, const RecognitionResultList& recognitionResults);


	private slots:
		void startSynchronisation();

		void sendCode(Simond::Request code);
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
		
		void recompileModel();
		
		void sendModelCompilationLog();
		void slotModelCompilationStatus(QString status, int progressNow, int progressMax);
		void slotModelCompilationError(QString error);
		void slotModelCompilationWordUndefined(const QString& word);
		void slotModelCompilationPhonemeUndefined(const QString& phoneme);
		void slotModelCompilationClassUndefined(const QString& undefClass);

		void slotModelAdaptionComplete();
		void slotModelAdaptionAborted();
		void slotModelAdaptionStatus(QString status, int progressNow);
		void slotModelAdaptionError(QString error);

		void synchronisationComplete();
		void synchronisationDone();

		void sendScenarioList();
		void fetchScenario();
		void requestScenario(const QString& scenarioId);
		void sendScenario(const QString& scenarioId);
		void sendSelectedScenarioList();
		void synchronizeAlreadyAvailableScenarios();
		
		void synchronizeSamples();
		void fetchTrainingSample();
		void sendSample(QString sampleName);
		
		void activeModelCompiled();
		void activeModelCompilationAborted();

	public:
		ClientSocket(int socketDescriptor, DatabaseAccess *databaseAccess, QObject *parent=0);
			       
		virtual ~ClientSocket();
		
};

#endif
