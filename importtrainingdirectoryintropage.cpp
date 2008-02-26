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

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectoryIntroPage::ImportTrainingDirectoryIntroPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(tr("Trainingsdaten aus Ordner importieren"));
	QVBoxLayout *lay = new QVBoxLayout(this);
	
	QLabel *lbIntroText = new QLabel(this);
	lbIntroText->setText(tr("Willkommen beim Importieren von Trainingsdaten aus\neinem Ordner.\n\nAuf diese Weise koennen Sie vorhandene Trainingsdaten\nsimpel und leicht in simon uebernehmen.\n\nBitte waehlen Sie den Ordner aus, der die Trainingsdaten\n(im Wave-Format, 16bit,16khz, mono) enthaelt:\n\n"));
	lay->addWidget(lbIntroText);
	
	
	QHBoxLayout *fileSelectorLay = new QHBoxLayout();
	this->leDirectory = new QLineEdit(this);
	QPushButton *selectDir = new QPushButton(this);
	selectDir->setIcon(QIcon(":/images/icons/document-open.svg"));
	connect(selectDir, SIGNAL(clicked()), this, SLOT(setDir()));
	fileSelectorLay->addWidget(leDirectory);
	fileSelectorLay->addWidget(selectDir);
	lay->addLayout(fileSelectorLay);

	setLayout(lay);
	
	registerField("directory*", leDirectory);
}



/**
 * \brief Prompts the user to select a directory
 * \author Peter Grasch
 */
void ImportTrainingDirectoryIntroPage::setDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Trainingsdaten-Ordner"));
	leDirectory->setText(dir);
}

ImportTrainingDirectoryIntroPage::~ImportTrainingDirectoryIntroPage()
{
    leDirectory->deleteLater();
}
