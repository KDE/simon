//
// C++ Implementation: firstrunimportdictwiktionarypage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "firstrunimportdictwiktionarypage.h"
#include "firstrunwizard.h"
#include <QRadioButton>
#include <QLayout>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the object
 */
FirstRunImportDictWiktionaryPage::FirstRunImportDictWiktionaryPage(QWidget* parent): ImportDictWiktionaryPage(parent)
{}


/**
 * \brief Returns the next id in the first-run-wizard
 * @return the id of the working page
 */
int FirstRunImportDictWiktionaryPage::nextId() const
{
    return FirstRunWizard::DictionaryWorkingPage;
}

