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

#ifndef SIMON_DATABASEACCESS_H_7A4CB26A3C424BDB8B4E76F6ED8EA465
#define SIMON_DATABASEACCESS_H_7A4CB26A3C424BDB8B4E76F6ED8EA465

#include <QObject>
#include <QList>
#include <QMutex>

class QSqlDatabase;
class QSqlQuery;
class User;
class Sample;
class Microphone;
class SoundCard;
class UserInInstitution;
class Language;
class Institution;
class QStringList;
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

    bool init(const QString& type, const QString& host, qint16 port, const QString& dbName, const QString& user, const QString& password, const QString& options);
    void closeConnection();

    bool isConnected();

    User* getUser(qint32 id);
    QList<User*> getUsers(User* filterUser, qint32 institutionId, const QString& referenceId, bool *success);
    bool addUser(User *u, int& userId);
    bool modifyUser(User *u);
    bool removeUser(qint32 id);
    int getLastUserId();

    QList<Language*> getLanguages(bool *success = 0);
    QList<Microphone*> getMicrophones(bool *success = 0);
    QList<SoundCard*> getSoundCards(bool *success = 0);
    Institution* getInstitution(qint32 id);
    QList<Institution*> getInstitutions(bool *success = 0);
    bool addInstitution(Institution *i);
    bool modifyInstitution(Institution *i);
    bool removeInstitution(qint32 id);

    bool getOrCreateMicrophone(Microphone *m, qint32& microphoneId);
    bool getOrCreateSoundCard(SoundCard *s, qint32& soundCardId);

    int addUserInstitutionAssociation(UserInInstitution *uii);
    bool deleteUserInstitutionAssociation(qint32 userId, qint32 institutionId);
    QList<UserInInstitution*> getUserInstitutionAssociation(qint32 userId, bool *success);

    qint32 nextSampleId();
    bool storeSample(Sample *s);
    QStringList* getSamplePaths(qint32 userId);
};
#endif
