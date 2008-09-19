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
#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QSqlTableModel>
#include <QString>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class DatabaseAccess : public QObject {
	Q_OBJECT
private:
	QSqlDatabase *db;
signals:
	void connected(QString host);
	void connectionDropped();
	void queryExecuted(QString);
	void usingTable(QString table);
	void usingDatabase(QString db);
	void error(QString error);
public:
    DatabaseAccess();

    bool connectTo();
    void closeConnection();
    bool insertUser(QString username, QString password);
    QString getErrorString();
    QSqlTableModel* getTable(QString table);
    bool executeQuery(QString query);

    QStringList getTables();
    QString getPassword(QString user);

    bool isConnected() { return db->isOpen(); }


    ~DatabaseAccess();

};

#endif
