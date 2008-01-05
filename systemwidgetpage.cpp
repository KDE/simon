//
// C++ Implementation: systemwidgetpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "systemwidgetpage.h"
#include "systemwidget.h"
#include "externalprogrammanager.h"
#include <QVBoxLayout>

class SystemWidget;

SystemWidgetPage::SystemWidgetPage(QWidget *parent)
 : QWizardPage(parent)
{
}

void SystemWidgetPage::setChild(SystemWidget *child)
{
	connect (child, SIGNAL(changed()), this, SIGNAL(completeChanged()));
	
	child->init();
	this->child = child;
	setTitle(child->getTitle());

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(child);
	this->setLayout(lay);
	child->show();
}

/**
 * \brief Applys the systemwidgets values
 * 
 * This method is automatically called when pressing the next button
 * \author Peter Grasch
 * @return If we are "allowed" to continue
 */
bool SystemWidgetPage::validatePage()
{
	child->apply();
	return isComplete();
}

bool SystemWidgetPage::isComplete() const
{
	return child->isComplete();
}


SystemWidgetPage::~SystemWidgetPage()
{
}


