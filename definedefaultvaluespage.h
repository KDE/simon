//
// C++ Interface: definedefaultvaluespage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEFINEDEFAULTVALUESPAGE_H
#define DEFINEDEFAULTVALUESPAGE_H

#include <QWizardPage>
#include <QVariant>

#include "ui_definiedefaultvaluesdlg.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class DefineDefaultValuesPage : public QWizardPage
{
Q_OBJECT
signals:
	void done();
private:
	bool complete;
	Ui::DefineDefaultValuesDlg ui;
public:
    DefineDefaultValuesPage(QWidget* parent);

    ~DefineDefaultValuesPage();

	bool checkAndSet(QString option, QVariant value);
    bool isComplete() const;
    void initializePage();

};

#endif
