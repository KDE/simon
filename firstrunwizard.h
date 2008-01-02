//
// C++ Interface: firstrunwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNWIZARD_H
#define FIRSTRUNWIZARD_H

#include "simonwizard.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class QWizardPage;
class FirstRunWizard : public SimonWizard
{
private:
	QWizardPage *createIntroPage();

	QWizardPage *createFinishedPage();
public:
    FirstRunWizard(QWidget* parent);

    ~FirstRunWizard();

};

#endif
