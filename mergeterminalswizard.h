//
// C++ Interface: mergeterminalswizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MERGETERMINALSWIZARD_H
#define MERGETERMINALSWIZARD_H

#include <QWizard>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class GrammarManager;
class WordListManager;

class QWizardPage;

class MergeTerminalsWizard : public QWizard
{
private:
	QWizardPage* createIntroPage();
	QWizardPage* createSelectTerminalsPage();
	QWizardPage* createWorkingPage();
	QWizardPage* createFinishedPage();
public:
    MergeTerminalsWizard(QWidget* parent);

    ~MergeTerminalsWizard();

};

#endif
