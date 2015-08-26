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

#ifndef SIMON_SSC_MODIFYUSER_H_BAF52575E3B8429C94DF14318118B503
#define SIMON_SSC_MODIFYUSER_H_BAF52575E3B8429C94DF14318118B503

#include <KDELibs4Support/KDE/KDialog>
#include "ui_details.h"

class User;
class ModifyUser;
class UserInInstitution;

class ModifyUser : public KDialog
{

  Q_OBJECT

    private:
    Ui::UserDetails ui;

    QList<UserInInstitution*> uiisCurrent;
    QList<UserInInstitution*> uiisAdd;
    QList<UserInInstitution*> uiisDelete;

    void init(User *u);
    User* createUser(qint32 userId=0);
    void displayUser(User *u);
    void displayLanguages();

    void commitUserInInstitutions(qint32 userId);
    void displayCurrentInstitutionAssociation(qint32 userId);

  private slots:
    void checkIfComplete();
    void addInstitutionAssociation();
    void removeInstitutionAssociation();

  public:
    ModifyUser(QWidget *parent=0);
    ~ModifyUser();

    int modifyUser(User *u);
    int newUser();

  public slots:
    void deleteLater();

};
#endif
