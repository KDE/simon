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

ImportTrainingDirectory::ImportTrainingDirectory(QWidget *parent) : QWizard(parent)
{
	setWindowTitle("Importiere Trainingsdaten von Ordner");
 	addPage(createIntroPage());
	addPage(createWorkingPage());
	addPage(createFinishedPage());
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(idChanged(int)));
	prevId=0;
}

void ImportTrainingDirectory::idChanged(int id)
{
	if (id == 1)
	{
		if (prevId==2)
			restart();
		else ((ImportTrainingDirectoryWorkingPage*) 
			page(1))->importDir(
				((ImportTrainingDirectoryIntroPage*) 
				page(0))->getField("directory").toString());
	}
	
	prevId = id;
}


ImportTrainingDirectory::~ImportTrainingDirectory()
{
}


ImportTrainingDirectoryIntroPage* ImportTrainingDirectory::createIntroPage()
{
	ImportTrainingDirectoryIntroPage *introPage =  new 
		ImportTrainingDirectoryIntroPage(this);
	return introPage;
}

ImportTrainingDirectoryWorkingPage* ImportTrainingDirectory::createWorkingPage()
{
	return new ImportTrainingDirectoryWorkingPage(this);
}

QWizardPage* ImportTrainingDirectory::createFinishedPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle("Importieren des Ordners abgeschlossen");
	QLabel *label = new QLabel(intro);
	label->setText("Die Dateien aus dem Ordner wurden nun imporitert.\n\nVielen dank, dass Sie sich die Zeit genommen haben um simon\nzu verbessern.");
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}
