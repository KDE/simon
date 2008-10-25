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
#include <QSqlDatabase>

class DatabaseAccess : public QObject
{
	Q_OBJECT

	signals:
		void error(const QString& error);

	private:
		QSqlDatabase *db;
		bool executeQuery(const QString& query);

	public:
		DatabaseAccess(QObject *parent=0);   
		~DatabaseAccess();

		bool init();
		void closeConnection();

		bool addUser(const QString& username, const QString& password);
		bool authenticateUser(const QString& user, const QString& password);

		bool isConnected() { return db->isOpen(); }
};

#endif
