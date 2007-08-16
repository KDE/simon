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

/**
*   @autor Susanne Tschernegg
*/
ImportProgramWizard::ImportProgramWizard(QWidget* parent, Qt::WindowFlags flags): QWizard(parent, flags)
{
    this->addPage(createIntroPage());
	this->addPage(createSelectProgramPage());
	this->addPage(createConfigureProgramPage());
    this->addPage(createImportProgramPage());
    this->addPage(createFinishedPage());

	setWindowTitle("Programm hinzufügen");
}


ImportProgramWizard::~ImportProgramWizard()
{}

/**
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
*   @autor Susanne Tschernegg
*/
SelectProgramPage* ImportProgramWizard::createSelectProgramPage()
{
    return new SelectProgramPage(this);
}

/**
*   @autor Susanne Tschernegg
*/
ConfigureProgramPage* ImportProgramWizard::createConfigureProgramPage()
{
    return new ConfigureProgramPage(this);
}

/**
*   @autor Susanne Tschernegg
*/
ImportProgramPage* ImportProgramWizard::createImportProgramPage()
{
    return new ImportProgramPage(this);
}

/**
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


