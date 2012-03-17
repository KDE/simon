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

#ifndef SIMON_CREATEORCONDITIONASSOCIATIONWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3
#define SIMON_CREATEORCONDITIONASSOCIATIONWIDGET_H_9F05429CB8EF43B8BFAC15877F9854C3

/** \file createorconditionassociation.h
 * \brief The file containing the CreateOrConditionAssociation baseclass header.
 */

#include <QWidget>
#include <simoncontextdetection/createconditionwidget.h>
#include "ui_createorconditionassociationwidget.h"

class Condition;
class CompoundCondition;
class QSortFilterProxyModel;

/**
 *	@class CreateOrConditionAssociationWidget
 *	@brief The CreateOrConditionAssociationWidget class provides a widget to modify the specific attributes of a OrConditionAssociation
 *
 *      The CreateOrConditionWidget allows the component Condition objects of the OrConditionAssociation to be changed by
 *      providing a ListView containing all of the current Condition objects as well as add, delete, and edit Condition
 *      buttons.  The NewAssociationCondition dialog is used for adding new Condition objects the the OrConditionAssociation.
 *
 *      \sa OrConditionAssociation, CreateConditionWidget, NewAssociationCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */
class CreateOrConditionAssociationWidget : public CreateConditionWidget
{
  Q_OBJECT

    private:
    Ui::CreateOrConditionAssociationWidget ui;

    CompoundCondition* m_compoundAssociationCondition;
    QSortFilterProxyModel *m_conditionsProxy;

    Condition* getCurrentCondition();

  private slots:
    void addAssociationCondition();
    void deleteAssociationCondition();
    void editAssociationCondition();
    void selectionChanged();

  public:
    Condition* createCondition();

    bool init(Condition* condition);
    bool isInstanceOfSameCondition(Condition *condition);
    bool isComplete();

    explicit CreateOrConditionAssociationWidget(CompoundCondition *compoundCondition, QWidget *parent=0);

    virtual ~CreateOrConditionAssociationWidget();

};
#endif
