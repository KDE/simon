/*
 *   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#include "avatar.h"
#include <QDomDocument>
#include <QBuffer>

const char* Avatar::formatString = "PNG"; //!< Set image format string here

Avatar::Avatar(int id, const QString& name, const QImage& image) : m_id(id),
      m_name(name), m_image(image)
{
}

QDomElement Avatar::serialize(QDomDocument* doc)
{
  QDomElement avatarElem = doc->createElement("avatar");
  avatarElem.setAttribute("id", m_id);

  QDomElement nameElem = doc->createElement("name");
  nameElem.appendChild(doc->createTextNode(m_name));
  
  QDomElement imageElem = doc->createElement("image");
  
  QBuffer imageStr;
  imageStr.open(QIODevice::WriteOnly);
  m_image.save(&imageStr, formatString);
  
  imageElem.setAttribute("format", QLatin1String(formatString));
  imageElem.appendChild(doc->createTextNode(imageStr.buffer().toBase64()));
  
  avatarElem.appendChild(nameElem);
  avatarElem.appendChild(imageElem);
  return avatarElem;
}

bool Avatar::deSerialize(const QDomElement& elem)
{
  bool ok = true;
  int id = elem.attribute("id").toInt(&ok);
  if ((id == 0) || !ok)
    return false;

  m_id = id;
  m_name = elem.firstChildElement("name").text();
  QDomElement imageElem = elem.firstChildElement("image");
  QByteArray format = imageElem.attribute("format").toAscii();
  
  QByteArray data = QByteArray::fromBase64(imageElem.text().toAscii());
  m_image.loadFromData(data, format.constData());
  return true;
}

Avatar* Avatar::createInstance(const QDomElement& elem)
{
  Avatar *a = new Avatar;
  if (!a->deSerialize(elem))
  {
    delete a;
    a = 0;
  }
  return a;
}

void Avatar::setName(const QString& name)
{
  m_name = name;
}

void Avatar::setImage(const QImage& image)
{
  m_image = image;
}
