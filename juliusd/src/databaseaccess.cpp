/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "databaseaccess.h"
#include <QDebug>
#include <QSqlRecord>
#include "settings.h"

DatabaseAccess::DatabaseAccess()
{
	db = new QSqlDatabase(QSqlDatabase::addDatabase(Settings::getS("DbType")));
}

bool DatabaseAccess::insertUser(QString user, QString pass)
{
	QString query = "INSERT INTO User (Username, Password) VALUES (\""+user+"\", \""+pass+"\")";
	QSqlQuery q;
	return q.exec(query);
}

bool DatabaseAccess::connectTo()
{
	QString host = Settings::getS("DbHost");
	QString dbname = Settings::getS("DbName");
	db->setHostName(host);
	db->setPort(Settings::get("DbPort").toInt());
	db->setUserName(Settings::getS("DbUser"));
	db->setPassword(Settings::getS("DbPass"));
	db->setDatabaseName(dbname);
	
	if (db->open())
	{
		emit connected(host);
		emit usingDatabase(dbname);
		return true;
	} else {
		emit error(db->lastError().text());
		return false;
	}
}

QString DatabaseAccess::getPassword(QString user)
{
	QSqlQuery q;
	q.prepare("SELECT Password FROM User WHERE Username=\""+user+"\"");
	q.exec();
	q.next();
	return q.record().value("Password").toString();
}

QString DatabaseAccess::getErrorString()
{
	return db->lastError().text();
}

void DatabaseAccess::closeConnection()
{
	db->close();
	emit connectionDropped();
}

QSqlTableModel* DatabaseAccess::getTable(QString table)
{
	QSqlTableModel *sqltable = new QSqlTableModel();
	sqltable->setTable(table);
	if (!(sqltable->select()))
		emit error(sqltable->lastError().text());
	return sqltable;
}

QStringList DatabaseAccess::getTables()
{
	if (isConnected())
		return db->tables();
	else return QStringList();
}

bool DatabaseAccess::executeQuery(QString query)
{
	QSqlQuery q;
	q.exec(query);
	if (!q.isActive()) { 
		emit error(q.lastError().text());
		return false;
	} else {
		emit queryExecuted(query);
		return true;
	}
		
	
}

DatabaseAccess::~DatabaseAccess()
{
	if (db) delete db;
}


