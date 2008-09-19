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
#include "userdialog.h"
#include "databaseaccess.h"
#include "newuserdialog.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>

UserDialog::UserDialog(DatabaseAccess *db, QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
	ui.setupUi(this);
	this->db = db;
	this->newUser = new NewUserDialog(this);
	init();
	connect(ui.pbDeleteUser, SIGNAL(clicked()), this, SLOT(deleteUser()));
	connect(ui.pbAddUser, SIGNAL(clicked()), this, SLOT(addUser()));
}

void UserDialog::addUser()
{
	if (!db) return;
	newUser->init();
	if (newUser->exec())
	{
		QString username = newUser->getUsername();
		QString password = newUser->getPassword();
		/*bool success = */db->insertUser(username, password);
		QSqlTableModel *model = (QSqlTableModel*) ui.tvUsers->model();
		model->select();
	}
}

void UserDialog::deleteUser()
{
	QModelIndex index = ui.tvUsers->currentIndex();
	if (!index.isValid())
	{
		QMessageBox::information(this, i18n("Ungültige Auswahl"), i18n("Bitte wählen Sie einen Benutzer in der Liste aus"));
		return;
	}
	if (QMessageBox::question(this, i18n("Benutzer löschen"), i18n("Wollen Sie den ausgewählten Benutzer wirklich unwiederruflich löschen?"), QMessageBox::Yes|QMessageBox::No))
	{
		
		QSqlTableModel *model = (QSqlTableModel*) ui.tvUsers->model();
		model->removeRow(index.row());
	}
}


bool UserDialog::init()
{
	if (!db) return false;
	ui.tvUsers->setModel(db->getTable("User"));
	return true;
}

UserDialog::~UserDialog()
{
}


