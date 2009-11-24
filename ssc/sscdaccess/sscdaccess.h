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


#ifndef SSCDACESS_H
#define SSCDACESS_H

#include "sscdaccess_export.h"
#include <sscobjects/user.h>
#include <sscobjects/language.h>
#include <sscprotocol/sscprotocol.h>
#include <QList>
#include <QObject>

class QSslSocket;
class QTimer;
class Operation;
class SSCObject;

class User;
class Institution;
class UserInInstitution;

const qint8 protocolVersion=1;

/**
 *	@class SSCDAccess
 *	@brief Provides the methods and emits the needed signals to work with sscd
 *
 *	@version 0.1
 *	@date 24.10.2009
 *	@author Peter Grasch
 */
class SSCDACESS_EXPORT SSCDAccess : public QObject {
	Q_OBJECT

signals:
	void connected();
	void disconnected();

	void error(const QString& errStr);

	void warning(const QString&);

	void status(const QString&, int progNow=-1, int progMax=0);
	void progress(int now, int max=-1);


public:
	SSCDAccess(QWidget *parent=0);
	~SSCDAccess();
	
	static SSCDAccess* getInstance(QWidget *parent=0)
	{
		if (!instance) instance = new SSCDAccess(parent);
		return instance;
	}

	bool isConnected();
	QString lastError();

	User *getUser(qint32 id);
	QList<User*> getUsers(User *filter,qint32 institutionId, const QString& referenceId, bool *ok);
	int addUser(User* u);
	bool modifyUser(User* u);

	bool deleteUser(User* u);

	QList<Language*> getLanguages(bool *ok);

	QList<Institution*> getInstitutions(bool *ok);
	bool addInstitution(Institution* i);
	bool modifyInstitution(Institution* i);
	bool deleteInstitution(Institution* i);


	bool addUserInInstitution(UserInInstitution* uii);
	bool deleteUserInInstitution(UserInInstitution* uii);
	QList<UserInInstitution*> getUserInInstitutions(qint32 userId, bool *ok);


	bool sendSample(qint32 userId, qint32 sampleType, const QString& prompt, const QByteArray& data);

public slots:
	void disconnectFromServer();
	void connectTo( QString server, quint16 port, bool encrypted );

private:
	static SSCDAccess *instance;
	QSslSocket *socket; //!< QSslSocket for communicating with the sscd-socket

	QTimer *timeoutWatcher;
	QString lastErrorString;
	void waitForMessage(qint64 length, QDataStream& stream, QByteArray& message);
	
private slots:
	bool sendRequest (qint32 request);
	bool sendRequest (qint32 request, qint32 message);
	bool sendRequest (qint32 request, qint32 message, qint32 message2);
	void sendObject(SSC::Request code, SSCObject* object);
	void connectedTo();
	void errorOccured();
	void timeoutReached();
};

#endif
