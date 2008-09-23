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
#include <QRadioButton>
#include <KStandardDirs>
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

	setWindowTitle(i18n("Ort hinzufügen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/importplace.png")));

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
	intro->setTitle(i18n("Hinzufügen eines Ortes"));

	QLabel *label = new QLabel(intro);
	label->setWordWrap(true);
	label->setText(i18n("Hier können Sie einen Ort den Kommandos hinzufügen.\n\nsimon unterscheidet grundsätzlich zwischen lokalen (Ordner) und entfernten Orten (HTTP / FTP).\n\n"));
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
	finished->setTitle(i18n("Hinzufügen eines Ortes"));
	QLabel *label = new QLabel(finished);
	label->setWordWrap(true);
	label->setText(i18n("\n\nKlicken Sie auf \"Fertigstellen\" um den Wizard abzuschließen."));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

