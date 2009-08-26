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
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QVariant>

#include <KDebug>
#include <KStandardDirs>

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
	db = 0;
}

bool DatabaseAccess::init()
{
	if (db) {
		db->close();
		delete db;
	}
	db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
	
	KConfig config(KStandardDirs::locate("config", "simondrc"));
	KConfigGroup cGroup(&config, "Database");
	QString dbUrl = cGroup.readEntry("DatabaseUrl", KUrl(KStandardDirs::locateLocal("appdata", "simond.db"))).path();
	
	if (!QFile::exists(dbUrl)) {
		QFile::copy(KStandardDirs::locate("appdata", "simond.db"), dbUrl);
	}
	
	db->setDatabaseName(dbUrl);
	
	if (!db->open())	//open database
	{
		emit error(db->lastError().text());
		
		return false;
	}
	
	if (db->tables().isEmpty()) //create tables
	{
		QSqlQuery q("CREATE TABLE `User` ( "
				    "UserID integer PRIMARY KEY AUTOINCREMENT, "
				    "Name varchar(150) NOT NULL, "
				    "Password varchar(250) not null"
				    ");");
		if (!q.exec()) {
			emit error(db->lastError().text());
			return false;
		}
		return true;
	}
	return true;
}



void DatabaseAccess::closeConnection()
{
	db->close();
}


//USER

bool DatabaseAccess::addUser(const QString& user, const QString& password)
{
	QString query = "INSERT INTO User (Name, Password) VALUES ('"+user+"', '"+password+"')";
	QSqlQuery q;
	q.prepare("INSERT INTO User (Name, Password) VALUES (:user, :password)");
	q.bindValue(":user", user);
	q.bindValue(":password", password);
	if (!q.exec()) {
		emit error(db->lastError().text());
		return false;
	}

	return true;
}

bool DatabaseAccess::authenticateUser(const QString& user, const QString& password)
{
	QSqlQuery q;
	q.prepare("SELECT Name FROM User WHERE Name=':user' AND Password=':pass'");
	q.bindValue(":user", user);
	q.bindValue(":password", password);

	if (q.exec(query)) 
	{
		return q.first();
	} else
	{
		emit error(db->lastError().text());
		return false;
	}
}

DatabaseAccess::~DatabaseAccess()
{
	if (db) {
		db->close();
		delete db;
	}
}
