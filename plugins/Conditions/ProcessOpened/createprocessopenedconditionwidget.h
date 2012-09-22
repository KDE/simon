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

/** \file createprocessopenedcondition.h
 * \brief The file containing the CreateProcessOpenedCondition baseclass header.
 */

#include <QWidget>
#include <simoncontextdetection/createconditionwidget.h>
#include "ui_createprocessopenedconditionwidget.h"

class Condition;

/**
 *	@class CreateProcessOpenedConditionWidget
 *	@brief The CreateProcessOpenedConditionWidget class provides a widget to modify the specific attributes of a ProcessOpenedCondition
 *
 *      The CreateProcessOpenedConditionWidget allows the process name that will be monitored by the ProcessOpenedCondition
 *      to be specified by the user with a LineEdit.
 *
 *      \sa Condition, ProcessInfo, ProcessOpenedCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */
class CreateProcessOpenedConditionWidget : public CreateConditionWidget
{
  Q_OBJECT

  public:
    Condition* createCondition(QDomDocument* doc, QDomElement& conditionElem);

    bool init(Condition* condition);
    bool isComplete();

    explicit CreateProcessOpenedConditionWidget(QWidget *parent=0);

    virtual ~CreateProcessOpenedConditionWidget();

  private:
    Ui::CreateProcessOpenedConditionWidget ui;

  private slots:
    void processFileDialog();

};
#endif
