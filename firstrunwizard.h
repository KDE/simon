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
	QWizardPage *createCheckList();

	QWizardPage *createExternalProgramsPage();


	QWizardPage *createLanguageModelDescription();
	QWizardPage *createLanguageModelSelectSource();
	QWizardPage *createNewModelDescription();
	QWizardPage *createModelSettings();

	QWizardPage *createFinishedPage();
public:

enum FirstRunWizardPages {
	IntroPage = 0,
	CheckList = 1,
	ExternalPrograms = 2,
	ModelDescription = 3,
	SelectSource = 4,
	NewModelDescription = 5,
	ModelSettingsPage = 6,
	ImportDictionary = 7
};

    FirstRunWizard(QWidget* parent);

    ~FirstRunWizard();

};

#endif
