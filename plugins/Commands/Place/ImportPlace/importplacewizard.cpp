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

	setWindowTitle(i18n("Add Place"));
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
	intro->setTitle(i18n("Add a new Place"));

	QLabel *label = new QLabel(intro);
	label->setWordWrap(true);
	label->setText(i18n("With this wizard you can assoziate places with commands.\n\nBoth local and remote places are supported."));
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
	finished->setTitle(i18n("Place added"));
	QLabel *label = new QLabel(finished);
	label->setWordWrap(true);
	label->setText(i18n("Press Finish to complete this wizard."));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

