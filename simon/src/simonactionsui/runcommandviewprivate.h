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

#ifndef RUNCOMMANDVIEWPRIVATE_H
#define RUNCOMMANDVIEWPRIVATE_H

#include "ui_rundialog.h"

class Command;
class RunCommand;
class CommandPreviewWidget;
/**
 *	@class RunCommandViewPrivate
 *	@brief Provides a graphical frontend to run commands and perform 
 * 	system actions
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class RunCommandViewPrivate : public QWidget {
	Q_OBJECT

private:
	Ui::RunDlg ui;
	Command* getCommandToModify();

private slots:
	void addCommand();
	void deleteCommand();
	void editCommand();
	void reflectSelectionStatus(QModelIndex index);

public slots:
	void loadCommands();

public:
    RunCommandViewPrivate(QWidget *parent);

    ~RunCommandViewPrivate();

};

#endif
