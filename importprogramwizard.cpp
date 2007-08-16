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

ImportProgramWizard::ImportProgramWizard(QWidget* parent, Qt::WindowFlags flags): QWizard(parent, flags)
{
    this->addPage(createIntroPage());
	this->addPage(createSelectProgramPage());
	this->addPage(createConfigureProgramPage());
    this->addPage(createImportProgramPage());
    this->addPage(createFinishedPage());

	setWindowTitle(tr("Programm hinzufügen"));
}


ImportProgramWizard::~ImportProgramWizard()
{}
    
QWizardPage* createIntroPage()
{
    QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Hinzufügen des Programmes"));
	QLabel *label = new QLabel(intro);
	label->setText(tr("Hier kann ein Programm den Kommandos - die Simon kennt - hinzugefügt werden."));
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}
        
SelectProgramPage* createSelectProgramPage()
{
    return new SelectProgramPage(this);
}
    
ConfigureProgramPage* createConfigureProgramPage()
{
    return new ConfigureProgramPage(this);
}
    
ImportProgramPage* createImportProgramPage()
{
    return new ImportProgramPage(this);
}
    
QWizard* ImportProgramWizard::createFinishedPage()
{
    QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(tr("Hinzufügen des Programmes"));
	QLabel *label = new QLabel(finished);
	label->setText(tr("Klicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschließen."));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}


