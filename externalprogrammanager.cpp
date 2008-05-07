//
// C++ Implementation: externalprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "externalprogrammanager.h"
#include "settings.h"

/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ExternalProgramManager::ExternalProgramManager(QWidget* parent): SystemWidget(tr("Externe Programme"), QIcon(":/images/icons/applications-other.svg"), tr("Externe Programme verwalten"), parent)
{
	ui.setupUi(this);
	guessChildTriggers(this);
	hide();

	connect(ui.leHDMan, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leHLEd, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leHCopy, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leHCompV, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leHERest, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leHVite, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leBzip2, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leHHEd, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leMkfa, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));
	connect(ui.leDfaMinimize, SIGNAL(urlChanged(QString)), this, SIGNAL(changed()));

	help = tr("Setzen Sie Pfade zu den externen Programmen, die simon erweitern.");
}

/**
 * \brief Returns true if all fields are filled out
 * \author Peter Grasch
 * @return True, if all mandatory fields are filled out
 */
bool ExternalProgramManager::isComplete()
{
	return (!(ui.leHDMan->text().isEmpty()) && 
		!(ui.leHLEd->text().isEmpty()) && 
		!(ui.leHCopy->text().isEmpty()) && 
		!(ui.leHCompV->text().isEmpty()) && 
		!(ui.leHERest->text().isEmpty()) && 
		!(ui.leHVite->text().isEmpty()) && 
		!(ui.leBzip2->text().isEmpty()) && 
		!(ui.leHHEd->text().isEmpty()) &&  
		!(ui.leMkfa->text().isEmpty()) &&  
		!(ui.leDfaMinimize->text().isEmpty()));
}


/**
 * \brief Applys the changed settings
 * \author Peter Grasch
 * @return Success
 */
bool ExternalProgramManager::apply()
{
	Settings::set("Programs/HTK/HDMan", ui.leHDMan->text());
	Settings::set("Programs/HTK/HLEd", ui.leHLEd->text());
	Settings::set("Programs/HTK/HCopy", ui.leHCopy->text());
	Settings::set("Programs/HTK/HCompV", ui.leHCompV->text());
	Settings::set("Programs/HTK/HERest", ui.leHERest->text());
	Settings::set("Programs/HTK/HHEd", ui.leHHEd->text());
	Settings::set("Programs/HTK/HVite", ui.leHVite->text());
	
	Settings::set("Programs/Files/BZip2", ui.leBzip2->text());
	
	Settings::set("Programs/Julius/mkfa", ui.leMkfa->text());
	Settings::set("Programs/Julius/dfa_minimize", ui.leDfaMinimize->text());
	
	return true;
}

/**
 * \brief Resets the changed settings
 * \author Peter Grasch
 * @return Success
 */
bool ExternalProgramManager::reset()
{
	return init();
}

/**
 * \brief Initialy loads the settings
 * \author Peter Grasch
 * @return Success
 */
bool ExternalProgramManager::init()
{
	ui.leHDMan->setText(Settings::getS("Programs/HTK/HDMan"));
	ui.leHLEd->setText(Settings::getS("Programs/HTK/HLEd"));
	ui.leHCopy->setText(Settings::getS("Programs/HTK/HCopy"));
	ui.leHCompV->setText(Settings::getS("Programs/HTK/HCompV"));
	ui.leHERest->setText(Settings::getS("Programs/HTK/HERest"));
	ui.leHHEd->setText(Settings::getS("Programs/HTK/HHEd"));
	ui.leHVite->setText(Settings::getS("Programs/HTK/HVite"));
	
	ui.leBzip2->setText(Settings::getS("Programs/Files/BZip2"));

	ui.leMkfa->setText(Settings::getS("Programs/Julius/mkfa"));
	ui.leDfaMinimize->setText(Settings::getS("Programs/Julius/dfa_minimize"));
	
	return true;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ExternalProgramManager::~ExternalProgramManager()
{
}


