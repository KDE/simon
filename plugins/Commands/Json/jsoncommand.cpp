/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "jsoncommand.h"
#include "jsoncommandmanager.h"
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>

#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

const QString JsonCommand::staticCategoryText()
{
  return i18n("JSON");
}


const QString JsonCommand::getCategoryText() const
{
  return JsonCommand::staticCategoryText();
}


const KIcon JsonCommand::staticCategoryIcon()
{
  return KIcon("network-connect");
}


const KIcon JsonCommand::getCategoryIcon() const
{
  return JsonCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> JsonCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18nc("Url of the call", "Url"), m_url);
  out.insert(i18nc("The request itself (\"function\")", "Request"), m_request);
  return out;
}


bool JsonCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  return static_cast<JsonCommandManager*>(parent())->sendRequest(m_url, m_request);
}


QDomElement JsonCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement urlElem = doc->createElement("url");
  urlElem.appendChild(doc->createTextNode(m_url));

  QDomElement requestElem = doc->createElement("request");
  requestElem.appendChild(doc->createTextNode(m_request));

  commandElem.appendChild(urlElem);
  commandElem.appendChild(requestElem);

  return commandElem;
}


bool JsonCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement urlElem = commandElem.firstChildElement("url");
  QDomElement requestElem = commandElem.firstChildElement("request");

  if (urlElem.isNull() || requestElem.isNull()) return false;

  m_url = urlElem.text();
  m_request = requestElem.text();

  return true;
}

QString JsonCommand::url()
{
  return m_url;
}

QString JsonCommand::request()
{
  return m_request;
}

STATIC_CREATE_INSTANCE_C(JsonCommand);
