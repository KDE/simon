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


#ifndef JULIUSCONTROL_H
#define JULIUSCONTROL_H


#include <QObject>

class QSslSocket;
class QTimer;

const qint8 protocolVersion=0;

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

	void sendSyncFile(QString filename);

};

#endif
