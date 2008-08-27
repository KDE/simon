//
// C++ Interface: mergeterminalworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MERGETERMINALWORKINGPAGE_H
#define MERGETERMINALWORKINGPAGE_H

#include <QWizardPage>
#include "ui_mergeterminalsworkingpage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class MergeTerminals;
class MergeTerminalsWorkingPage : public QWizardPage
{
Q_OBJECT
signals:
	void done();
private:
	Ui::MergeTerminalsWorkingPage ui;
	MergeTerminals *mergeTerminals;
	bool complete;
private slots:
	void displayProgress(int progress, int max);
	void displayStatus(QString status);
	void finished();

public:
    MergeTerminalsWorkingPage(QWidget* parent);
	bool isComplete() const { return this->complete; }
	void initializePage();
    ~MergeTerminalsWorkingPage();

};

#endif
