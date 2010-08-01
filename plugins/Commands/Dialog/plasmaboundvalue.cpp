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

#include "plasmaboundvalue.h"

#include <QDomElement>
#include <QDomDocument>

#include <KLocalizedString>
#include <Plasma/DataEngine>

PlasmaBoundValue::PlasmaBoundValue(const QString& name) : BoundValue(name)
{
}

PlasmaBoundValue::PlasmaBoundValue(const QString& name, const QString& dataEngine, const QString& dataEngineName,
    const QString& dataSource, const QString& key) :
  BoundValue(name), m_dataEngine(dataEngine), m_dataEngineName(dataEngineName), m_dataSource(dataSource), m_key(key)
{
}

QString PlasmaBoundValue::getTypeName()
{
  return i18nc("Typename of script bound values", "Plasma");
}

QVariant PlasmaBoundValue::getValue()
{
  return "FIXME";
}

QString PlasmaBoundValue::getValueDescription()
{
  return m_dataEngine+'/'+m_dataSource+'/'+m_key;
}

bool PlasmaBoundValue::deSerialize(const QDomElement& elem)
{
  QDomElement dataEngineElem = elem.firstChildElement("dataEngine");
  QDomElement dataSourceElem = elem.firstChildElement("dataSource");
  QDomElement keyElem = elem.firstChildElement("key");

  if (dataEngineElem.isNull() || dataSourceElem.isNull() || keyElem.isNull()) 
    return false;

  m_dataEngine = dataEngineElem.text();
  m_dataEngineName = dataEngineElem.attribute("name");
  m_dataSource = dataSourceElem.text();
  m_key = keyElem.text();
  return true;
}

bool PlasmaBoundValue::serializePrivate(QDomDocument *doc, QDomElement& elem, int& id)
{
  id = 3;

  QDomElement dataEngineElem = doc->createElement("dataEngine");
  dataEngineElem.appendChild(doc->createTextNode(m_dataEngine));
  dataEngineElem.setAttribute("name", m_dataEngineName);
  elem.appendChild(dataEngineElem);

  QDomElement dataSourceElem = doc->createElement("dataSource");
  dataSourceElem.appendChild(doc->createTextNode(m_dataSource));
  elem.appendChild(dataSourceElem);

  QDomElement keyElem = doc->createElement("key");
  keyElem.appendChild(doc->createTextNode(m_key));
  elem.appendChild(keyElem);

  return true;
}





