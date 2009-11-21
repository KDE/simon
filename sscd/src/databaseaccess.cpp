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

#include "databaseaccess.h"
#include "sscqueries.h"
#include "mysqlqueries.h"
#include <sscobjects/user.h>
#include <sscobjects/language.h>
#include <sscobjects/institution.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>
#include <QVariant>
#include <QSqlDatabase>


/*
 * Constructor
 */
DatabaseAccess::DatabaseAccess(QObject* parent) : QObject(parent),
	db(0), queryProvider(0)
{
}

/*
 * Connects to the database and sets up the query provider;
 * \param type specifies the database software in use (may be SQLITE to use a built-in one; In this case set dbName to filename
 * \param dbName name of the database to use
 * \param user database user
 * \param password database user password
 */
bool DatabaseAccess::init(const QString& type, const QString& host, qint16 port, const QString& dbName, const QString& user, const QString& password)
{
	if(db) {
		db->close();
		delete db;
		QSqlDatabase::removeDatabase("defaultConnection");
		delete queryProvider;
	}

	db = new QSqlDatabase(QSqlDatabase::addDatabase(type));
	
	db->setHostName(host);
	db->setPort(port);
	db->setUserName(user);
	db->setPassword(password);
	db->setDatabaseName(dbName);

	if (type == "QMYSQL") queryProvider = new MYSQLQueries();
	else queryProvider = new SSCQueries();

	if (!db->open()) {	//open database
		emit error(db->lastError().text());
		return false;
	}
	qDebug() << "Database opened...";

	return true;
}



/*
 * Closes the database connection but doesn't delete the handle
 */
void DatabaseAccess::closeConnection()
{
	db->close();
}


/*
 * Returns true if the database handle exists (init() was called) and
 * is connected to a database
 * \see init()
 */
bool DatabaseAccess::isConnected()
{
	return db && db->isOpen();
}

/**
 * Executes the query and returns the success;
 * If the query fails, the error message is propagated through error()
 */
bool DatabaseAccess::executeQuery(QSqlQuery& query)
{
	bool succ = query.exec();
	if (!succ)
		emit error(query.lastError().text());
	return succ;
}



/**
 * Returns the requested user or NULL if the user was not found
 */
User* DatabaseAccess::getUser(qint32 id)
{
	QSqlQuery q = queryProvider->getUser();

	q.bindValue(":userid", id);

	if (!q.exec() || !q.first()) {
		emit error(q.lastError().text());
		return NULL;
	}

	
	return new User(q.value(0).toInt(),
			   q.value(1).toString(),
			   q.value(2).toString(),
			   q.value(3).toChar(),
			   q.value(4).toInt(),
			   q.value(5).toString(),
			   q.value(6).toString(),
			   q.value(7).toString(),
			   q.value(8).toString(),
			   q.value(9).toString(),
			   q.value(10).toString(),
			   q.value(11).toInt(),
			   q.value(12).toInt(),
			   q.value(13).toInt(),
			   q.value(14).toString(),
			   q.value(15).toBool(),
			   q.value(16).toBool());
}


bool DatabaseAccess::addUser(User *u)
{
	QSqlQuery q = queryProvider->addUser();

	q.bindValue(":surname", u->surname());
	q.bindValue(":forename", u->givenName());
	q.bindValue(":sex", u->sex());
	q.bindValue(":birthyear", u->birthYear());
	q.bindValue(":zipcode", u->zipCode());
	q.bindValue(":education", u->education());
	q.bindValue(":currentoccupation", u->currentOccupation());
	q.bindValue(":mothertongue", u->motherTongueId());
	q.bindValue(":diagnosis", u->diagnosis());
	q.bindValue(":orientation", u->orientation());
	q.bindValue(":motorfunction", u->motorFunction());
	q.bindValue(":communication", u->communication());
	q.bindValue(":mouthmotoric", u->mouthMotoric());
	q.bindValue(":interviewpossible", u->interviewPossible());
	q.bindValue(":repeatpossible", u->repeatingPossible());

	return executeQuery(q);
}

bool DatabaseAccess::modifyUser(User *u)
{
	QSqlQuery q = queryProvider->modifyUser();

	q.bindValue(":userid", u->userId());
	q.bindValue(":surname", u->surname());
	q.bindValue(":forename", u->givenName());
	q.bindValue(":sex", u->sex());
	q.bindValue(":birthyear", u->birthYear());
	q.bindValue(":zipcode", u->zipCode());
	q.bindValue(":education", u->education());
	q.bindValue(":currentoccupation", u->currentOccupation());
	q.bindValue(":mothertongue", u->motherTongueId());
	q.bindValue(":diagnosis", u->diagnosis());
	q.bindValue(":orientation", u->orientation());
	q.bindValue(":motorfunction", u->motorFunction());
	q.bindValue(":communication", u->communication());
	q.bindValue(":mouthmotoric", u->mouthMotoric());
	q.bindValue(":interviewpossible", u->interviewPossible());
	q.bindValue(":repeatpossible", u->repeatingPossible());

	qDebug() << "Executing modify query: " << u->userId();

	return executeQuery(q);
}

bool DatabaseAccess::removeUser(qint32 id)
{
	QSqlQuery q = queryProvider->removeUser();

	q.bindValue(":userid", id);

	return executeQuery(q);
}

QList<Language*>* DatabaseAccess::getLanguages()
{
	QList<Language*>* ll = new QList<Language*>();

	QSqlQuery q = queryProvider->getLanguages();

	if (!q.exec()) {
		emit error(db->lastError().text());
		return NULL;
	}

	while (q.next()) {
		ll->append(new Language(q.value(0).toString(),
			   q.value(1).toString()));
	}

	return ll;
}

QList<Institution*>* DatabaseAccess::getInstitutions()
{
	QList<Institution*>* ins = new QList<Institution*>();

	QSqlQuery q = queryProvider->getInstitutions();

	if (!q.exec()) {
		emit error(db->lastError().text());
		return NULL;
	}

	while (q.next()) {
		ins->append(new Institution(q.value(0).toInt(),
			   q.value(1).toString()));
	}

	return ins;
}



bool DatabaseAccess::addInstitution(Institution *i)
{
	QSqlQuery q = queryProvider->addInstitution();

	q.bindValue(":name", i->name());

	return executeQuery(q);
}

bool DatabaseAccess::modifyInstitution(Institution *i)
{
	QSqlQuery q = queryProvider->modifyInstitution();

	q.bindValue(":institutionid", i->id());
	q.bindValue(":name", i->name());

	return executeQuery(q);
}

bool DatabaseAccess::removeInstitution(qint32 id)
{
	QSqlQuery q = queryProvider->removeInstitution();

	q.bindValue(":institutionid", id);

	return executeQuery(q);
}



DatabaseAccess::~DatabaseAccess()
{
	if (db) {
		db->close();
		delete db;
		delete queryProvider;
	}
}
