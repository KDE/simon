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

#include "scriptboundvalue.h"

#include <QDomElement>
#include <QDomDocument>

#include <KLocalizedString>

ScriptBoundValue::ScriptBoundValue(const QString& name) : BoundValue(name)
{
}

ScriptBoundValue::ScriptBoundValue(const QString& name, const QString& script) :
  BoundValue(name), m_script(script)
{
}

QString ScriptBoundValue::getTypeName()
{
  return i18nc("Typename of script bound values", "Script");
}

QVariant ScriptBoundValue::getValue()
{
  return "Fixme";
}

QString ScriptBoundValue::getValueDescription()
{
  return m_script;
}

bool ScriptBoundValue::deSerialize(const QDomElement& elem)
{
  QDomElement scriptElem = elem.firstChildElement("script");

  if (scriptElem.isNull()) return false;

  m_script = scriptElem.text();
  return true;
}

bool ScriptBoundValue::serializePrivate(QDomDocument *doc, QDomElement& elem, int& id)
{
  id = 2;

  QDomElement scriptElem = doc->createElement("script");
  scriptElem.appendChild(doc->createTextNode(m_script));
  
  elem.appendChild(scriptElem);
  return true;
}




