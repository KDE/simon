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
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>
#include <KUrl>

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
	db=NULL;
	userModel=NULL;
}

bool DatabaseAccess::init()
{
	if(db)
	{
		db->close();
		delete db;
		QSqlDatabase::removeDatabase("simond");
	}

	db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "simond"));
	
	KConfig config(KStandardDirs::locate("config", "simondrc"));
	KConfigGroup cGroup(&config, "Database");
	QString databaseUrl= cGroup.readEntry("DatabaseUrl", KUrl()).path();

	if (databaseUrl.isEmpty())
	{
		QString localKdeFile = KStandardDirs::locateLocal("data", "simond/simond.db");

		if (!QFile::exists(localKdeFile))
			QFile::copy(KStandardDirs::locate("data", "simond/simond.db"), 
					localKdeFile);

		databaseUrl = localKdeFile;
	}
	
	db->setDatabaseName(databaseUrl);
	m_database = databaseUrl;

	
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
	if (!db) return false;

	kDebug() << "Executing query " << query;
	
	QSqlQuery q(*db);
	q.exec(query);
	if (!q.isActive())
	{
		kDebug() << "Query failed";
		emit error(db->lastError().text());
		return false;
	} else {
		if(userModel) userModel->select();
		return true;
	}
}

void DatabaseAccess::closeConnection()
{
	db->close();
}

//#include <KMessageBox>

//USER
bool DatabaseAccess::addUser(const QString& user, const QString& password)
{
	QString query = "INSERT INTO User (Name, Password) VALUES ('"+user+"', '"+password+"');";
	//KMessageBox::information(0, query);
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
	QSqlQuery q(*db);

	if (q.exec(query)) 
	{
		return q.first();
	} else
	{
		emit error(db->lastError().text());
		return false;
	}
}

bool DatabaseAccess::setPassword(const QString& username, const QString& password)
{
	QString query = "UPDATE User SET Password='"+password+"' WHERE Name='"+username+"';";
	return executeQuery(query);
}


bool DatabaseAccess::deleteUser(const QString& username)
{
	QString query = "DELETE FROM User WHERE Name='"+username+"';";
	return executeQuery(query);
}

QSqlTableModel* DatabaseAccess::getUsers()
{
	if (userModel) return userModel;

	QSqlTableModel *user= new QSqlTableModel(this, *db);
	user->setTable("User");
	user->removeColumn(0); //skip id
	user->setHeaderData(0, Qt::Horizontal, i18n("Benutzername"));
	user->setHeaderData(1, Qt::Horizontal, i18n("Verschlüsseltes Passwort"));
	user->select();

	this->userModel = user;

	return user;
}

DatabaseAccess::~DatabaseAccess()
{
	db->close();
	delete db;
}
