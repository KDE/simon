/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_CREATEDBUSCONDITIONWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEDBUSCONDITIONWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

#include <QWidget>
#include <simoncontextdetection/createconditionwidget.h>
#include "ui_createdbusconditionwidget.h"

class Condition;
class CompoundCondition;

class CreateDBusConditionWidget : public CreateConditionWidget
{
  Q_OBJECT

    private:
    Ui::CreateDBusConditionWidget ui;

  public:
    Condition* createCondition(QDomDocument* doc, QDomElement& conditionElem);

    bool init(Condition* condition);
    bool isComplete();

    explicit CreateDBusConditionWidget( QWidget* parent = 0 );

    virtual ~CreateDBusConditionWidget();

};
#endif
