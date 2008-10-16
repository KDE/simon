/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "importprogramwizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <KStandardDirs>
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

	setWindowTitle ( i18n("Programm hinzufügen") );
	setPixmap ( QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/importprogram.png")) );

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
	finished->setTitle ( i18n ( "Hinzufügen des Programmes" ) );
	QLabel *label = new QLabel ( finished );
	label->setText ( i18n ( "Klicken Sie auf \"Fertigstellen\" um den Wizard abzuschließen." ) );
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

