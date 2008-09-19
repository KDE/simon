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
#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>

#include "ui_users.h"
class NewUserDialog;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class DatabaseAccess;

class UserDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::DlgUsers ui;
	DatabaseAccess *db;
	NewUserDialog *newUser;
	
	bool init();
	
public slots:
	void addUser();
	void deleteUser();
	
public:
    UserDialog(DatabaseAccess *db, QWidget* parent=0, Qt::WindowFlags f=0);
    ~UserDialog();

};

#endif
