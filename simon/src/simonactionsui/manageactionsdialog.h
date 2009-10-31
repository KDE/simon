/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef MANAGEACTIONSDIALOG_H
#define MANAGEACTIONSDIALOG_H


#include <KDialog>
#include <KPageWidget>
#include "ui_manageactionsdlg.h"
class Action;

class ManageActionsDialog : public KDialog {

Q_OBJECT

private:
	Ui::DlgManageActions ui;
	KPageWidget *pageWidget;

	Action* getCurrentlySelectedAction();

private slots:
	void add();
	void remove();
	void moveUp();
	void moveDown();

public:
	int exec();
	ManageActionsDialog(QWidget *parent=0);
	~ManageActionsDialog();
};



#endif

