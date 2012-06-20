/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com
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

#ifndef SIMON_NEWASSOCIATIONCONDITION_H_CD6234308AA54117A89845ADFE98BCA1
#define SIMON_NEWASSOCIATIONCONDITION_H_CD6234308AA54117A89845ADFE98BCA1

/** \file newconditionassociation.h
 * \brief The file containing the NewConditionAssociation baseclass header.
 */

#include <KDialog>
#include <QList>
#include "ui_newassociationcondition.h"

class Condition;
class CreateConditionWidget;

/**
 *	@class NewAssociationCondition
 *	@brief The NewAssociationCondition class is a dialog for creating conditions that will be monitored by an AssociationCondition
 *
 *      The NewAssociationCondition uses CreateConditionWidget objects that have been passed to it from \ref CreateOrConditionAssociationWidget::addCondition()
 *      and uses those CreateConditionWidget objects to allow the user to customize a new Condition that
 *      will be added to the CompoundCondition that temporarily stores the component Condition objects of the OrConditionAssociation.
 *
 *      \sa ConditionAssociation, OrConditionAssociation, CompoundCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class NewAssociationCondition : protected KDialog
{
  Q_OBJECT

  private:
    Ui::DlgAssociationCondition ui;
    QList<CreateConditionWidget*> m_conditionCreators;
    //void switchToTypeOfCondition(Condition* condition);

  private slots:
    //void setWindowTitleToConditionName(QString name);
    void checkIfComplete();

  public:
    NewAssociationCondition(QWidget *parent=0);
    ~NewAssociationCondition();

    bool registerCreators(QList<CreateConditionWidget*> conditionCreators);

    Condition* newAssociationCondition();

  public slots:
    void deleteLater();
    void init(Condition *condition);
    void conditionSuggested(Condition *condition);
};
#endif
