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

#include "../../ModelManagement/WordList/ImportDict/importdictbomppage.h"
#include "firstrunwizard.h"

/**
 \class FirstRunImportBOMPPage
 \brief Overwrites the ImportBOMPPages nextId method to return the DictionaryWorkingPage of the FirstRunWizard
 \author Peter Grasch
 \date 6.1.2008
 \version 0.1
*/
class FirstRunImportBOMPPage : public ImportDictBOMPPage
{
public:
    FirstRunImportBOMPPage(QWidget* parent) : ImportDictBOMPPage(parent) {}

	/**
	 * \brief Returns FirstRunWizard::DictionaryWorkingPage
	 * \author Peter Grasch
	 * @return FirstRunWizard::DictionaryWorkingPage
	 */
	int nextId() const
	{ return FirstRunWizard::DictionaryWorkingPage; }

};

#endif
