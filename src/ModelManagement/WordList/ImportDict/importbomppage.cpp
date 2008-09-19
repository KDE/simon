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
#include "importbomppage.h"
#include <QLabel>
#include <KLineEdit>
#include <KPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QVariant>


/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportBOMPPage::ImportBOMPPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);

	registerField("bompFileName*", ui.urFile, "url", SIGNAL(textChanged (const QString &)));
	setTitle(i18n("Importiere BOMP Wörterbuch"));
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportBOMPPage::~ImportBOMPPage()
{
}


