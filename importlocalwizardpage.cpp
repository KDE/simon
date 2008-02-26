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

#include <QLineEdit>
#include <QVariant>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDir>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent 
 * Parent of the wizardpage
 */
ImportLocalWizardPage::ImportLocalWizardPage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle(tr("Importieren aus Textdatei"));
	connect(ui.tbSelectPath, SIGNAL(clicked()), this, SLOT(setLocalSourceFile()));
	
	registerField("importTrainingTextLFilename*", ui.lePath);
	registerField("importTrainingTextLTextname*", ui.leName);
}


/**
 * \brief Sets the local source file
 * \author Peter Grasch
 * Shows a dialog that prompts the user to open a Textfile 
 * (Filter: *.txt)
 */
void ImportLocalWizardPage::setLocalSourceFile()
{

	QStringList files = QFileDialog::getOpenFileNames(this, tr("Zu importierende Textdateien öffnen"), QDir::currentPath(), tr("Textdateien (*.txt)"));
	if (files.count() == 0) return;

	setField("importTrainingTextLFilename", files.at(0));
}
