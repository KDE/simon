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

#ifndef SIMON_CREATEPROCESSOPENEDCONDITIONWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEPROCESSOPENEDCONDITIONWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

#include <QWidget>
#include <simoncontextdetection/createconditionwidget.h>
#include "ui_createprocessopenedconditionwidget.h"

class Condition;
class CompoundCondition;

/**
 *	@class CreateProcessOpenedConditionWidget
 *	@brief Provides a widget to modify the specific attributes of a ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 6.3.2011
 *	@author Adam Nash
 */
class CreateProcessOpenedConditionWidget : public CreateConditionWidget
{
  Q_OBJECT

    private:
    Ui::CreateProcessOpenedConditionWidget ui;

  private slots:

  public:
    Condition* createCondition();

    bool init(Condition* condition);
    bool isComplete();

    explicit CreateProcessOpenedConditionWidget(CompoundCondition *compoundCondition, QWidget *parent=0);

    virtual ~CreateProcessOpenedConditionWidget();

};
#endif
