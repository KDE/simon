/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_XMLDOMREADER_H_17AB53AF6FDF4C55A6EF4595D7C030A0
#define SIMON_XMLDOMREADER_H_17AB53AF6FDF4C55A6EF4595D7C030A0

#include "xmlreader.h"
#include "simonxml_export.h"

class QDomDocument;
/**
	\class XMLDomReader
	
	\author Peter Grasch
	\brief Implements qts DOM API to parse xml documents and implements the XMLReader interface
	\date 12.05.2007
	\version 0.1
 */
class SIMONXML_EXPORT XMLDomReader : public XMLReader {
protected:
	QDomDocument *doc;
public:
	explicit XMLDomReader(QString path, QObject* parent=0);

	bool save(QString path="");
	
	bool load(QString path="");
	
    ~XMLDomReader();

};

#endif
