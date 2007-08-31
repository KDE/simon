//
// C++ Implementation: programmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "program.h"
#include "programmanager.h"
#include "programcategory.h"
#include "settings.h"
#include "categoryxmlreader.h"
#include <QVariant>
#include <QMessageBox>

ProgramManager::ProgramManager()
{
	categoryReader = new CategoryXMLReader(Settings::get("PathToProgramCategories").toString());
}


ProgramCategoryList ProgramManager::readCategories()
{
	categoryReader->load();
	ProgramCategoryList *list = categoryReader->getCategoryList();
	if (!list) return ProgramCategoryList();
	else return *list; 
}

const Program* ProgramManager::getProgram(QString name, QString command)
{
	int i=0;
	if (!this->programs) return NULL;

	const Program *found = NULL;
	
	while ((i<programs->count()) && ((programs->at(i).getName() != name) || (programs->at(i).getExec() != command))) {i++; }
	
	if (i != programs->count())
		found = (&(programs->at(i)));
	
	return found;
}

ProgramManager::~ProgramManager()
{
}


