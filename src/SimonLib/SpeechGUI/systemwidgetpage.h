//
// C++ Interface: systemwidgetpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SYSTEMWIDGETPAGE_H
#define SYSTEMWIDGETPAGE_H

#include <QWizardPage>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class SystemWidget;
class SystemWidgetPage : public QWizardPage
{
Q_OBJECT
private:
	SystemWidget *child;
public:
    SystemWidgetPage(QWidget *parent);
	void setChild(SystemWidget *child);
	void initializePage();

	bool isComplete() const;
	bool validatePage();
    ~SystemWidgetPage();

};

#endif
