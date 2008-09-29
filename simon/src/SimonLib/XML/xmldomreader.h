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


#ifndef XMLDOMREADER_H
#define XMLDOMREADER_H

#include "xmlreader.h"

class QDomDocument;
/**
	\class XMLDomReader
	
	\author Peter Grasch
	\brief Implements qts DOM API to parse xml documents and implements the XMLReader interface
	\date 12.05.2007
	\version 0.1
 */
class XMLDomReader : public XMLReader {
protected:
	QDomDocument *doc;
public:
	explicit XMLDomReader(QString path, QObject* parent=0);

	bool save(QString path="");
	
	bool load(QString path="");
	
    ~XMLDomReader();

};

#endif
