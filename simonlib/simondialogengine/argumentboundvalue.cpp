/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "argumentboundvalue.h"
#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <KLocalizedString>

ArgumentBoundValue::ArgumentBoundValue(const QString& name): BoundValue(name)
{
}

ArgumentBoundValue::ArgumentBoundValue(const QString& name, int argumentNumber): BoundValue(name),
    m_argumentNumber(argumentNumber)
{
}

void ArgumentBoundValue::setArguments(const QStringList& arguments)
{
  if (m_argumentNumber > arguments.count())
    m_currentValue = "";
  else
    m_currentValue = arguments[m_argumentNumber-1];
}


QString ArgumentBoundValue::getTypeName()
{
  return i18n("Argument");
}

QString ArgumentBoundValue::getValueDescription()
{
  return i18n("An argument passed from the recognition");
}

QVariant ArgumentBoundValue::getValue()
{
  return m_currentValue;
}

bool ArgumentBoundValue::serializePrivate(QDomDocument* doc, QDomElement& elem, int& id)
{
  id = 4;

  QDomElement valueElem = doc->createElement("argumentId");
  valueElem.appendChild(doc->createTextNode(QString::number(m_argumentNumber)));
  
  elem.appendChild(valueElem);
  return true;
}

bool ArgumentBoundValue::deSerialize(const QDomElement& elem)
{
  QDomElement valueElem = elem.firstChildElement("argumentId");

  if (valueElem.isNull()) return false;

  bool ok = true;
  m_argumentNumber = valueElem.text().toInt(&ok);
  return ok;
}

