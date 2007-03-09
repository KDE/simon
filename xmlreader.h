//
// C++ Interface: xmlreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class XMLReader{
public:
    XMLReader();

    void parseFile(QString filename);
    ~XMLReader();

};

#endif
