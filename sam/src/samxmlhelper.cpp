/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *  Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "samxmlhelper.h"
#include <QDomElement>
#include <QDomDocument>
#include <KUrlRequester>

void SamXMLHelper::serializePath(QDomDocument* doc, QDomElement& parent, KUrlRequester* requester, const QString& tagName)
{
  serializePath(doc, parent, requester->url(), tagName);
}

void SamXMLHelper::serializePath(QDomDocument* doc, QDomElement& parent, const KUrl &url, const QString& tagName)
{
  QDomElement elem = doc->createElement(tagName);
  elem.appendChild(doc->createTextNode(url.toLocalFile()));
  parent.appendChild(elem);
}

void SamXMLHelper::serializeText(QDomDocument *doc, QDomElement& parent, const QString& text, const QString& tagName)
{
  QDomElement elem = doc->createElement(tagName);
  elem.appendChild(doc->createTextNode(text));
  parent.appendChild(elem);
}

void SamXMLHelper::serializeInt(QDomDocument* doc, QDomElement& parent, int value, const QString& tagName)
{
  QDomElement elem = doc->createElement(tagName);
  elem.appendChild(doc->createTextNode(QString::number(value)));
  parent.appendChild(elem);
}

QString SamXMLHelper::getText(QDomElement parent, const QString& tagName)
{
  return parent.firstChildElement(tagName).text();
}



int SamXMLHelper::getInt(QDomElement parent, const QString& tagName)
{
  return getText(parent, tagName).toInt();
}

