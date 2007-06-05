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


SelectSourceWizardPage::SelectSourceWizardPage(QWidget *parent) : QWizardPage(parent)
{ }

int SelectSourceWizardPage::nextId() const
{
	if (this->local->isChecked())
	{
		return 2;
	} else {
		return 3;
	}
}
