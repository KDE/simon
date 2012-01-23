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

#ifndef SIMON_SAMXMLHELPER_H_4002119636CC42C68FE07273F9000A73
#define SIMON_SAMXMLHELPER_H_4002119636CC42C68FE07273F9000A73

class QDomDocument;
class QDomElement;
class KUrlRequester;
class QString;

class SamXMLHelper
{
public:
  static void serializePath(QDomDocument* doc, QDomElement& parent, KUrlRequester* requester, const QString& tagName);
  static void serializeText(QDomDocument* doc, QDomElement& parent, const QString& text, const QString& tagName);
  static void serializeInt(QDomDocument* doc, QDomElement& parent, int value, const QString& tagName);
  
  static QString getText(QDomElement parent, const QString& tagName);
  static int getInt(QDomElement parent, const QString& tagName);
  
};

#endif

