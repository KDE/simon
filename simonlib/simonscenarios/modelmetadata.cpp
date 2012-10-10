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

ModelMetadata::ModelMetadata(const QDomElement &domElement)
{
  this->deserializeXml(domElement);
}

QDomElement ModelMetadata::serializeXml(QDomDocument &domDocument) const
{
  QDomElement rootElem = domDocument.createElement("baseModel");
  domDocument.appendChild(rootElem);

  QDomElement nameElem = domDocument.createElement("name");
  nameElem.appendChild(domDocument.createTextNode(m_name));

  QDomElement creationDateElem = domDocument.createElement("creationDate");
  creationDateElem.appendChild(domDocument.createTextNode(m_time.toString(Qt::ISODate)));

  QDomElement typeElem = domDocument.createElement("type");
  typeElem.appendChild(domDocument.createTextNode(m_type));

  QDomElement droppedPronunciationsElem = domDocument.createElement("droppedPronunciations");
  foreach (const QString& pronunciation, m_droppedPronunciations) {
    QDomElement droppedPronunciationElem = domDocument.createElement("pronunciation");
    droppedPronunciationElem.appendChild(domDocument.createTextNode(pronunciation));
    droppedPronunciationsElem.appendChild(droppedPronunciationElem);
  }

  rootElem.appendChild(nameElem);
  rootElem.appendChild(creationDateElem);
  rootElem.appendChild(typeElem);
  rootElem.appendChild(droppedPronunciationsElem);

  return rootElem;
}

bool ModelMetadata::deserializeXml(const QDomElement &domElement)
{
  if(domElement.tagName() != "baseModel") return false;
  QDomElement nameNode = domElement.firstChildElement("name");
  if(nameNode.isNull())
    return false;
  m_name = nameNode.text();

  QDomElement timeNode = domElement.firstChildElement("creationDate");
  if(timeNode.isNull())
    return false;
  m_time = QDateTime::fromString(timeNode.text(), Qt::ISODate);

  QDomElement typeNode = domElement.firstChildElement("type");
  if(typeNode.isNull())
    return false;
  m_type = typeNode.text();

  m_droppedPronunciations.clear();
  QDomElement droppedPronunciationsElem = domElement.firstChildElement("droppedPronunciations");
  QDomElement droppedPronunciationElem = droppedPronunciationsElem.firstChildElement("pronunciation");
  while (!droppedPronunciationElem.isNull()) {
    m_droppedPronunciations << droppedPronunciationElem.text();
    droppedPronunciationElem = droppedPronunciationElem.nextSiblingElement("pronunciation");
  }

  return true;
}

