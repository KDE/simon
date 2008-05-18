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
#include <QPushButton>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileInfo>
#include <QLabel>
#include <QCoreApplication>
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
	setTitle(tr("Verarbeite Ordner..."));
	completed = false;
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbMain = new QLabel(this);
	lbMain->setText(tr("Der angegebene Ordner wird verarbeit.\nSie können den Fortschritt mit dem Fortschrittsbalken\nüberprüfen.\n\nBitte haben Sie einen Moment Geduld.\n\n"));
	lbStatus = new QLabel(this);
	
	pbMain = new QProgressBar(this);
	pbMain->setMaximum(0);

	lay->addWidget(lbMain);
	lay->addWidget(pbMain);
	lay->addWidget(lbStatus);
	setLayout(lay);

	importer = new ImportTrainingData(this);
	connect(importer, SIGNAL(done()), this, SLOT(setComplete()));
	connect(importer, SIGNAL(progress(int, int)), this, SLOT(displayProgress(int, int)));
	connect(importer, SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
}

void ImportTrainingDirectoryWorkingPage::displayProgress(int now, int max)
{
	if (max != -1)
		this->pbMain->setMaximum(max);
	pbMain->setValue(now);
}

void ImportTrainingDirectoryWorkingPage::displayStatus(QString status)
{
	lbStatus->setText(status);
}

void ImportTrainingDirectoryWorkingPage::displayError(QString error)
{
	QMessageBox::critical(this, tr("Fehler"), error);
}

void ImportTrainingDirectoryWorkingPage::setComplete()
{
	completed = true;
	emit completeChanged();

	if (QMessageBox::question(this, tr("Änderungen anwenden"), tr("Soll das Sprachmodell mit diesen neuen Daten neu kompiliert werden?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
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
	
	importer->import(field("directory").toString());
}

ImportTrainingDirectoryWorkingPage::~ImportTrainingDirectoryWorkingPage()
{
    pbMain->deleteLater();
}
