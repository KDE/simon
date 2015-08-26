/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_DATABASEACCESS_H_AC44723DF6984B64B25AFB4B64838A57
#define SIMON_DATABASEACCESS_H_AC44723DF6984B64B25AFB4B64838A57

#include "simonddatabaseaccess_export.h"
#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>

class SIMONDDATABASEACCESS_EXPORT DatabaseAccess : public QObject
{
  Q_OBJECT

    signals:
  void error(const QString& error);

  private:
    QSqlDatabase *db;
    QSqlTableModel *userModel;
    QString m_database;

  public:
    DatabaseAccess(QObject *parent=0);
    ~DatabaseAccess();

    bool init();
    QString getDatabase() { return m_database; }
    void closeConnection();

    bool addUser(const QString& username, const QString& password);
    bool deleteUser(const QString& username);
    bool setPassword(const QString& username, const QString& password);

    QSqlTableModel* getUsers();

    bool authenticateUser(const QString& user, const QString& password);

    bool isConnected() { return db->isOpen(); }
};
#endif
