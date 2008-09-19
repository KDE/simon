//
// C++ Implementation: importtrainingdirectoryintropage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingdirectoryintropage.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectoryIntroPage::ImportTrainingDirectoryIntroPage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	
	ui.urTrainingDataDirectory->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
	
	setTitle(i18n("Trainingsdaten aus Ordner importieren"));
	
	registerField("directory*", ui.urTrainingDataDirectory, "url", SIGNAL(textChanged(QString)));
}
