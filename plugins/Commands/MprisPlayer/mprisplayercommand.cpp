/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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

#include "mprisplayercommand.h"

#include <QObject>
#include <QVariant>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusInterface>

#include <KIcon>
#include <KDebug>
#include <KLocalizedString>

const QString MprisPlayerCommand::staticCategoryText()
{
    return i18n("Media Player Control");
}

const QString MprisPlayerCommand::getCategoryText() const
{
    return MprisPlayerCommand::staticCategoryText();
}

const KIcon MprisPlayerCommand::staticCategoryIcon()
{
    return KIcon("applications-multimedia");
}

const KIcon MprisPlayerCommand::getCategoryIcon() const
{
    return MprisPlayerCommand::staticCategoryIcon();
}

QString MprisPlayerCommand::serviceName()
{
    return m_serviceName;
}

QString MprisPlayerCommand::path()
{
    return m_path;
}

QString MprisPlayerCommand::interface()
{
    return m_interface;
}

QString MprisPlayerCommand::method()
{
    return m_method;
}

QStringList MprisPlayerCommand::arguments()
{
    return m_args;
}

QDomElement MprisPlayerCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement serviceNameElem = doc->createElement("serviceName");
  serviceNameElem.appendChild(doc->createTextNode(m_serviceName));
  QDomElement pathElem = doc->createElement("path");
  pathElem.appendChild(doc->createTextNode(m_path));
  QDomElement interfaceElem = doc->createElement("interface");
  interfaceElem.appendChild(doc->createTextNode(m_interface));
  QDomElement methodElem = doc->createElement("method");
  methodElem.appendChild(doc->createTextNode(m_method));

  QDomElement argsElem = doc->createElement("arguments");

  foreach (const QString& a, m_args)
  {
    QDomElement argElem = doc->createElement("argument");
    argElem.appendChild(doc->createTextNode(a));
    argsElem.appendChild(argElem);
  }

  commandElem.appendChild(serviceNameElem);
  commandElem.appendChild(pathElem);
  commandElem.appendChild(interfaceElem);
  commandElem.appendChild(methodElem);
  commandElem.appendChild(argsElem);

  return commandElem;
}


bool MprisPlayerCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement serviceNameElem = commandElem.firstChildElement("serviceName");
  QDomElement pathElem = commandElem.firstChildElement("path");
  QDomElement interfaceElem = commandElem.firstChildElement("interface");
  QDomElement methodElem = commandElem.firstChildElement("method");
  QDomElement argsElem = commandElem.firstChildElement("arguments");

  if (serviceNameElem.isNull() || pathElem.isNull() || interfaceElem.isNull()
      || methodElem.isNull() || argsElem.isNull())
    return false;

  m_serviceName = serviceNameElem.text();
  m_path = pathElem.text();
  m_interface = interfaceElem.text();
  m_method = methodElem.text();

  QDomElement argElem = argsElem.firstChildElement("argument");
  m_args.clear();
  while (!argElem.isNull())
  {
    m_args << argElem.text();
    argElem = argElem.nextSiblingElement("argument");
  }
  return true;
}

const QMap<QString,QVariant> MprisPlayerCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18nc("Name of the service", "Service Name"), m_serviceName);
  out.insert(i18nc("Name of the path", "Path"), m_path);
  out.insert(i18nc("Name of the D-Bus interface", "Interface"), m_interface);
  out.insert(i18nc("Name of the method to call", "Method"), m_method);
  out.insert(i18nc("Arguments of the call", "Arguments"), m_args.join(", "));
  return out;
}

bool MprisPlayerCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  QDBusMessage m = QDBusMessage::createMethodCall(m_serviceName,
                                                    m_path,
                                                    m_interface,
                                                    m_method);
  QList<QVariant> args;
  foreach (QString arg, m_args) { // krazy:exclude=foreach
    int i=0;
    while ((arg.contains("%")) && (i < currentArguments().count())) {
      arg = arg.arg(currentArguments()[i++]);
    }

    //check for integers
    bool ok = true;
    int a = arg.toInt(&ok);
    if (ok)
      args.append(QVariant(a));
    else
      args.append(arg);
  }

  kDebug() << args;
  m.setArguments(args);
  return QDBusConnection::sessionBus().send(m);
}

STATIC_CREATE_INSTANCE_C(MprisPlayerCommand)
