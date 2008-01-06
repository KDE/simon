//
// C++ Interface: firstrunimportdictselectsourcepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNIMPORTDICTSELECTSOURCEPAGE_H
#define FIRSTRUNIMPORTDICTSELECTSOURCEPAGE_H

#include "importdictselectsourcepage.h"
#include "firstrunwizard.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class FirstRunImportDictSelectSourcePage : public ImportDictSelectSourcePage
{
public:
    FirstRunImportDictSelectSourcePage(QWidget* parent) 
		: ImportDictSelectSourcePage (parent) {}

    int nextId() const {
		if (field("hadifix").toBool())
		{
			return FirstRunWizard::DictionaryBOMPPage;
		} else return FirstRunWizard::DictionaryWiktionaryPage;
    }

};

#endif
