//
// C++ Interface: firstrunimportdictwiktionarypage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNIMPORTDICTWIKTIONARYPAGE_H
#define FIRSTRUNIMPORTDICTWIKTIONARYPAGE_H

#include "importdictwiktionarypage.h"

/**
 \class FirstRunImportDictWiktionaryPage
 \brief Overrides the nextId for use in the firstrunwizard
 \author Peter Grasch
 \version 0.1
 \date 17.1.2008
*/
class FirstRunImportDictWiktionaryPage : public ImportDictWiktionaryPage
{
public:
    FirstRunImportDictWiktionaryPage(QWidget* parent);

    int nextId() const;

};

#endif
