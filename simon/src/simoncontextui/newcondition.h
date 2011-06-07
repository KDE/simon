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

#ifndef SIMON_NEWCONDITION_H_CD6234308AA54117A89845ADFE98BCA1
#define SIMON_NEWCONDITION_H_CD6234308AA54117A89845ADFE98BCA1

#include <KDE/KDialog>
#include <QList>
#include "ui_modifycondition.h"

class Condition;
class CreateConditionWidget;

class NewCondition : protected KDialog
{
  Q_OBJECT

  private:
    Ui::DlgModifyCondition ui;
    QList<CreateConditionWidget*> *m_conditionCreators;
    //void switchToTypeOfCondition(Condition* condition);

  private slots:
    //void setWindowTitleToConditionName(QString name);
    void checkIfComplete();

  public:
    NewCondition(QWidget *parent=0);
    ~NewCondition();

    bool registerCreators(QList<CreateConditionWidget*>* conditionCreators);

    bool newCondition();

  public slots:
    void deleteLater();
    void init(Condition *condition);
    void conditionSuggested(Condition *condition);
};
#endif
