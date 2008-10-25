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
#include <QObject>
#include <QStringList>

class QSslSocket;
class QTimer;
class ModelManager;

const qint8 protocolVersion=1;

/**
 *	@class RecognitionControl
 *	@brief Provides the methods and emits the needed signals to work with Julius
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
	

	enum RecognitionStatus {
		Ready=0,
		Started=1,
		TemporarilyUnavailable=2,
		Stopped=3
	};

private:
	QSslSocket *socket; //!< QSslSocket for communicating with the juliusd-socket

	QTimer *timeoutWatcher;
	ModelManager *modelManager;

	QStringList serverConnectionsToTry;
	QStringList serverConnectionErrors;

signals:
	/*--------------------connection--------------------*/
	void connected();
	void disconnected();

	/*
	  Errors emitted are ALWAYS fatal.
	  This signal will only be emitted when an error occurs that _aborts_ the current process.
	
	  BUT, there might be some errors that could have been skipped, altough it is not recommended.
	  Those errors can be skipped by setting the corresponding Setting in the juliusd-configuration.
	  If the error is technically not fatal, but is made fatal by setting this configuration item to false, 
	  skippable is true
	*/
	void error(const QString& errStr, bool skippable=false);
	void connectionError(const QString& errStr);
	void warning(const QString&);
	void status(const QString&);
	void progress(int now, int max=-1);

	/*-----------------user management-----------------*/
	void loggedIn();

	/*-------------------recognition-------------------*/
	void recognitionStatusChanged(RecognitionControl::RecognitionStatus);
	void recognised(const QString&, const QString& sampa, const QString& samparaw);
	


	
public slots:
	void disconnectFromServer();
	void startConnecting();
	void stopRecognition();
	void startRecognition();

	void startSynchronisation();
	
	

private slots:
	void sendRequest (qint32 request);
	void login();
	bool isConnected();
	void connectedTo();
	void errorOccured();
	void connectTo( QString server="127.0.0.1", quint16 port=4444 );
	void timeoutReached();
	void messageReceived();
	bool sendActiveModel();
	void sendModelSrcModifiedDate();
	void sendActiveModelModifiedDate();


	void sendWordListModifiedDate();
	void sendWordList();

	void sendGrammarModifiedDate();
	void sendGrammar();

	void sendLanguageDescriptionModifiedDate();
	void sendLanguageDescription();

	void sendTrainingModifiedDate();
	void sendTraining();

	void synchronisationComplete();


};

#endif
