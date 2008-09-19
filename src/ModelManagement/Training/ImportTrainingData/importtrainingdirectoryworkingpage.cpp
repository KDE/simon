//
// C++ Implementation: importtrainingdirectoryworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingdirectoryworkingpage.h"
#include <KMessageBox>
#include <KUrl>
#include <QFileInfo>
#include <QVariant>
#include "importtrainingdata.h"
#include "../../modelmanager.h"

/**
 * \brief Constructor - creates the GUI
 * \author Peter Grasch
 * @param parent 
 * Parent of the widget
 */
ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	
	setTitle(i18n("Verarbeite Ordner..."));
	completed = false;
	
	importer = new ImportTrainingData(this);
	connect(importer, SIGNAL(done()), this, SLOT(setComplete()));
	connect(importer, SIGNAL(progress(int, int)), this, SLOT(displayProgress(int, int)));
	connect(importer, SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
}

void ImportTrainingDirectoryWorkingPage::displayProgress(int now, int max)
{
	if (max != -1)
		ui.pbMain->setMaximum(max);
	ui.pbMain->setValue(now);
}

void ImportTrainingDirectoryWorkingPage::displayStatus(QString status)
{
	ui.lbStatus->setText(status);
}

void ImportTrainingDirectoryWorkingPage::displayError(QString error)
{
	KMessageBox::error(this, error);
}

void ImportTrainingDirectoryWorkingPage::setComplete()
{
	completed = true;
	emit completeChanged();

	if (KMessageBox::questionYesNoCancel(this, i18n("Soll das Sprachmodell mit diesen neuen Daten neu kompiliert werden?"), i18n("Änderungen anwenden")) == KMessageBox::Yes)
		ModelManager::compileModel();
	emit done();
}


/**
 * \brief Starts the importing process and calls all the other methods
 * \author Peter Grasch
 */
void ImportTrainingDirectoryWorkingPage::initializePage()
{
	completed = false;
	emit completeChanged();
	
	//reading
	importer->import(field("directory").value<KUrl>().path());
}
