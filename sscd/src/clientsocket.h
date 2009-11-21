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
#include <sscprotocol/sscprotocol.h>


const qint8 protocolVersion=1;

class DatabaseAccess;
class SSCObject;
class User;

class ClientSocket : public QSslSocket
{
	Q_OBJECT

	private:
		QMutex messageLocker;

		DatabaseAccess *databaseAccess;
		
		void waitForMessage(qint64 length, QDataStream& stream, QByteArray& message);
		void sendCode(SSC::Request code);
		void sendObject(SSC::Request code, SSCObject* object);
		void sendObjects(SSC::Request code, QList<SSCObject*> objects);

		void sendUser(qint32 id);
		void sendUsers(User *filterUser, qint32 institutionId, const QString& referenceId);
		void removeUser(qint32 id);
		void sendLanguages();
		void sendInstitutions();
		void removeInstitution(qint32 id);

	private slots:
		void slotSocketError();
		void processRequest();
	public:
		ClientSocket(int socketDescriptor, DatabaseAccess *databaseAccess, QObject *parent=0);
			       
		virtual ~ClientSocket();
		
};

#endif
