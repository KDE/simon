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

#ifndef SIMON_NEWAUTHOR_H_EB6DB2F851F44CB985AC63C96BB6E9A0
#define SIMON_NEWAUTHOR_H_EB6DB2F851F44CB985AC63C96BB6E9A0

#include <KDialog>
#include "ui_modifyauthor.h"

class Author;

class NewAuthor : protected KDialog
{

  Q_OBJECT

    private:
    Ui::ModifyAuthor ui;

  private slots:
    void checkIfComplete();
  public:
    NewAuthor(QWidget *parent=0);
    ~NewAuthor();

    Author* newAuthor();
};
#endif
