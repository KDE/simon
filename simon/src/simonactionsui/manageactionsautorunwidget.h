/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_MANAGEACTIONSAUTORUNWIDGET_H_FFF7A6377C9C4E42B2B5636676AD423F
#define SIMON_MANAGEACTIONSAUTORUNWIDGET_H_FFF7A6377C9C4E42B2B5636676AD423F

#include <QWidget>
#include "ui_manageactionsautorunwidget.h"
#include <simonscenarios/command.h>

class ManageActionsAutorunWidget : public QWidget
{
  private:
    Ui::ManageActionsAutorunWidget ui;
    CommandList allCommands;

    void initCommands();

  protected:
    void showEvent(QShowEvent *event);

  public:
    ManageActionsAutorunWidget(QWidget *parent = 0);
    bool save();
};

#endif

