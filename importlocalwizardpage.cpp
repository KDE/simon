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



/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent 
 * Parent of the wizardpage
 */
ImportLocalWizardPage::ImportLocalWizardPage(QWidget *parent) : QWizardPage(parent)
{}



/**
 * \brief Wrapper for the registerField command
 * @param name
 * Name of the field
 * @param widget
 * Widget to apply to
 * @param property
 * The property
 * @param changedSignal 
 * The signal we look for
 */
void ImportLocalWizardPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}


/**
 * \brief Sets the local source file
 * \author Peter Grasch
 * Shows a dialog that prompts the user to open a Textfile 
 * (Filter: *.txt)
 */
void ImportLocalWizardPage::setLocalSourceFile()
{

	QStringList files = QFileDialog::getOpenFileNames(this, "Zu importierende Textdateien öffnen", QDir::currentPath(), "Textdateien (*.txt)");
	if (files.count() == 0) return;

	lePath->setText(files.at(0));
}
