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
#include <KIcon>
#include "../SimonLib/Settings/settings.h"

/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ExternalProgramManager::ExternalProgramManager(QWidget* parent): SystemWidget(i18n("Externe Programme"), KIcon("applications-other"), i18n("Externe Programme verwalten"), parent)
{
	ui.setupUi(this);
	guessChildTriggers(this);
	hide();

	connect(ui.urHDMan, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHLEd, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHCopy, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHCompV, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHERest, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHVite, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urBzip2, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHHEd, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urMkfa, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urDfaMinimize, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	help = i18n("Setzen Sie Pfade zu den externen Programmen, die simon erweitern.");
}

/**
 * \brief Returns true if all fields are filled out
 * \author Peter Grasch
 * @return True, if all mandatory fields are filled out
 */
bool ExternalProgramManager::isComplete()
{
	return (!(ui.urHDMan->url().isEmpty()) && 
		!(ui.urHLEd->url().isEmpty()) && 
		!(ui.urHCopy->url().isEmpty()) && 
		!(ui.urHCompV->url().isEmpty()) && 
		!(ui.urHERest->url().isEmpty()) && 
		!(ui.urHVite->url().isEmpty()) && 
		!(ui.urBzip2->url().isEmpty()) && 
		!(ui.urHHEd->url().isEmpty()) &&  
		!(ui.urMkfa->url().isEmpty()) &&  
		!(ui.urDfaMinimize->url().isEmpty()));
}


/**
 * \brief Applys the changed settings
 * \author Peter Grasch
 * @return Success
 */
bool ExternalProgramManager::apply()
{
	Settings::set("Programs/HTK/HDMan", ui.urHDMan->url().path());
	Settings::set("Programs/HTK/HLEd", ui.urHLEd->url().path());
	Settings::set("Programs/HTK/HCopy", ui.urHCopy->url().path());
	Settings::set("Programs/HTK/HCompV", ui.urHCompV->url().path());
	Settings::set("Programs/HTK/HERest", ui.urHERest->url().path());
	Settings::set("Programs/HTK/HHEd", ui.urHHEd->url().path());
	Settings::set("Programs/HTK/HVite", ui.urHVite->url().path());
	
	Settings::set("Programs/Files/BZip2", ui.urBzip2->url().path());
	
	Settings::set("Programs/Julius/mkfa", ui.urMkfa->url().path());
	Settings::set("Programs/Julius/dfa_minimize", ui.urDfaMinimize->url().path());
	
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
	ui.urHDMan->setPath(Settings::getS("Programs/HTK/HDMan"));
	ui.urHLEd->setPath(Settings::getS("Programs/HTK/HLEd"));
	ui.urHCopy->setPath(Settings::getS("Programs/HTK/HCopy"));
	ui.urHCompV->setPath(Settings::getS("Programs/HTK/HCompV"));
	ui.urHERest->setPath(Settings::getS("Programs/HTK/HERest"));
	ui.urHHEd->setPath(Settings::getS("Programs/HTK/HHEd"));
	ui.urHVite->setPath(Settings::getS("Programs/HTK/HVite"));
	
	ui.urBzip2->setPath(Settings::getS("Programs/Files/BZip2"));

	ui.urMkfa->setPath(Settings::getS("Programs/Julius/mkfa"));
	ui.urDfaMinimize->setPath(Settings::getS("Programs/Julius/dfa_minimize"));
	
	return true;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ExternalProgramManager::~ExternalProgramManager()
{
}


