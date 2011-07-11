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

#include "createprocessopenedconditionwidget.h"
#include "processopenedcondition.h"
#include <QLineEdit>
#include "simoncontextdetection/contextmanager.h"

CreateProcessOpenedConditionWidget::CreateProcessOpenedConditionWidget(CompoundCondition *compoundCondition,
QWidget *parent) : CreateConditionWidget(compoundCondition, parent)
{
  ui.setupUi(this);

  //setWindowIcon();
  setWindowTitle("Process Opened Condition");

  connect(ui.leProgramName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}


bool CreateProcessOpenedConditionWidget::isComplete()
{
  return !(ui.leProgramName->text().isEmpty());
}

bool CreateProcessOpenedConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  ProcessOpenedCondition *procOpenCondition = dynamic_cast<ProcessOpenedCondition*>(condition);
  if (!procOpenCondition) return false;

  ui.leProgramName->setText(procOpenCondition->getName());
  return true;
}


Condition* CreateProcessOpenedConditionWidget::createCondition()
{
    kDebug() << "Creating Process Opened Condition";
    QDomDocument doc;
    QDomElement conditionElem = doc.createElement("condition");
    conditionElem.setAttribute("name", "simonprocessopenedconditionplugin.desktop");

    QDomElement invertElem = doc.createElement("inverted");
    invertElem.appendChild(doc.createTextNode("0"));
    conditionElem.appendChild(invertElem);

    QDomElement openElem = doc.createElement("processname");
    openElem.appendChild(doc.createTextNode(ui.leProgramName->text()));
    conditionElem.appendChild(openElem);

    ContextManager* manager = ContextManager::instance();

    return manager->getCondition(conditionElem);
}


CreateProcessOpenedConditionWidget::~CreateProcessOpenedConditionWidget()
{
}
