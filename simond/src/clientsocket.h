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

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QSslSocket>
#include <QList>
#include <QMutex>
#include "clientsocket.h"
#include <simonddatabaseaccess/databaseaccess.h>
#include <simonprotocol/simonprotocol.h>


const qint8 protocolVersion=1;

class DatabaseAccess;
class RecognitionControl;
class SynchronisationManager;
class ModelCompilationManager;

class ClientSocket : public QSslSocket
{
	Q_OBJECT

	private:
		bool synchronisationRunning;
		enum ModelSource {
			Undefined=0,
			Client=1,
			Server=2
		};
		
		QString username;
		QMutex messageLocker;
		
		ModelSource modelSource;

		DatabaseAccess *databaseAccess;
		RecognitionControl *recognitionControl;
		SynchronisationManager *synchronisationManager;
		ModelCompilationManager *modelCompilationManager;
		
		void waitForMessage(qint64 length, QDataStream& stream, QByteArray& message);

	public slots:
		void sendRecognitionResult(const QString& data, const QString& sampa, const QString& samparaw);


	private slots:
		void sendCode(Simond::Request code);
		void processRequest();
		void slotSocketError();

		bool sendActiveModel();

		void recognitionReady();
		void recognitionAwaitingStream(qint32 port, qint32 sampleRate);
		void recognitionError(const QString& error);
		void recognitionWarning(const QString& warning);
		void recognitionStarted();
		void recognitionStopped();
		void recognitionPaused();
		void recognitionResumed();

		bool sendWordList();
		bool sendGrammar();
		bool sendLanguageDescription();
		bool sendTraining();
		
		void recompileModel();
		
		void sendModelCompilationLog();
		void slotModelCompilationStatus(const QString& status, int progressNow, int progressMax);
		void slotModelCompilationError(const QString& error);
		void slotModelCompilationWordUndefined(const QString& word);
		void slotModelCompilationPhonemeUndefined(const QString& phoneme);
		void slotModelCompilationClassUndefined(const QString& undefClass);
		
		void synchronisationComplete();
		void synchronisationDone();
		
		void synchronizeSamples();
		void fetchTrainingSample();
		void sendSample(QString sampleName);
		
		void activeModelCompiled();

	public:
		ClientSocket(int socketDescriptor, DatabaseAccess *databaseAccess, QObject *parent=0);
			       
		~ClientSocket();
		
};

#endif
