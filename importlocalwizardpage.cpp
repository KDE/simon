//
// C++ Implementation: importlocalwizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importlocalwizardpage.h"



ImportLocalWizardPage::ImportLocalWizardPage(QWidget *parent) : QWizardPage(parent)
{}



void ImportLocalWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}


void ImportLocalWizardPage::setLocalSourceFile()
{

	QStringList files = QFileDialog::getOpenFileNames(this, "Zu importierende Textdateien öffnen", QDir::currentPath(), "Textdateien (*.txt)");
	if (files.count() == 0) return;

	lePath->setText(files.at(0));
}
