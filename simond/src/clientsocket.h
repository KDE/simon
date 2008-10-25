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
#include "clientsocket.h"
#include "databaseaccess.h"
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

		enum ModelSource {
			Undefined=0,
			Client=1,
			Server=2
		};
		
		QString username;
		
		ModelSource modelSource;

		DatabaseAccess *databaseAccess;
		RecognitionControl *recognitionControl;
		SynchronisationManager *synchronisationManager;
		ModelCompilationManager *modelCompilationManager;

	public slots:
		void sendRecognitionResult(const QString& data, const QString& sampa, const QString& samparaw);


	private slots:
		void sendCode(Simond::Request code);
		void processRequest();
		void slotSocketError();

		bool sendActiveModel();

		void recognitionReady();
		void recognitionError(const QString& error);
		void recognitionWarning(const QString& warning);
		void recognitionStarted();
		void recognitionStopped();
		void recognitionTemporarilyUnavailable(const QString& reason);

		bool sendWordList();
		bool sendGrammar();
		bool sendLanguageDescription();
		bool sendTraining();

	public:
		ClientSocket(int socketDescriptor, DatabaseAccess *databaseAccess, QObject *parent=0);
			       
		~ClientSocket();
		
};

#endif
