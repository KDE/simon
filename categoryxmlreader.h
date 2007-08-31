//
// C++ Interface: categoryxmlreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CATEGORYXMLREADER_H
#define CATEGORYXMLREADER_H

#include <xmldomreader.h>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class CategoryXMLReader : public XMLDomReader
{
public:
    CategoryXMLReader(QString path, QObject* parent);

    ~CategoryXMLReader();

    int save(QString path);
    void load(QString path);

};

#endif
