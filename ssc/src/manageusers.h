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

#ifndef SIMON_SSC_MANAGEUSERS_H_B9CB5471B4234231A17437881707E5A8
#define SIMON_SSC_MANAGEUSERS_H_B9CB5471B4234231A17437881707E5A8

#include <KDialog>
#include "ui_finduser.h"

class User;
class UserModel;
class QSortFilterProxyModel;

class ManageUsers : public KDialog
{

  Q_OBJECT

    private:
    Ui::Users ui;
    UserModel *model;
    QSortFilterProxyModel *proxyModel;

    void updateList();
    User* getCurrentlySelectedUser();
    int getInterviewingPossible();
    int getRepeatingPossible();
    char getSex();
    void displayLanguages();

  private slots:
    void filter();

    void addUser();
    void editUser();
    void deleteUser();

    void findInstitution();

  public:
    ManageUsers(QWidget *parent=0);
    ~ManageUsers();
    int exec();

    User* getUser();

  public slots:
    void deleteLater();
};
#endif
