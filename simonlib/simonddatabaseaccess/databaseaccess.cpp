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
#include <KLocale>

DatabaseAccess::DatabaseAccess(QObject* parent) : QObject(parent),
db(0),
userModel(0)
{
}


bool DatabaseAccess::init()
{
  if(db) {
    db->close();
    delete db;
    QSqlDatabase::removeDatabase("simond");
  }

  db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "simond"));

  KConfig config(KStandardDirs::locate("config", "simondrc"));
  KConfigGroup cGroup(&config, "Database");
  QString databaseUrl= cGroup.readEntry("DatabaseUrl", KUrl()).toLocalFile();

  if (databaseUrl.isEmpty()) {
    QString localKdeFile = KStandardDirs::locateLocal("data", "simond/simond.db");

    if (!QFile::exists(localKdeFile))
      QFile::copy(KStandardDirs::locate("data", "simond/simond.db"),
        localKdeFile);

    databaseUrl = localKdeFile;
  }

  db->setDatabaseName(databaseUrl);
  m_database = databaseUrl;

  if (!db->open()) {                              //open database
    emit error(db->lastError().text());

    kDebug() << "Could not open db";

    return false;
  }

  if (db->tables().isEmpty()) {                   //create tables
    QSqlQuery q("CREATE TABLE `User` ( "
      "UserID integer PRIMARY KEY AUTOINCREMENT, "
      "Name varchar(150) NOT NULL, "
      "Password varchar(250) not null"
      ");", *db);
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
  QSqlQuery q(*db);
  q.prepare("INSERT INTO User (Name, Password) VALUES (:user, :password)");
  q.bindValue(":user", user);
  q.bindValue(":password", password);

  if (!q.exec()) {
    emit error(q.lastError().text());
    return false;
  }

  if (userModel) userModel->select();
  return true;
}


bool DatabaseAccess::authenticateUser(const QString& user, const QString& password)
{
  QSqlQuery q(*db);
  q.prepare("SELECT Name FROM User WHERE Name=:user AND Password=:pass");
  q.bindValue(":user", user);
  q.bindValue(":password", password);

  if (q.exec())
    return q.first();

  emit error(db->lastError().text());
  return false;
}


bool DatabaseAccess::setPassword(const QString& username, const QString& password)
{
  QSqlQuery q(*db);
  q.prepare("UPDATE User SET Password=:password WHERE Name=:user");
  q.bindValue(":user", username);
  q.bindValue(":password", password);

  if (!q.exec()) {
    emit error(db->lastError().text());
    return false;
  }

  if (userModel) userModel->select();
  return true;
}


bool DatabaseAccess::deleteUser(const QString& username)
{
  QSqlQuery q(*db);
  q.prepare("DELETE FROM User WHERE Name=:user");
  q.bindValue(":user", username);

  if (!q.exec()) {
    emit error(db->lastError().text());
    return false;
  }

  if (userModel) userModel->select();
  return true;
}


QSqlTableModel* DatabaseAccess::getUsers()
{
  if (userModel) {
    userModel->select();
    return userModel;
  }

  userModel = new QSqlTableModel(this, *db);
  userModel->setTable("User");
  userModel->removeColumn(0);                     //skip id
  userModel->setHeaderData(0, Qt::Horizontal, i18nc("Name of the Simond user", "Username"));
  userModel->setHeaderData(1, Qt::Horizontal, i18n("Encrypted Password"));
  userModel->select();

  return userModel;
}


DatabaseAccess::~DatabaseAccess()
{
  if (db) {
    db->close();
    delete db;
  }
}
