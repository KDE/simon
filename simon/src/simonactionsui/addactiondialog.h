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

#ifndef SIMON_ADDACTIONDIALOG_H_C44C2BD2497A4FE982149C3550DF0662
#define SIMON_ADDACTIONDIALOG_H_C44C2BD2497A4FE982149C3550DF0662


#include <KDialog>
#include "ui_addactiondlg.h"

class Action;
class ActionModel;
class QSortFilterProxyModel;

class AddActionDialog : public KDialog {

Q_OBJECT

private:
	Ui::DlgAddAction ui;
	QSortFilterProxyModel *proxyModel;
	ActionModel *actionModel;
 
private slots:
	void watchButtonOk();

public:
	Action* getAction();
	int exec();
	AddActionDialog(QWidget *parent=0);
	~AddActionDialog();
};



#endif


