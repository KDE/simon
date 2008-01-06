//
// C++ Interface: firstrunimportbomppage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNIMPORTBOMPPAGE_H
#define FIRSTRUNIMPORTBOMPPAGE_H

#include "importbomppage.h"
#include "firstrunwizard.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class FirstRunImportBOMPPage : public ImportBOMPPage
{
public:
    FirstRunImportBOMPPage(QWidget* parent) : ImportBOMPPage(parent) {}

	int nextId() const
	{ return FirstRunWizard::DictionaryWorkingPage; }

};

#endif
