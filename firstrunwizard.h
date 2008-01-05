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

enum FirstRunWizardPages {
	IntroPage=0,
	CheckList=1,
	ExternalPrograms=2,
	ModelDescription=3,
	SelectSource=4,
	ModelSettings=5
};
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class QWizardPage;
class FirstRunWizard : public SimonWizard
{
private:
	QWizardPage *createIntroPage();
	QWizardPage *createCheckList();

	QWizardPage *createExternalProgramsPage();


	QWizardPage *createLanguageModelDescription();
	QWizardPage *createLanguageModelSelectSource();

	QWizardPage *createFinishedPage();
public:
    FirstRunWizard(QWidget* parent);

    ~FirstRunWizard();

};

#endif
