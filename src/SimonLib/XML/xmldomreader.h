//
// C++ Interface: xmldomreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
    QString path;
public:
	explicit XMLDomReader(QString path, QObject* parent=0);

	bool save(QString path="");
	
	bool load(QString path="");
	
    ~XMLDomReader();

};

#endif
