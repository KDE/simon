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

#include "../XML/xmldomreader.h"
#include "programcategory.h"

/**
 \class CategoryXMLReader
 \author Peter Grasch
 \version 0.1
 \date 31.8.2007
 \brief Reads the programcategories from the xml file
*/
class CategoryXMLReader : public XMLDomReader
{
private:
	ProgramCategoryList *categoryList;
public:
    CategoryXMLReader(QString path, QObject* parent=0);
    ~CategoryXMLReader();

	ProgramCategoryList* getCategoryList() { return this->categoryList; }

    void load(QString path="");

};

#endif
