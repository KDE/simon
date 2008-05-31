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
#include <QDebug>
#include <QVBoxLayout>
#include <QPixmap>
#include <QRadioButton>
#include "../placecommand.h"
#include "selectplacepage.h"

/**
*   \brief constructor which creats the wizardpages
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
ImportPlaceWizard::ImportPlaceWizard(QWidget* parent): QWizard(parent)
{
	this->addPage(createIntroPlacePage());
	selectPlacePage = createSelectPlacePage();
	addPage(selectPlacePage);
	this->addPage(createFinishedPage());

	setWindowTitle(tr("Ort hinzufügen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(tr(":/images/banners/importplace.png")));

	connect(this, SIGNAL(finished(int)), this, SLOT(createCommand(int)));
}

void ImportPlaceWizard::createCommand(int status)
{
	if (status)
		emit commandCreated(new PlaceCommand(selectPlacePage->getName(), "", selectPlacePage->getUrl()));
	
	restart();
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
ImportPlaceWizard::~ImportPlaceWizard()
{}

/**
*   \brief Creates the select-page
*   @author Peter Grasch
*   @return The page
*/
SelectPlacePage* ImportPlaceWizard::createSelectPlacePage()
{
	return new SelectPlacePage(this);
}

/**
*   \brief Creates the intro page
*   @author Susanne Tschernegg
*   @return IntroPlacePage*
*       returns the new initialized IntroPlacePage
*/
QWizardPage* ImportPlaceWizard::createIntroPlacePage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Hinzufügen eines Ortes"));

	QLabel *label = new QLabel(intro);
	label->setWordWrap(true);
	label->setText(tr("Hier können Sie einen Ort ihren Kommandos hinzufügen, um ihn später über das Schlüsselwort Simon anzusprechen. \n\nEs wird zwischen lokalen und entfernten Orten unterschieden. Unter entfernten Orten versteht man Orte, die über das Internet erreichbar sind.\n\n"));
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);

	return intro;
}



/**
*   \brief creates the last page
*   @author Susanne Tschernegg
*   @return QWizardPage*
*       returns a new WizardPage 
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
*   @author Susanne Tschernegg
*   @param int newId
*       holds the Id of the current page
*/
// void ImportPlaceWizard::idChanged(int newId)
// {
//     if((oldId==0) && (newId==1))
//     {
//         if(introPlacePage->rbRemotePlace->isChecked())
//         {
//             oldId = newId + 1;
//             next();
//         }
//         else
//         {
//             oldId = newId;
//         }
//     }
//     else if((oldId==1) && (newId==2))
//     {
//         if(introPlacePage->rbLocalPlace->isChecked())
//         {
//             oldId = newId + 1;
//             next();
//         }
//         else
//         {
//              oldId = newId;
//         }
//     }
//     else if((oldId==2) && (newId==1))
//     {
//         oldId = newId - 1;
//         back();
//     }
//     else if((oldId==3) && (newId==2))
//     {
//         if(introPlacePage->rbLocalPlace->isChecked())
//         {
//             oldId = newId - 1;
//             back();
//         }
//         else
//         {
//              oldId = newId;
//         }
//     }
//     else if((oldId==3) && (newId==4))
//     {
//         configurePlacePage->setPlaceName();
//         configurePlacePage->setPlaceValue();
//         importPlacePage->createCommand(configurePlacePage->getPlaceName(), configurePlacePage->getPlaceValue());
//     }
//     else
//     {    
//         oldId = newId;
//     }
//     if(newId==3)
//     {
//         if(introPlacePage->rbLocalPlace->isChecked())
//         {
//             configurePlacePage->setPlaceValue(localPlacePage->getPlacePath());
//         }
//         else if(introPlacePage->rbRemotePlace->isChecked())
//         {
//             configurePlacePage->setPlaceValue(remotePlacePage->getPlacePath());
//         }
//         configurePlacePage->writeInformation();
//     }
// }


