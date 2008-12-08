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

#ifndef NEWCOMMAND_H
#define NEWCOMMAND_H


#include <KDialog>
#include <QList>
#include "ui_modifycommands.h"

class Command;
class NewCommand;
class CreateCommandWidget;

// typedef EditCommand NewCommand;

class NewCommand : protected KDialog {

Q_OBJECT

private:
	Ui::DlgModifyCommands ui;
	QList<CreateCommandWidget*> *commandCreaters;

private slots:
	void setWindowTitleToCommandName(QString name);
	void checkIfComplete();

public:
	NewCommand(QWidget *parent=0);
	~NewCommand();

	bool registerCreators(QList<CreateCommandWidget*>* commandCreaters);

	Command* newCommand();

public slots:
    void deleteLater();
	void init(Command *command);
	void commandSuggested(Command *command);
};



#endif
