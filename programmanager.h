//
// C++ Interface: programmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include "program.h"
#include "programcategory.h"

class Program;
class QString;
class CategoryXMLReader;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ProgramManager{
private:
	ProgramList *programs;
	CategoryXMLReader *categoryReader;
public:
	virtual ProgramList *getPrograms(ProgramCategory category)=0;
	ProgramCategoryList readCategories();
	const Program* getProgram(QString name, QString command);
    ProgramManager();

    ~ProgramManager();

};

#endif
