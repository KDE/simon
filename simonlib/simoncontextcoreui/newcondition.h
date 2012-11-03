/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_NEWCONDITION_H_CD6234308AA54117A89845ADFE98BCA1
#define SIMON_NEWCONDITION_H_CD6234308AA54117A89845ADFE98BCA1

/** \file newcondition.h
 * \brief The file containing the NewCondition baseclass header.
 */

#include <KDialog>
#include <QList>
#include "simoncontextcoreui_export.h"

class Condition;
class CreateConditionWidget;
namespace Ui {
  class DlgModifyCondition;
}

/**
 *	@class NewCondition
 *	@brief The NewCondition class is a dialog for creating Condition objects that will be monitored by the CompoundCondition of a Scenario
 *
 *      The NewCondition uses CreateConditionWidget objects that have been passed to it from \ref ContextViewPrivate::addCondition()
 *      and uses those CreateConditionWidget objects to allow the user to customize a new Condition that
 *      will be added to the CompoundCondition that was specified for the CreateConditionWidget objects by ContextViewPrivate.
 *
 *      \sa ContextViewPrivate, Condition, CompoundCondition
 *
 *	@version 0.1
 *	@date 7.7.2011
 *	@author Adam Nash
 */

class SIMONCONTEXTCOREUI_EXPORT NewCondition : protected KDialog
{
  Q_OBJECT

  private:
    Ui::DlgModifyCondition *ui;
    QList<CreateConditionWidget*> m_conditionCreators;
    //void switchToTypeOfCondition(Condition* condition);

  private slots:
    //void setWindowTitleToConditionName(QString name);
    void checkIfComplete();

  public:
    NewCondition(QWidget *parent=0);
    ~NewCondition();

    bool registerCreators(QList<CreateConditionWidget*> conditionCreators);

    Condition* newCondition();

  public slots:
    void deleteLater();
    void init(Condition *condition);
    void conditionSuggested(Condition *condition);
};
#endif
