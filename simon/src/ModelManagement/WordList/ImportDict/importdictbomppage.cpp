//
// C++ Implementation: importbomppage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictbomppage.h"


/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportDictBOMPPage::ImportDictBOMPPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);

	registerField("bompFileName*", ui.urFile, "url", SIGNAL(textChanged (const QString &)));
	setTitle(i18n("Importiere BOMP Wörterbuch"));
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictBOMPPage::~ImportDictBOMPPage()
{
}


