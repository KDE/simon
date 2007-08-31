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

#include "xmldomreader.h"
#include "programcategory.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class CategoryXMLReader : public XMLDomReader
{
private:
	ProgramCategoryList *categoryList;
public:
    CategoryXMLReader(QString path, QObject* parent=0);

	ProgramCategoryList* getCategoryList() { return this->categoryList; }
    ~CategoryXMLReader();

    void load(QString path="");

};

#endif
