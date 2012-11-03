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

#include "dbuscondition.h"
#include "createdbusconditionwidget.h"
#include <QWidget>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusConnection>
#include <KDebug>

//could be made into proper methods if conditions are refactored to use the
//qt property system to store configuration values (makes attributes
//accessible by name)

#define deSerializeTextElem(tagName, elementName) \
    {QDomElement qElem = elem.firstChildElement(tagName); \
    if (qElem.isNull()) { \
        kDebug() << "No element " << tagName << " found!  Deserialization failure!"; \
        return false; \
    } \
    elementName = qElem.text();}

#define serializeTextElem(tagName, elementName) \
    {QDomElement pathElem = doc->createElement(tagName); \
    pathElem.appendChild(doc->createTextNode(elementName)); \
    elem.appendChild(pathElem);}

K_PLUGIN_FACTORY( DBusConditionPluginFactory,
                  registerPlugin< DBusCondition >();
                )

K_EXPORT_PLUGIN( DBusConditionPluginFactory("simondbuscondition") )


DBusCondition::DBusCondition(QObject *parent, const QVariantList &args) :
  Condition(parent, args), m_stateConnection(0)
{
  m_pluginName = "simondbusconditionplugin.desktop";
}

CreateConditionWidget* DBusCondition::getCreateConditionWidget( QWidget* parent )
{
  return new CreateDBusConditionWidget(parent);
}

QDomElement DBusCondition::privateSerialize(QDomDocument *doc, QDomElement elem)
{
  serializeTextElem("serviceName", m_serviceName);
  serializeTextElem("statePath", m_statePath);
  serializeTextElem("stateInterface", m_stateInterface);
  serializeTextElem("stateCheckMethod", m_stateCheckMethod);
  
  QDomElement stateArgumentsElem = doc->createElement("stateArguments");
  foreach (const QString& attr, m_stateArguments) {
    QDomElement attrElem = doc->createElement("argument");
    attrElem.appendChild(doc->createTextNode(attr));
    stateArgumentsElem.appendChild(attrElem);
  }
  elem.appendChild(stateArgumentsElem);
  
  serializeTextElem("value", m_value);
  
  serializeTextElem("notificationPath", m_notificationPath);
  serializeTextElem("notificationInterface", m_notificationInterface);
  serializeTextElem("notificationSignal", m_notificationSignal);

  return elem;
}

QString DBusCondition::name()
{
  if (isInverted())
    return i18nc("%1...Check method name, %2...Service name, %3...value to match",
                 "Method '%1' of service '%2' does not evaluate to '%3'", m_stateCheckMethod, m_serviceName,
                 m_value);
  else
    return i18nc("%1...Check method name, %2...Service name, %3...value to match",
                 "Method '%1' of service '%2' evaluates to '%3'", m_stateCheckMethod, m_serviceName,
                 m_value);
}

void DBusCondition::check()
{
  kDebug() << "Checking";

  bool newSatisfied = false;
  QList<QVariant> args;
  foreach (const QString& a, m_stateArguments)
    args << a;
  QDBusReply<QVariant> reply = m_stateConnection->callWithArgumentList(QDBus::BlockWithGui, m_stateCheckMethod, args);
  kDebug() << reply << reply.isValid();
  if (reply.isValid())
    newSatisfied = reply.value().toString() == m_value;
  
  bool changed = (newSatisfied != m_satisfied);
  m_satisfied = newSatisfied;
  kDebug() << "Satisfied: " << m_satisfied;

  if (changed)
    emit conditionChanged();
}

bool DBusCondition::privateDeSerialize(QDomElement elem)
{
  deSerializeTextElem("serviceName", m_serviceName);
  deSerializeTextElem("statePath", m_statePath);
  deSerializeTextElem("stateInterface", m_stateInterface);
  deSerializeTextElem("stateCheckMethod", m_stateCheckMethod);
  deSerializeTextElem("value", m_value);
  
  deSerializeTextElem("notificationPath", m_notificationPath);
  deSerializeTextElem("notificationInterface", m_notificationInterface);
  deSerializeTextElem("notificationSignal", m_notificationSignal);
  
  QDomElement stateArgumentsElem = elem.firstChildElement("stateArguments");
  m_stateArguments.clear();
  QDomElement stateArgElem = stateArgumentsElem.firstChildElement("argument");
  while (!stateArgElem.isNull()) {
    m_stateArguments << stateArgElem.text();
    stateArgElem = stateArgElem.nextSiblingElement("argument");
  }
  
  delete m_stateConnection;

  m_stateConnection = new QDBusInterface(m_serviceName,
                                               m_statePath,
                                               m_stateInterface, 
                                               QDBusConnection::sessionBus(),
                                               this); 
  
  QDBusConnection::sessionBus().connect(m_serviceName, m_notificationPath, m_notificationInterface, 
                                          m_notificationSignal, this, SLOT(check()));
  
  check();

  return true;
}
