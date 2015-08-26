/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_CREATEEXECUTABLECOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEEXECUTABLECOMMANDWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

#include <QWidget>
#include <simonscenarios/createcommandwidget.h>
#include "ui_createexecutablecommandwidget.h"

class Command;
class CommandManager;

/**
 *	@class CreateExecutableCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an ExecutableCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreateExecutableCommandWidget : public CreateCommandWidget
{
  Q_OBJECT

    private:
    Ui::CreateExecutableCommandWidget ui;

  private slots:
    void selectProgram();
    void urlSelected(const QUrl&);

  public:
    Command* createCommand(const QString& name, const QString& iconSrc, const QString& description);

    bool init(Command* command);
    bool isComplete();

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    explicit CreateExecutableCommandWidget(CommandManager *manager, QWidget *parent=0);

    virtual ~CreateExecutableCommandWidget();

};
#endif
