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

#ifndef SIMON_COMMANDPREVIEWWIDGET_H_1F3BDF0AC51C423D9CA587CFC24BF9B6
#define SIMON_COMMANDPREVIEWWIDGET_H_1F3BDF0AC51C423D9CA587CFC24BF9B6

#include <QScrollArea>
#include "ui_commandpreviewwidget.h"

class Command;
class QModelIndex;

class CommandPreviewWidget : public QWidget
{

  Q_OBJECT

    private:
    Ui::CommandPreviewDlg ui;
    Command *command;

  private slots:
    void trigger();

  public slots:
    void updateCommand(const QModelIndex &commandIdx);

  public:
    CommandPreviewWidget(QWidget *parent=0);
    ~CommandPreviewWidget() {}

};
#endif
