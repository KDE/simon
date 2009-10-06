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


#ifndef RECOGNITIONCONTROL_H
#define RECOGNITIONCONTROL_H

#include "recognitioncontrol_export.h"
#include <simonrecognitionresult/recognitionresult.h>
#include <QObject>
#include <QStringList>
#include <QMutex>

class QSslSocket;
class QTimer;
class ModelManagerUiProxy;
class Operation;

const qint8 protocolVersion=2;

class AdinStreamer;
class QDateTime;

/**
 *	@class RecognitionControl
 *	@brief Provides the methods and emits the needed signals to work with simond
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class RECOGNITIONCONTROL_EXPORT RecognitionControl : public QObject {
	Q_OBJECT

public:
	RecognitionControl(QWidget *parent=0);

	~RecognitionControl();
	
	static RecognitionControl* getInstance(QWidget *parent=0)
	{
		if (!instance) instance = new RecognitionControl(parent);
		return instance;
	}


	enum RecognitionStatus {
		Ready=0,
		Started=1,
		Paused=2,
		Resumed=3,
		Stopped=4
	};


	bool getAvailableModels();
	bool switchToModel(const QDateTime& model);
	bool isConnected();

private:
	static RecognitionControl *instance;
	AdinStreamer *adinStreamer;
	QMutex messageLocker;
	QByteArray stillToProcess;
	bool recognitionReady;
	QSslSocket *socket; //!< QSslSocket for communicating with the simond-socket

	Operation *synchronisationOperation;
	Operation *modelCompilationOperation;

	QTimer *timeoutWatcher;
//	ModelManagerUiProxy *modelManager;

	QStringList serverConnectionsToTry;
	QStringList serverConnectionErrors;

	void sampleNotAvailable(const QString&);
	void wordUndefined(const QString&);
	void classUndefined(const QString&);
 	void phonemeUndefined(const QString&);
	void displayCompilationProtocol(const QString& protocol);


signals:
	void connected();
	void disconnected();

	void connectionError(const QString& errStr);
	void simondSystemError(const QString& errStr);
	void synchronisationError(const QString &err);
	void recognitionError(const QString &err);
	void compilationError(const QString &err, const QString& protocol);

	void simondSystemWarning(const QString&);
	void synchronisationWarning(const QString&);
	void recognitionWarning(const QString&);
	void compilationWarning(const QString&);

	void status(const QString&, int progNow=-1, int progMax=0);
	void progress(int now, int max=-1);

	void loggedIn();

	void recognitionStatusChanged(RecognitionControl::RecognitionStatus);
	void recognised(RecognitionResultList* recognitionResults);

	void modelsAvailable(const QList<QDateTime>& models);
	


	
public slots:
	void actOnAutoConnect();

	void disconnectFromServer();
	void startConnecting();

	void startRecognition();
	void stopRecognition();
	void pauseRecognition();
	void resumeRecognition();

	void fetchCompilationProtocol();
	void askStartSynchronisation();
	void startSynchronisation();
	
	

private slots:
	void slotDisconnected();
	void sendRequest (qint32 request);
	void login();
	void connectedTo();
	void errorOccured();
	void connectTo( QString server="127.0.0.1", quint16 port=4444 );
	void timeoutReached();
	void messageReceived();
	bool sendActiveModel();
	void sendModelSrcModifiedDate();
	void sendActiveModelModifiedDate();
	void sendActiveModelSampleRate();


	void sendWordListModifiedDate();
	void sendWordList();

	void sendGrammarModifiedDate();
	void sendGrammar();

	void sendLanguageDescriptionModifiedDate();
	void sendLanguageDescription();

	void sendTrainingModifiedDate();
	void sendTraining();

	void sendSample(QString sampleName);

	void synchronisationComplete();
	void synchronisationDone();

	void synchronizeSamples();
	void fetchMissingSamples();

	void streamStarted();
	void streamStopped();


};

#endif
