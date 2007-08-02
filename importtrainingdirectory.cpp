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
#include <QLineEdit>
#include <QDir>
#include <QPushButton>
#include <QIcon>
#include <QCoreApplication>

/**
 * \brief Constructor - Creats the pages and adds them
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectory::ImportTrainingDirectory(QWidget *parent) : QWizard(parent)
{
	setWindowTitle("Importiere Trainingsdaten von Ordner");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/importfolder.png"));

 	addPage(createIntroPage());
	addPage(createWorkingPage());
	addPage(createFinishedPage());
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(idChanged(int)));
	prevId=0;
}

/**
 * \brief Slot to react on changing the page
 * 
 * Restarts the wizard if we want to go back from finished->working;
 * Starts the importing if we go from intro->working
 * 
 * @param id newId
 */
void ImportTrainingDirectory::idChanged(int id)
{
	if (id == 1)
	{
		if (prevId==2)
			restart();
		else 
		{
			QCoreApplication::processEvents();
			((ImportTrainingDirectoryWorkingPage*)
				page(1))->importDir(
					((ImportTrainingDirectoryIntroPage*) 
					page(0))->getField("directory").toString());
		}
	}
	
	prevId = id;
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
	return new ImportTrainingDirectoryWorkingPage(this);
}

/**
 * \brief Creates the Finished page
 * @return the page
 */
QWizardPage* ImportTrainingDirectory::createFinishedPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle("Importieren des Ordners abgeschlossen");
	QLabel *label = new QLabel(intro);
	label->setText("Die Dateien aus dem Ordner wurden nun imporitert.\n\nVielen dank, dass Sie sich die Zeit genommen haben\num simon zu verbessern.");
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}
