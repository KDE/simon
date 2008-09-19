//
// C++ Interface: xmlsaxreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XMLSAXREADER_H
#define XMLSAXREADER_H

#include <QObject>
#include "xmlreader.h"

class QXmlDefaultHandler;

/**
	\class XMLSAXReader
	
	\author Peter Grasch
	\brief Implements qts SAX API to parse xml documents and implements the XMLReader interface
	\date 12.05.2007
	\version 0.1
*/
class XMLSAXReader : public XMLReader{
public:
	explicit XMLSAXReader(QString path, QObject* parent=0);
	
	void load(QXmlDefaultHandler* handler, QString path="");
    ~XMLSAXReader();

};

#endif
