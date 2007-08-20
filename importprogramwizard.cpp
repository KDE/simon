//
// C++ Implementation: importprogramwizard
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importprogramwizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

/**
*   \brief constructor which creats the wizardpages
*   @autor Susanne Tschernegg
*/
ImportProgramWizard::ImportProgramWizard(QWidget* parent): QWizard(parent)
{
    this->addPage(createIntroPage());
	this->addPage(createSelectProgramPage());
	this->addPage(createConfigureProgramPage());
    this->addPage(createImportProgramPage());
    this->addPage(createFinishedPage());

    //(connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
    
	setWindowTitle("Programm hinzufügen");
    setPixmap(QWizard::WatermarkPixmap, QPixmap(tr(":/images/importdict.png")));
}

/**
*   \brief destructor
*   @autor Susanne Tschernegg
*/
ImportProgramWizard::~ImportProgramWizard()
{}

/**
*   \brief Creates the intro page
*   @autor Susanne Tschernegg
*/
QWizardPage* ImportProgramWizard::createIntroPage()
{
    QWizardPage *intro = new QWizardPage(this);
	intro->setTitle("Hinzufügen des Programmes");
	QLabel *label = new QLabel(intro);
	label->setText("Hier kann ein Programm den Kommandos - die Simon kennt - hinzugefügt werden.");
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}

/**
*   \brief creates the selectprogrampage
*   @autor Susanne Tschernegg
*/
SelectProgramPage* ImportProgramWizard::createSelectProgramPage()
{
    return new SelectProgramPage(this);
}

/**
*   \brief creates the configureprogrampage
*   @autor Susanne Tschernegg
*/
ConfigureProgramPage* ImportProgramWizard::createConfigureProgramPage()
{
    return new ConfigureProgramPage(this);
}

/**
*   \brief creates the importprogrampage
*   @autor Susanne Tschernegg
*/
ImportProgramPage* ImportProgramWizard::createImportProgramPage()
{
    return new ImportProgramPage(this);
}

/**
*   \brief creates the last page
*   @autor Susanne Tschernegg
*/
QWizardPage* ImportProgramWizard::createFinishedPage()
{
    QWizardPage *finished = new QWizardPage(this);
	finished->setTitle("Hinzufügen des Programmes");
	QLabel *label = new QLabel(finished);
	label->setText("Klicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschließen.");
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

void ImportProgramWizard::test()
{
    QMessageBox::information(this, "test", "ksdjf");    
}

