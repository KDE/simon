//
// C++ Implementation: importtrainingdirectory
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingdirectory.h"
#include "importtrainingdirectoryintropage.h"
#include "importtrainingdirectoryworkingpage.h"

#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KLineEdit>
#include <QDir>
#include <KPushButton>
#include <QIcon>
#include <QCoreApplication>

/**
 * \brief Constructor - Creats the pages and adds them
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectory::ImportTrainingDirectory(QWidget *parent) : QWizard(parent)
{
	setWindowTitle(tr("Importiere Trainingsdaten von Ordner"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/importfolder.png"));

 	addPage(createIntroPage());
	addPage(createWorkingPage());
	addPage(createFinishedPage());
	prevId=0;
}



/**
 * \brief Creates a new ImoprtTrainingDirectoryIntroPage
 * @return the wizard page
 */
ImportTrainingDirectoryIntroPage* ImportTrainingDirectory::createIntroPage()
{
	ImportTrainingDirectoryIntroPage *introPage =  new 
		ImportTrainingDirectoryIntroPage(this);
	return introPage;
}

/**
 * \brief creates the working-page
 * @return the newly created ImportTrainingDirectoryWorkingPage
 */
ImportTrainingDirectoryWorkingPage* ImportTrainingDirectory::createWorkingPage()
{
	
	ImportTrainingDirectoryWorkingPage *page = new ImportTrainingDirectoryWorkingPage(this);
	connect(page, SIGNAL(done()), this, SLOT(next()));
	return  page;
}

/**
 * \brief Creates the Finished page
 * @return the page
 */
QWizardPage* ImportTrainingDirectory::createFinishedPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Importieren des Ordners abgeschlossen"));
	QLabel *label = new QLabel(intro);
	label->setText(tr("Die Dateien aus dem Ordner wurden nun importiert.\n\nVielen dank, dass Sie sich die Zeit genommen haben\num simon zu verbessern."));
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}
