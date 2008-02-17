//
// C++ Interface: importgrammarwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTGRAMMARWIZARD_H
#define IMPORTGRAMMARWIZARD_H

#include <simonwizard.h>
class QWizardPage;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportGrammarWizard : public SimonWizard
{
Q_OBJECT
signals:
	void grammarCreated(QStringList);
public:
    ImportGrammarWizard(QWidget* parent);

	QWizardPage* createIntroPage();
	QWizardPage* createSelectFilesPage();
	QWizardPage* createWorkingPage();
	QWizardPage* createFinishedPage();

    ~ImportGrammarWizard();

};

#endif
