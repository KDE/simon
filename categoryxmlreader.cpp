//
// C++ Implementation: categoryxmlreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "categoryxmlreader.h"

CategoryXMLReader::CategoryXMLReader(QString path, QObject* parent): XMLDomReader(path, parent)
{
}


CategoryXMLReader::~CategoryXMLReader()
{
}


int CategoryXMLReader::save(QString path)
{
    return XMLDomReader::save(path);
}

void CategoryXMLReader::load(QString path)
{
    XMLDomReader::load(path);
}

