//
// C++ Interface: juliuscontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef JULIUSCONTROL_H
#define JULIUSCONTROL_H


#include <QObject>
#include "simoninfo.h"

class QSslSocket;
class QTimer;

const qint8 protocolVersion=1;

/**
 *	@class JuliusControl
 *	@brief Provides the methods and emits the needed signals to work with Julius
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class JuliusControl : public QObject {
	Q_OBJECT
private:
	QSslSocket *socket; //!< QSslSocket for communicating with the juliusd-socket
	QTimer *timeoutWatcher;
signals:
	/*--------------------connection--------------------*/
	void connected();
	void disconnected();

	/*
	  Errors are ALWAYS fatal.
	  This signal will only be emitted when an error occurs that _aborts_ the current process.
	
	  BUT, there might be some errors that can be skipped, altough it is not recommended.
	  Those errors can be skipped by setting the corresponding Setting in the juliusd-configuration.
	  If the error is technically not fatal, but is made fatal by setting this configuration item to "false".
	*/
	void error(QString errStr, bool skippable=false);
	void connectionError(QString errStr);
	void warning(QString);

	/*-----------------user management-----------------*/
	void loggedIn();
	void recognised(QString, QString sampa, QString samparaw);
	


	
public slots:
	void messageReceived();
	
	void connectTo( QString server="127.0.0.1", quint16 port=4444 );
	void connectionLost();
	void connectedTo();
	void errorOccured();
	void timeoutReached();
	bool isConnected();
	void disconnectFromServer();
	void login();

private slots:
	void sendRequest (qint32 request);

public:
	JuliusControl();

    ~JuliusControl();

};

#endif
