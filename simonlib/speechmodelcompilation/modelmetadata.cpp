/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "modelmetadata.h"

ModelMetadata::ModelMetadata(const QDomElement &DomElement)
{
  this->DeserializeXml(DomElement);
}

QDomElement ModelMetadata::SerializeXml(QDomDocument &DomDocument) const
{
  QDomElement rootElem = DomDocument.createElement("baseModel");

  QDomElement nameElem = DomDocument.createElement("name");
  nameElem.appendChild(DomDocument.createTextNode(m_Name));

  QDomElement creationDateElem = DomDocument.createElement("creationDate");
  creationDateElem.appendChild(DomDocument.createTextNode(m_Time.toString(Qt::ISODate)));

  QDomElement typeElem = DomDocument.createElement("type");
  typeElem.appendChild(DomDocument.createTextNode(m_Type));

  rootElem.appendChild(nameElem);
  rootElem.appendChild(creationDateElem);
  rootElem.appendChild(typeElem);

  return rootElem;
}

bool ModelMetadata::DeserializeXml(const QDomElement &DomElement)
{
  if(DomElement.tagName() != "baseModel") return false;
  QDomElement NameNode = DomElement.firstChildElement("name");
  if(NameNode.isNull())
    return false;
  m_Name = NameNode.text();

  QDomElement TimeNode = DomElement.firstChildElement("creationDate");
  if(TimeNode.isNull())
    return false;
  m_Time = QDateTime::fromString(TimeNode.text(), Qt::ISODate);

  QDomElement TypeNode = DomElement.firstChildElement("type");
  if(TypeNode.isNull())
    return false;

  m_Type = TypeNode.text();

  return true;
}

