//
// C++ Interface: renameterminalwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENAMETERMINALWIZARD_H
#define RENAMETERMINALWIZARD_H

#include <QWizard>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class QWizardPage;

class RenameTerminalWizard : public QWizard
{
Q_OBJECT
private:
	QWizardPage *createIntroPage();
	QWizardPage *createSelectParametersPage();
	QWizardPage *createWorkingPage();
	QWizardPage *createFinishedPage();
public:
    RenameTerminalWizard(QWidget* parent);

    ~RenameTerminalWizard();

};

#endif
