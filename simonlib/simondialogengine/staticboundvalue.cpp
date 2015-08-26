/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "staticboundvalue.h"

#include <QDomElement>
#include <QDomDocument>

#include <KI18n/klocalizedstring.h>

StaticBoundValue::StaticBoundValue(const QString& name) : BoundValue(name)
{
}

StaticBoundValue::StaticBoundValue(const QString& name, const QVariant& value) :
  BoundValue(name), m_value(value)
{
}

QString StaticBoundValue::getTypeName()
{
  return i18nc("Typename of static bound values", "Static");
}

QVariant StaticBoundValue::getValue()
{
  return m_value;
}

QString StaticBoundValue::getValueDescription()
{
  return m_value.toString();
}

bool StaticBoundValue::deSerialize(const QDomElement& elem)
{
  QDomElement valueElem = elem.firstChildElement("value");

  if (valueElem.isNull()) return false;

  m_value = valueElem.attribute("content");
  return true;
}

bool StaticBoundValue::serializePrivate(QDomDocument *doc, QDomElement& elem, int& id)
{
  id = 1;

  QDomElement valueElem = doc->createElement("value");
  valueElem.setAttribute("content", m_value.toString());
  
  elem.appendChild(valueElem);
  return true;
}



