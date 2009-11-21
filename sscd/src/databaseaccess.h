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

#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QList>
#include <QMutex>

class QSqlDatabase;
class QSqlQuery;
class User;
class UserInInstitution;
class Language;
class Institution;
class SSCQueries;

class DatabaseAccess : public QObject
{
	Q_OBJECT

	signals:
		void error(const QString& error);

	private:
		QSqlDatabase *db;
		SSCQueries *queryProvider;
		QMutex transactionLock;

		bool executeQuery(QSqlQuery& query);
		int getLastInsertedId();

	public:
		DatabaseAccess(QObject *parent=0);   
		~DatabaseAccess();

		void lockTranscation();
		void unlockTransaction();

		bool init(const QString& type, const QString& host, qint16 port, const QString& dbName, const QString& user, const QString& password);
		void closeConnection();

		bool isConnected();

		User* getUser(qint32 id);
		QList<User*>* getUsers(User* filterUser, qint32 institutionId, const QString& referenceId);
		bool addUser(User *u, int& userId);
		bool modifyUser(User *u);
		bool removeUser(qint32 id);
		int getLastUserId();

		QList<Language*>* getLanguages();
		Institution* getInstitution(qint32 id);
		QList<Institution*>* getInstitutions();
		bool addInstitution(Institution *i);
		bool modifyInstitution(Institution *i);
		bool removeInstitution(qint32 id);


		int addUserInstitutionAssociation(UserInInstitution *uii);
		bool deleteUserInstitutionAssociation(qint32 userId, qint32 institutionId);
		QList<UserInInstitution*>* getUserInstitutionAssociation(qint32 userId);
};

#endif
