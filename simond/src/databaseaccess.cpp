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
}

bool DatabaseAccess::init()
{
	db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
	
	if (!QFile::exists(KStandardDirs::locateLocal("appdata", "simond.db")))
	{
		QFile::copy(KStandardDirs::locate("appdata", "simond.db"), 
				KStandardDirs::locateLocal("appdata", "simond.db"));
	}
	
	db->setDatabaseName(KStandardDirs::locateLocal("appdata", "simond.db"));
	
	if (!db->open())	//open database
	{
		emit error(db->lastError().text());
		
		kDebug() << "Could not open db";
		
		return false;
	}
	
	if (db->tables().isEmpty()) //create tables
	{
		kDebug() << "Creating tables";
		return executeQuery("CREATE TABLE `User` ( "
				    "UserID integer PRIMARY KEY AUTOINCREMENT, "
				    "Name varchar(150) NOT NULL, "
				    "Password varchar(250) not null"
				    ");");
	}
	return true;
}



bool DatabaseAccess::executeQuery(const QString& query)
{
	kDebug() << "Executing query " << query;
	
	QSqlQuery q;
	q.exec(query);
	if (!q.isActive())
	{
		kDebug() << "Query failed";
		emit error(db->lastError().text());
		return false;
	} else return true;
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
	return executeQuery(query);
}

bool DatabaseAccess::authenticateUser(const QString& user, const QString& password)
{
	QString cleanedPass = password;
	cleanedPass = cleanedPass.replace("'", "\\'");
	QString cleanedUser = user;
	cleanedUser = cleanedUser.replace("'", "\\'");
	QString query = "SELECT Name FROM User WHERE Name='"+cleanedUser+"' AND Password='"+cleanedPass+"'";
	kDebug() << query;
	QSqlQuery q;
	
// 	QSqlQuery q3;
// 	kDebug() << q3.exec("Update User set Password='"+password+"';");
	
// 	QSqlQuery q2;
// 	kDebug() << q2.exec("Select password from User");
// 	while (q2.next())
// 		if (q2.value(0).toString() == password)
// 			kDebug() << "Found It!";
// 		else kDebug() << "Zonk: " << q2.value(0).toString() << password;

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
	db->close();
	delete db;
}
