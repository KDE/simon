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
#include "selectprogrampage.h"
#include "../executablecommand.h"

/**
*   \brief constructor which creats the wizardpages
*   @author Grasch Peter
*   @param QWidget *parent
*/
ImportProgramWizard::ImportProgramWizard ( QWidget* parent ) : QWizard ( parent )
{
	this->addPage ( createIntroPage() );

	selector = createSelectProgramPage();
	this->addPage(selector);

	this->addPage ( createFinishedPage() );

	setWindowTitle ( "Programm hinzufügen" );
	setPixmap ( QWizard::WatermarkPixmap, QPixmap ( ":/images/banners/importprogram.png" ) );

	connect ( this, SIGNAL ( finished ( int ) ), this, SLOT ( import( int ) ) );
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
ImportProgramWizard::~ImportProgramWizard()
{}

/**
*   \brief Creates the intro page
*   @author Susanne Tschernegg
*   @return QWizardPage*
*       returns the introPage of this wizard
*/
QWizardPage* ImportProgramWizard::createIntroPage()
{
	QWizardPage *intro = new QWizardPage ( this );
	intro->setTitle ( i18n("Importieren eines Programmes") );
	QLabel *label = new QLabel ( intro );
	label->setWordWrap ( true );
	label->setText ( i18n("Dieser Assistent wird ihnen helfen, ein Program, welches bereits installiert wurde, in simon zu importieren.\n\nsimon wird dabei versuchen automatisch alle installierten Proramme zu erkennen und eine sortiert Liste präsentieren, aus der Sie das gewünschte Programm auswählen können.") );
	QVBoxLayout *layout = new QVBoxLayout ( intro );
	layout->addWidget ( label );
	intro->setLayout ( layout );

	return intro;
}

/**
*   \brief creates the selectprogrampage
*   @author Susanne Tschernegg
*   @return SelectProgramPage*
*       returns a new initialized SelectProgramPage
*/
SelectProgramPage* ImportProgramWizard::createSelectProgramPage()
{
	return new SelectProgramPage ( this );
}


/**
*   \brief creates the last page
*   @author Susanne Tschernegg
*   @return QWizardPage*
*       returns a new initialized finish-WizardPage
*/
QWizardPage* ImportProgramWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage ( this );
	finished->setTitle ( tr ( "Hinzufügen des Programmes" ) );
	QLabel *label = new QLabel ( finished );
	label->setText ( tr ( "Klicken Sie auf \"Fertigstellen\" um den Wizard abzuschließen." ) );
	label->setWordWrap ( true );
	QVBoxLayout *layout = new QVBoxLayout ( finished );
	layout->addWidget ( label );
	finished->setLayout ( layout );

	return finished;
}

void ImportProgramWizard::import(int status)
{
	if (selector && status)
	{
		ExecutableCommand *com = new ExecutableCommand(selector->getName(), selector->getIcon(),
						selector->getExecPath(), selector->getWorkingDirectory());
		emit commandCreated(com);
	}

	restart();
}

