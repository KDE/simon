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
ImportProgramWizard::ImportProgramWizard(QWidget* parent): SimonWizard(parent)
{
    oldId=0;
    this->addPage(createIntroPage());
	this->addPage(createSelectProgramPage());
	this->addPage(createConfigureProgramPage());
    ImportProgramPage *ipp = createImportProgramPage();
    this->addPage(ipp);
    this->addPage(createFinishedPage());
    
    //(connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
    
	setWindowTitle("Programm hinzufügen");
    setPixmap(QWizard::WatermarkPixmap, QPixmap(tr(":/images/banners/importprogram.png")));
    
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(idChanged(int)));
    connect(ipp, SIGNAL(commandCreated(Command*)), this, SIGNAL(commandCreated(Command*)));
   // connect(ipp, SIGNAL(commandCreated(Command*)), this, SLOT(next()));
    connect(this, SIGNAL(finished(int)), this, SLOT(restart()));
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
	label->setWordWrap(true);
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}

/**
*   \brief creates the selectprogrampage
*   @author Susanne Tschernegg
*/
SelectProgramPage* ImportProgramWizard::createSelectProgramPage()
{
    return new SelectProgramPage(this);
}

/**
*   \brief creates the configureprogrampage
*   @author Susanne Tschernegg
*/
ConfigureProgramPage* ImportProgramWizard::createConfigureProgramPage()
{
    return new ConfigureProgramPage(this);
}

/**
*   \brief creates the importprogrampage
*   @author Susanne Tschernegg
*/
ImportProgramPage* ImportProgramWizard::createImportProgramPage()
{
    return new ImportProgramPage(this);
}

/**
*   \brief creates the last page
*   @author Susanne Tschernegg
*/
QWizardPage* ImportProgramWizard::createFinishedPage()
{
    QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(tr("Hinzufügen des Programmes"));
	QLabel *label = new QLabel(finished);
	label->setText(tr("Klicken Sie auf \"Fertigstellen\" um den Wizard abzuschließen."));
	label->setWordWrap(true);
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

/**
*   \brief slot: the signal is emited this class
            if the current page changes, we will save the last pageId and the new pageId
*   @author Susanne Tschernegg, Peter Grasch
*/
void ImportProgramWizard::idChanged(int newId)
{
    if((oldId==1) && (newId==2))
    {
        SelectProgramPage *spp = dynamic_cast<SelectProgramPage*>(page(1));
        if(!spp)
            return;
        ConfigureProgramPage *cpp = dynamic_cast<ConfigureProgramPage*>(page(2));
        cpp->init(spp->getIcon(), spp->getName(), spp->getExecPath(), spp->getWorkingDirectory());
    }
    else if(((oldId==2) && (newId==3)) || ((oldId==3) && (newId==3)))
    {
        ConfigureProgramPage *cpp = dynamic_cast<ConfigureProgramPage*>(page(2));
        if(!cpp)
            return;
        ImportProgramPage *ipp = dynamic_cast<ImportProgramPage*>(page(3));
        ipp->createCommand(cpp->getIcon(), cpp->getName(), cpp->getExec(), cpp->getWorkingDir());
        if(oldId<=newId)next();
    }
    else if((oldId==4) && (newId<oldId))
    {
        restart();
        next();
    }
    oldId = newId;
}

/**
*   \brief slot: the signal is emited in the commandSettings.cpp (changeExistingName)
            if the name of the commando already exists in the commandlist, a message box (called in commendSettings) will be opend and asks,
            if the user wants to change the name of the commando
*   @autor Susanne Tschernegg
*/
void ImportProgramWizard::changeName(bool change)
{
    if(!change)
    {
        done(-1);
    }
    else
    {
        back();
    }
}
