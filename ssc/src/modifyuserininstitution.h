/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SSC_MODIFYUSERININSTITUTION_H_9AAC894018944A3E8A4145273FF70BE7
#define SIMON_SSC_MODIFYUSERININSTITUTION_H_9AAC894018944A3E8A4145273FF70BE7

#include <KDELibs4Support/KDE/KDialog>
#include "ui_modifyuserininstitution.h"

class User;
class Institution;
class UserInInstitution;

class ModifyUserInInstitution : protected KDialog
{

  Q_OBJECT

    private:
    Ui::ModifyUserInInstitution ui;

  private slots:
    void checkIfComplete();
    void findInstitution();

  public:
    ModifyUserInInstitution(QWidget *parent=0);
    ~ModifyUserInInstitution();
    UserInInstitution* add();

  public slots:
    void deleteLater();

};
#endif
