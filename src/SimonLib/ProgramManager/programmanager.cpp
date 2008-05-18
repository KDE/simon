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
#include "../Settings/settings.h"
#include "categoryxmlreader.h"
#include <QMessageBox>

ProgramManager::ProgramManager()
{
	categoryReader = new CategoryXMLReader(Settings::getS("PathToProgramCategories"));
	this->programs = new ProgramList();
}


ProgramCategoryList ProgramManager::readCategories()
{
	categoryReader->load();
	ProgramCategoryList *list = categoryReader->getCategoryList();
	if (!list) this->categories = new ProgramCategoryList();
	else this->categories = list; 
	return (*(this->categories));
}

ProgramCategory* ProgramManager::getCategory(QString name)
{
	if (!this->categories) return NULL;
	
	int i=0;
	while ((i<categories->count()) && (this->categories->at(i).getName() != name))
	{ i++; }
	
	if (i != categories->count())
		return new ProgramCategory(this->categories->at(i));

	return NULL;
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


