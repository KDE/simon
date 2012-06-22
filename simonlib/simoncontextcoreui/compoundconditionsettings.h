/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#ifndef COMPOUNDCONDITIONSETTINGS_H
#define COMPOUNDCONDITIONSETTINGS_H

#include <QWidget>
#include "simoncontextcoreui_export.h"

class CompoundCondition;
class QSortFilterProxyModel;
class Condition;

namespace Ui {
  class CompoundConditionSettings;
}

class SIMONCONTEXTCOREUI_EXPORT CompoundConditionSettings : public QWidget
{
  Q_OBJECT
  

public:
  explicit CompoundConditionSettings ( QWidget* parent = 0, Qt::WindowFlags f = 0 );
  void setConditions(CompoundCondition *c);
    
private slots:
  void addCondition();
  void deleteCondition();
  void editCondition();
  void selectionChanged();
  
private:
  Ui::CompoundConditionSettings *ui;
  
  CompoundCondition *m_conditions;
  QSortFilterProxyModel *conditionsProxy;
  
  Condition* getCurrentCondition();
};

#endif // COMPOUNDCONDITIONSETTINGS_H
