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
	ui.leHDMan->setText(Settings::get("Programs/HTK/HDMan").toString());
	ui.leHLEd->setText(Settings::get("Programs/HTK/HLEd").toString());
	ui.leHCopy->setText(Settings::get("Programs/HTK/HCopy").toString());
	ui.leHCompV->setText(Settings::get("Programs/HTK/HCompV").toString());
	ui.leHERest->setText(Settings::get("Programs/HTK/HERest").toString());
	ui.leHHEd->setText(Settings::get("Programs/HTK/HHEd").toString());
	ui.leHVite->setText(Settings::get("Programs/HTK/HVite").toString());
	
	ui.leBzip2->setText(Settings::get("Programs/Files/BZip2").toString());
	
	return true;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ExternalProgramManager::~ExternalProgramManager()
{
}


