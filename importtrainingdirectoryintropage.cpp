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
#include <QPushButton>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

ImportTrainingDirectoryIntroPage::ImportTrainingDirectoryIntroPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle("Trainingsdaten aus Ordner importieren");
	QVBoxLayout *lay = new QVBoxLayout(this);
	
	QLabel *lbIntroText = new QLabel(this);
	lbIntroText->setText(tr("Willkommen beim Importieren von Trainingsdaten aus\neinem Ordner.\n\nAuf diese Weise koennen Sie vorhandene Trainingsdaten\nsimpel und leicht in simon uebernehmen.\n\nBitte waehlen Sie den Ordner aus, der die Trainingsdaten\n(im Wave-Format, 16bit,16khz, mono) enthaelt:\n\n"));
	lay->addWidget(lbIntroText);
	
	
	QHBoxLayout *fileSelectorLay = new QHBoxLayout(this);
	this->leDirectory = new QLineEdit(this);
// 	leDirectory->setText(QDir::currentPath());
	QPushButton *selectDir = new QPushButton(this);
	selectDir->setIcon(QIcon(":/images/icons/document-open.svg"));
	connect(selectDir, SIGNAL(clicked()), this, SLOT(setDir()));
	fileSelectorLay->addWidget(leDirectory);
	fileSelectorLay->addWidget(selectDir);
	lay->addLayout(fileSelectorLay);

	setLayout(lay);
	
	registerField("directory*", leDirectory);
}


ImportTrainingDirectoryIntroPage::~ImportTrainingDirectoryIntroPage()
{
}

void ImportTrainingDirectoryIntroPage::registerField(const QString &name, QWidget *widget, const char* 
		property, const char* changedSignal)
{
	QWizardPage::registerField(name, widget, property, changedSignal);
}


void ImportTrainingDirectoryIntroPage::setDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, "Trainingsdaten-Ordner");
	leDirectory->setText(dir);
}
