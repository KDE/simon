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
 \class DefineDefaultValuesPage
 \brief Defines default values for crucial settings
 \version 0.1
 \date 13.1.2008
 \author Peter Grasch
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

	bool checkAndSet(QString option, QVariant value);
    bool isComplete() const;
    void initializePage();

};

#endif
