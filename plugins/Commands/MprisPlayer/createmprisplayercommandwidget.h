/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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

#ifndef SIMON_CREATEMPRISPLAYERCOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEMPRISPLAYERCOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

#include "mprisplayercommand.h"
#include "ui_createmprisplayercommandwidget.h"
#include <simonscenarios/createcommandwidget.h>

#include <QWidget>

class CommandManager;

/**
 *	@class CreateMprisPlayerCommandWidget
 *	@brief Provides a widget to select the method to be called
 *
 *	@version 0.1
 *	@date 23.05.2014
 *	@author Ashish Madeti
 */
class CreateMprisPlayerCommandWidget : public CreateCommandWidget
{
    Q_OBJECT

  public:
    Command* createCommand(const QString& name, const QString& iconSrc, const QString& description);

    bool init(Command* command);
    bool isComplete();

    explicit CreateMprisPlayerCommandWidget(CommandManager *manager, QWidget *parent=0);
    virtual ~CreateMprisPlayerCommandWidget();

  private:
    Ui::CreateMprisPlayerCommandWidget ui;
};

#endif // SIMON_CREATEMPRISPLAYERCOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
