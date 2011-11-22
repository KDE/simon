/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_CREATEAKONADICOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEAKONADICOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

#include <QWidget>
#include <simonscenarios/command.h>
#include <simonscenarios/createcommandwidget.h>
#include "ui_createakonadicommandwidget.h"

class Command;
class CommandManager;

/**
 *	@class CreateAkonadiCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an AkonadiCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreateAkonadiCommandWidget : public CreateCommandWidget
{
  Q_OBJECT

  private:
    Ui::CreateAkonadiCommandWidget ui;
    CommandList allCommands;


  public:
    Command* createCommand(const QString& name, const QString& iconSrc, const QString& description);

    bool init(Command* command);
    bool isComplete();

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    explicit CreateAkonadiCommandWidget(CommandManager *manager, QWidget *parent=0);

    virtual ~CreateAkonadiCommandWidget();

};
#endif
