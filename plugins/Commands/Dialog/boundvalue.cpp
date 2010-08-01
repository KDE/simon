/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "boundvalue.h"
#include "staticboundvalue.h"
#include "scriptboundvalue.h"
#include <QDomElement>
#include <KDebug>

BoundValue* BoundValue::createInstance(const QDomElement& elem)
{
  if (elem.isNull()) return 0;

  QDomElement nameElem = elem.firstChildElement("name");
  QString name = nameElem.text();

  BoundValue *value = 0;
  kDebug() << "Bound value type: " << elem.attribute("type").toInt();
  switch (elem.attribute("type").toInt())
  {
    case 1:
      //static
      value = new StaticBoundValue(name);
      break;
    case 2:
      //script
      value = new ScriptBoundValue(name);
      kDebug() << "Creating script bound value";
      break;
    case 3:
      //plasma
      //value = new PlasmaBoundValue(name);
      break;
  }
  if (value && !value->deSerialize(elem))
  {
      kDebug() << "Failed to deserialize";
    delete value;
    value = 0;
  }
  return value;
}

QDomElement BoundValue::serialize(QDomDocument *doc)
{
  int id = 0;
  QDomElement elem = doc->createElement("boundValue");
  QDomElement nameElem = doc->createElement("name");
  nameElem.appendChild(doc->createTextNode(m_name));
  elem.appendChild(nameElem);

  if (!serializePrivate(doc, elem, id))
    return QDomElement();

  elem.setAttribute("type", QString::number(id));
  return elem;
}
