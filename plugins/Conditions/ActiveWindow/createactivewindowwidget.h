/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef SIMON_CREATEACTIVEWINDOWWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEACTIVEWINDOWWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

/** \file createactivewindow.h
 * \brief The file containing the CreateActiveWindow baseclass header.
 */

#include <QWidget>
#include <simoncontextdetection/createconditionwidget.h>
#include "ui_createactivewindowwidget.h"

class Condition;
class CompoundCondition;

/**
 *	@class CreateActiveWindowWidget
 *	@brief The CreateActiveWindowWidget class provides a widget to modify the specific attributes of a ActiveWindow
 *
 *      The CreateActiveWindowWidget allows the active window and its associated process name that will be monitored by the ActiveWindow
 *      to be specified by the user with a LineEdit and file browser (so that the desired executable file can be located)
 *
 *      \sa Condition, ProcessInfo, ActiveWindow
 *
 *	@version 0.1
 *	@date 8.3.2011
 *	@author Adam Nash
 */
class CreateActiveWindowWidget : public CreateConditionWidget
{
  Q_OBJECT

    private:
    Ui::CreateActiveWindowWidget ui;

  private slots:

  public:
    Condition* createCondition();

    bool init(Condition* condition);
    bool isInstanceOfSameCondition(Condition *condition);
    bool isComplete();

    explicit CreateActiveWindowWidget(CompoundCondition *compoundCondition, QWidget *parent=0);

    virtual ~CreateActiveWindowWidget();

};
#endif
