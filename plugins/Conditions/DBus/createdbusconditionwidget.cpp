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

#include "createdbusconditionwidget.h"
#include "dbuscondition.h"

#include <simoncontextdetection/contextmanager.h>


CreateDBusConditionWidget::CreateDBusConditionWidget(QWidget *parent) : CreateConditionWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("D-Bus Condition"));
  setWindowIcon(QIcon::fromTheme("network-connect"));

  connect(ui.leServiceName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leStatePath, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leStateInterface, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leStateCheckMethod, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leNotificationPath, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leNotificationInterface, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leNotificationSignal, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leValue, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}

bool CreateDBusConditionWidget::isComplete()
{
  return !(ui.leServiceName->text().isEmpty() || ui.leStatePath->text().isEmpty()
           || ui.leStateInterface->text().isEmpty() || ui.leStateCheckMethod->text().isEmpty()
           || ui.leNotificationPath->text().isEmpty() || ui.leNotificationInterface->text().isEmpty()
           || ui.leNotificationSignal->text().isEmpty() || ui.leValue->text().isEmpty());
}

bool CreateDBusConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  DBusCondition *dbusCondition = dynamic_cast<DBusCondition*>(condition);

  if (!dbusCondition) return false;

  ui.leServiceName->setText(dbusCondition->serviceName());

  ui.leStatePath->setText(dbusCondition->statePath());
  ui.leStateInterface->setText(dbusCondition->stateInterface());
  ui.leStateCheckMethod->setText(dbusCondition->stateCheckMethod());
  ui.elwStateArguments->setItems(dbusCondition->stateArguments());
  ui.leValue->setText(dbusCondition->value());

  ui.leNotificationPath->setText(dbusCondition->notificationPath());
  ui.leNotificationInterface->setText(dbusCondition->notificationInterface());
  ui.leNotificationSignal->setText(dbusCondition->notificationSignal());
  return true;
}

Condition* CreateDBusConditionWidget::createCondition(QDomDocument* doc, QDomElement& conditionElem)
{
  conditionElem.setAttribute("name", "simondbusconditionplugin.desktop");

  QDomElement serviceNameElem = doc->createElement("serviceName");
  serviceNameElem.appendChild(doc->createTextNode(ui.leServiceName->text()));
  conditionElem.appendChild(serviceNameElem);

  QDomElement pathElem = doc->createElement("statePath");
  pathElem.appendChild(doc->createTextNode(ui.leStatePath->text()));
  conditionElem.appendChild(pathElem);

  QDomElement interfaceElem = doc->createElement("stateInterface");
  interfaceElem.appendChild(doc->createTextNode(ui.leStateInterface->text()));
  conditionElem.appendChild(interfaceElem);

  QDomElement checkMethodElem = doc->createElement("stateCheckMethod");
  checkMethodElem.appendChild(doc->createTextNode(ui.leStateCheckMethod->text()));
  conditionElem.appendChild(checkMethodElem);


  QDomElement stateArgumentsElem = doc->createElement("stateArguments");
  foreach (const QString & attr, ui.elwStateArguments->items()) {
    QDomElement attrElem = doc->createElement("argument");
    attrElem.appendChild(doc->createTextNode(attr));
    stateArgumentsElem.appendChild(attrElem);
  }
  conditionElem.appendChild(stateArgumentsElem);

  QDomElement valueElem = doc->createElement("value");
  valueElem.appendChild(doc->createTextNode(ui.leValue->text()));
  conditionElem.appendChild(valueElem);

  QDomElement notificationPathElem = doc->createElement("notificationPath");
  notificationPathElem.appendChild(doc->createTextNode(ui.leNotificationPath->text()));
  conditionElem.appendChild(notificationPathElem);
  QDomElement notificationInterfaceElem = doc->createElement("notificationInterface");
  notificationInterfaceElem.appendChild(doc->createTextNode(ui.leNotificationInterface->text()));
  conditionElem.appendChild(notificationInterfaceElem);
  QDomElement notificationSignalElem = doc->createElement("notificationSignal");
  notificationSignalElem.appendChild(doc->createTextNode(ui.leNotificationSignal->text()));
  conditionElem.appendChild(notificationSignalElem);

  return ContextManager::instance()->getCondition(conditionElem);
}


CreateDBusConditionWidget::~CreateDBusConditionWidget()
{
}
