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
    this->addPage(createIntroPage());
	this->addPage(createChoosePlacePage());
	this->addPage(createLocalPlacePage());
    this->addPage(createRemotePlacePage());
    this->addPage(createImportPlacePage());
    this->addPage(createFinishedPage());
    
    //(connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
    
	setWindowTitle(tr("Ort hinzufügen"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(tr(":/images/importprogram.png")));
    
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(idChanged(int)));
    //connect(ipp, SIGNAL(commandCreated(Command*)), this, SIGNAL(commandCreated(Command*)));
   // connect(ipp, SIGNAL(commandCreated(Command*)), this, SLOT(next()));
    connect(this, SIGNAL(finished(int)), this, SLOT(restart()));
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
QWizardPage* ImportPlaceWizard::createIntroPage()
{
    QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Hinzufügen des Ortes"));
	QLabel *label = new QLabel(intro);
	label->setText(tr("Hier kann ein Ort den Kommandos - die Simon kennt - hinzugefügt werden."));
	label->setWordWrap(true);
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}

/**
*   \brief creates the cooseplacepage
*   @autor Susanne Tschernegg
*/
ChoosePlacePage* ImportPlaceWizard::createChoosePlacePage()
{
    return new ChoosePlacePage(this);
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
	finished->setTitle(tr("Hinzufügen des Ortes"));
	QLabel *label = new QLabel(finished);
	label->setText(tr("Klicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschließen."));
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
//    QMessageBox::information(this, "importprogwizard .. idchanged ANF... newID", QString::number(newId));
//    QMessageBox::information(this, "importprogwizard .. idchanged ANF... oldID", QString::number(oldId));
   // if((oldId==1) && (newId==2))
    
    oldId = newId;
}
