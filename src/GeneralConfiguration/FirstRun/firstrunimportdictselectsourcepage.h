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

#include "../../ModelManagement/WordList/ImportDict/importdictselectsourcepage.h"
#include "firstrunwizard.h"

/**
 \class FirstRunImportDictSelectSourcePage
 \date 6.1.2008
 \version 0.1
 \author Peter Grasch
 \brief Overwrites the ImportDictSelectSourcePages nextId() to return the appropriate ids
*/
class FirstRunImportDictSelectSourcePage : public ImportDictSelectSourcePage
{
public:
    FirstRunImportDictSelectSourcePage(QWidget* parent) ;

    /**
     * \brief Returns either the id of the bomp- or the wiktionarypage depending on what the user selected
     * \author Peter Grasch
     * @return The id of the next page
     */
    int nextId() const;

};

#endif
