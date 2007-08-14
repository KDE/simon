//
// C++ Implementation: selectsourcewizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "selectsourcewizardpage.h"


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent the parent of the qwizardpage
 */
SelectSourceWizardPage::SelectSourceWizardPage(QWidget *parent) : QWizardPage(parent)
{ }

/**
 * \brief The next id of the wizard
 * \author Peter Grasch
 * If local: 2, else 3
 * @return 
 */
int SelectSourceWizardPage::nextId() const
{
	if (this->local->isChecked())
	{
		return 2;
	} else {
		return 3;
	}
}
