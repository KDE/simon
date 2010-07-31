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

#ifndef SIMON_NEWCOMMAND_H_CD6234308AA54117A89845ADFE98BCA1
#define SIMON_NEWCOMMAND_H_CD6234308AA54117A89845ADFE98BCA1

#include <KDialog>
#include <QList>
#include "ui_modifycommands.h"

class Command;
class NewCommand;
class CommandManager;
class CreateCommandWidget;

// typedef EditCommand NewCommand;

class NewCommand : protected KDialog
{

  Q_OBJECT

  private:
    Ui::DlgModifyCommands ui;
    QList<CreateCommandWidget*> *commandCreaters;
    void switchToTypeOfManager(CommandManager* manager);

  private slots:
    void setWindowTitleToCommandName(QString name);
    void checkIfComplete();

  public:
    NewCommand(QWidget *parent=0);
    ~NewCommand();

    bool registerCreators(QList<CreateCommandWidget*>* commandCreaters);

    bool newCommand(CommandManager* preSelectedCategory=0);

  public slots:
    void deleteLater();
    void init(Command *command);
    void commandSuggested(Command *command);
};
#endif
