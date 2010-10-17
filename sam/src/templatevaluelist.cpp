/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "templatevaluelist.h"

TemplateValueList::TemplateValueList(const QString& id) : m_id(id)
{
}

void TemplateValueList::add(const QString& key,const QString& value)
{
  m_values.insert(key, value);
}

QHash<QString,QString> TemplateValueList::values()
{
  return m_values;
}

bool TemplateValueList::hasChildren() const
{
  return !m_children.isEmpty();
}

void TemplateValueList::addChild(TemplateValueList* value)
{
  m_children << value;
}

QList<TemplateValueList*> TemplateValueList::children() const
{
  return m_children;
}

TemplateValueList::~TemplateValueList()
{
  qDeleteAll(m_children);
}

