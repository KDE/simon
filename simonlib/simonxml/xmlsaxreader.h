/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_XMLSAXREADER_H_61BBBF20E5A048609AA7213E6A1AAFB7
#define SIMON_XMLSAXREADER_H_61BBBF20E5A048609AA7213E6A1AAFB7

#include <QtCore/QObject>
#include "xmlreader.h"
#include "simonxml_export.h"

class QXmlDefaultHandler;

/**
  \class XMLSAXReader

  \author Peter Grasch
  \brief Implements qts SAX API to parse xml documents and implements the XMLReader interface
  \date 12.05.2007
  \version 0.1
*/
class SIMONXML_EXPORT XMLSAXReader : public XMLReader
{
  public:
    explicit XMLSAXReader(QString path, QObject* parent=0);

    void load(QXmlDefaultHandler* handler, QString path="");
    ~XMLSAXReader();

};
#endif
