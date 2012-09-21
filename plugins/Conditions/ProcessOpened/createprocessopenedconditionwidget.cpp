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
#include <simoncontextdetection/contextmanager.h>
#include <simonuicomponents/selectprogramdialog.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QStringList>

CreateProcessOpenedConditionWidget::CreateProcessOpenedConditionWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("Process Opened Condition"));
  setWindowIcon(KIcon("view-process-all"));

  connect(ui.leProgramName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.pbLocateProgram, SIGNAL(clicked()), this, SLOT(processFileDialog()));
}


void CreateProcessOpenedConditionWidget::processFileDialog()
{
    SelectProgramDialog *dlg = new SelectProgramDialog(this);

    if (dlg->selectCommand())
    {
        ui.leProgramName->setText(dlg->getExecPath().split(QRegExp("/+|\\\\+")).back());
    }

    dlg->deleteLater();
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


Condition* CreateProcessOpenedConditionWidget::createCondition(QDomDocument* doc, QDomElement& conditionElem)
{
    conditionElem.setAttribute("name", "simonprocessopenedconditionplugin.desktop");
    
    QDomElement openElem = doc->createElement("processname");
    openElem.appendChild(doc->createTextNode(ui.leProgramName->text()));
    conditionElem.appendChild(openElem);

    return ContextManager::instance()->getCondition(conditionElem);
}


CreateProcessOpenedConditionWidget::~CreateProcessOpenedConditionWidget()
{
}
