/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "createdbusconditionwidget.h"
#include "dbuscondition.h"

#include <simoncontextdetection/contextmanager.h>

#include <QLineEdit>
#include <QFileDialog>
#include <QStringList>

CreateDBusConditionWidget::CreateDBusConditionWidget(CompoundCondition *compoundCondition,
QWidget *parent) : CreateConditionWidget(compoundCondition, parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("D-Bus Condition"));
  setWindowIcon(KIcon("network-connect"));

  connect(ui.leServiceName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.lePath, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leInterface, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leCheckMethod, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leNotificationSignal, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leValue, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}

bool CreateDBusConditionWidget::isComplete()
{
    return !(ui.leServiceName->text().isEmpty() || ui.lePath->text().isEmpty() ||
             ui.leInterface->text().isEmpty() || ui.leCheckMethod->text().isEmpty() ||
             ui.leNotificationSignal->text().isEmpty() || ui.leValue->text().isEmpty());
}

bool CreateDBusConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  DBusCondition *dbusCondition = dynamic_cast<DBusCondition*>(condition);
  if (!dbusCondition) return false;

  ui.leServiceName->setText(dbusCondition->serviceName());
  ui.lePath->setText(dbusCondition->path());
  ui.leInterface->setText(dbusCondition->interface());
  ui.leCheckMethod->setText(dbusCondition->checkMethod());
  ui.leNotificationSignal->setText(dbusCondition->notificationSignal());
  ui.leValue->setText(dbusCondition->value());
  
  ui.cbInverted->setChecked(dbusCondition->isInverted());
  return true;
}


Condition* CreateDBusConditionWidget::createCondition()
{
    QDomDocument doc;
    
    QDomElement conditionElem = doc.createElement("condition");
    conditionElem.setAttribute("name", "simondbusconditionplugin.desktop");

    QDomElement invertElem = doc.createElement("inverted");
    invertElem.appendChild(doc.createTextNode(ui.cbInverted->isChecked() ? "1" : "0"));
    conditionElem.appendChild(invertElem);

    QDomElement serviceNameElem = doc.createElement("serviceName");
    serviceNameElem.appendChild(doc.createTextNode(ui.leServiceName->text()));
    conditionElem.appendChild(serviceNameElem);

    QDomElement pathElem = doc.createElement("path");
    pathElem.appendChild(doc.createTextNode(ui.lePath->text()));
    conditionElem.appendChild(pathElem);

    QDomElement interfaceElem = doc.createElement("interface");
    interfaceElem.appendChild(doc.createTextNode(ui.leInterface->text()));
    conditionElem.appendChild(interfaceElem);

    QDomElement checkMethodElem = doc.createElement("checkMethod");
    checkMethodElem.appendChild(doc.createTextNode(ui.leCheckMethod->text()));
    conditionElem.appendChild(checkMethodElem);

    QDomElement notificationSignalElem = doc.createElement("notificationSignal");
    notificationSignalElem.appendChild(doc.createTextNode(ui.leNotificationSignal->text()));
    conditionElem.appendChild(notificationSignalElem);

    QDomElement valueElem = doc.createElement("value");
    valueElem.appendChild(doc.createTextNode(ui.leValue->text()));
    conditionElem.appendChild(valueElem);
    
    return ContextManager::instance()->getCondition(conditionElem);
}


CreateDBusConditionWidget::~CreateDBusConditionWidget()
{
}
