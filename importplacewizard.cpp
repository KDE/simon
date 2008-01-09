//
// C++ Implementation: importplacewizard
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importplacewizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

/**
*   \brief constructor which creats the wizardpages
*   @autor Susanne Tschernegg
*/
ImportPlaceWizard::ImportPlaceWizard(QWidget* parent): QWizard(parent)
{
    oldId=0;
    introPlacePage = createIntroPlacePage();
    this->addPage(introPlacePage);
    localPlacePage = createLocalPlacePage();
	this->addPage(localPlacePage);
    remotePlacePage = createRemotePlacePage();
    this->addPage(remotePlacePage);
    configurePlacePage = createConfigurePlacePage();
    this->addPage(configurePlacePage);
    importPlacePage = createImportPlacePage();
    this->addPage(importPlacePage);
    this->addPage(createFinishedPage());
    
    //(connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
    
	setWindowTitle(tr("Ort hinzufügen"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(tr(":/images/banners/importprogram.png")));
    
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(idChanged(int)));
    connect(this, SIGNAL(finished(int)), this, SLOT(restart()));
    
    connect(importPlacePage, SIGNAL(commandCreated(Command*)), this, SIGNAL(commandCreated(Command*)));
   // connect(importPlacePage, SIGNAL(commandCreated(Command*)), this, SLOT(next()));
    connect(introPlacePage, SIGNAL(placeChanged()), this, SLOT(placePreselectionChanged()));
}

/**
*   \brief destructor
*   @autor Susanne Tschernegg
*/
ImportPlaceWizard::~ImportPlaceWizard()
{}

/**
*   \brief Creates the intro page
*   @autor Susanne Tschernegg
*/
IntroPlacePage* ImportPlaceWizard::createIntroPlacePage()
{
    /*QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Hinzufügen eines Ortes"));
	QLabel *label = new QLabel(intro);
	label->setText(tr("Hier können Sie einen Ort ihren Kommandos hinzufügen, um ihn später über das Schlüsselwort Simon anzusprechen. \n\n" + 
                            "Es wird zwischen lokalen und entfernten Orten unterschieden. Unter entfernten Orten versteht man Orte, die über das Internet erreichbar sind."));
	label->setWordWrap(true);
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);*/
	
	return new IntroPlacePage(this);
}

/**
*   \brief creates the localplacepage
*   @autor Susanne Tschernegg
*/
LocalPlacePage* ImportPlaceWizard::createLocalPlacePage()
{
    return new LocalPlacePage(this);
}

/**
*   \brief creates the remoteplacepage
*   @autor Susanne Tschernegg
*/
RemotePlacePage* ImportPlaceWizard::createRemotePlacePage()
{
    return new RemotePlacePage(this);
}

/**
*   \brief creates the configurePlacePage
*   @autor Susanne Tschernegg
*/
ConfigurePlacePage* ImportPlaceWizard::createConfigurePlacePage()
{
    return new ConfigurePlacePage(this);
}

/**
*   \brief creates the importplacepage
*   @autor Susanne Tschernegg
*/
ImportPlacePage* ImportPlaceWizard::createImportPlacePage()
{
    return new ImportPlacePage(this);
}

/**
*   \brief creates the last page
*   @autor Susanne Tschernegg
*/
QWizardPage* ImportPlaceWizard::createFinishedPage()
{
    QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(tr("Hinzufügen eines Ortes"));
	QLabel *label = new QLabel(finished);
	label->setText(tr("\n\nKlicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschließen."));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

/**
*   \brief slot: the signal is emited this class
            if the current page changes, we will save the last pageId and the new pageId
*   @autor Susanne Tschernegg
*/
void ImportPlaceWizard::idChanged(int newId)
{
    //QMessageBox::information(this, "importprogwizard .. idchanged ANF... oldID", QString::number(oldId));
    //QMessageBox::information(this, "importprogwizard .. idchanged ANF... newID", QString::number(newId));
    if((oldId==0) && (newId==1))
    {
        if(introPlacePage->rbRemotePlace->isChecked())
        {
            oldId = newId + 1;
            next();
        }
        else
        {
            oldId = newId;
        }
    }
    else if((oldId==1) && (newId==2))
    {
        if(introPlacePage->rbLocalPlace->isChecked())
        {
            oldId = newId + 1;
            next();
        }
        else
        {
             oldId = newId;
        }
    }
    else if((oldId==2) && (newId==1))
    {
        oldId = newId - 1;
        back();
    }
    else if((oldId==3) && (newId==2))
    {
        if(introPlacePage->rbLocalPlace->isChecked())
        {
            oldId = newId - 1;
            back();
        }
        else
        {
             oldId = newId;
        }
    }
    else if((oldId==3) && (newId==4))
    {
        configurePlacePage->setPlaceName();
        configurePlacePage->setPlaceValue();
        importPlacePage->createCommand(configurePlacePage->getPlaceName(), configurePlacePage->getPlaceValue());
    }
    else
    {    
        oldId = newId;
    }
    if(newId==3)
    {
        if(introPlacePage->rbLocalPlace->isChecked())
        {
            configurePlacePage->setPlaceValue(localPlacePage->getPlacePath());
        }
        else if(introPlacePage->rbRemotePlace->isChecked())
        {
            configurePlacePage->setPlaceValue(remotePlacePage->getPlacePath());
        }
        configurePlacePage->writeInformation();
    }
}

/**
*   \brief slot: for checking, which class of places where choosen ... we don't use this method yet
*   @author Susanne Tschernegg
*/
void ImportPlaceWizard::placePreselectionChanged()
{
    //QMessageBox::information(this,"importplacewizard","placepreselectionchanged");
    IntroPlacePage *ipp = dynamic_cast<IntroPlacePage*>(page(0));
    if(!ipp)
        return;
 /*   if(introPlacePage->rbLocalPlace->isChecked())
    {
        //remotePlacePage->hide();
        remotePlacePage->close();
       // localPlacePage->setVisible();
        
        //back();
        //this->setPage(1, createLocalPlacePage());
        //next();
    }
    else if(introPlacePage->rbRemotePlace->isChecked())
    {
        //localPlacePage->hide();
        localPlacePage->close();
       // remotePlacePage->setVisible();

        //back();
        //this->setPage(1, createRemotePlacePage());
        //next();
    }*/
}

/**
*   \brief slot: the signal is emited in the commandSettings.cpp (changeExistingName)
            if the name of the commando already exists in the commandlist, a message box (called in commendSettings) will be opend and asks,
            if the user wants to change the name of the commando
*   @autor Susanne Tschernegg
*/
void ImportPlaceWizard::changeName(bool change)
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
