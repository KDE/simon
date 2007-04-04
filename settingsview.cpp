//
// C++ Implementation: settingsview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settingsview.h"


/**
 * \brief Constructor
 * 
 * This instances a new dialog and connects the signals/slots
 * It also enables/disables the plattform dependant options (alsa/dsound)
 * 
 * \author Peter Grasch
 * 
 * \param QWidget *parent
 * The parent of the dialog
 */
SettingsView::SettingsView(QWidget *parent)
 : QDialog(parent)
{
	ui.setupUi(this);
	
	connect(ui.pbSystemSettings, SIGNAL(clicked()), this, SLOT(switchToSystem()));
	connect(ui.pbSoundSettings, SIGNAL(clicked()), this, SLOT(switchToSound()));
	connect(ui.pbCommandSettings, SIGNAL(clicked()), this, SLOT(switchToCommands()));
	connect(ui.pbProtocolSettings, SIGNAL(clicked()), this, SLOT(switchToProtocols()));
	connect(ui.pbRevert, SIGNAL(clicked()), this, SLOT(switchToHistory()));
	
#ifdef linux
	ui.lbDirectX->setVisible(false);
#endif
#ifdef __win32
	ui.lbAlSA->setVisible(false);
#endif
}





/**********************************************/
/******************Tab Stuff*******************/
/**********************************************/

/**
 * \brief Untoggles all the "tab" buttons on top
 * 
 * \author Peter Grasch
 */
void SettingsView::unsetAllTabs()
{
	ui.pbSystemSettings->setChecked(false);
	ui.pbSoundSettings->setChecked(false);
	ui.pbCommandSettings->setChecked(false);
	ui.pbProtocolSettings->setChecked(false);
	ui.pbRevert->setChecked(false);
}

/**
 * \brief Switches to the "system" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToSystem()
{
	unsetAllTabs();
	
	ui.pbSystemSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 0 );
}

/**
 * \brief Switches to the "sound" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToSound()
{
	unsetAllTabs();
	
	ui.pbSoundSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 1 );
}

/**
 * \brief Switches to the "command" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToCommands()
{
	unsetAllTabs();
	
	ui.pbCommandSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 2 );
}

/**
 * \brief Switches to the "command" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToProtocols()
{
	unsetAllTabs();
	
	ui.pbProtocolSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 3 );
}

/**
 * \brief Switches to the "history" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToHistory()
{
	unsetAllTabs();
	
	ui.pbRevert->setChecked(true);
	ui.swSettings->setCurrentIndex( 4 );
}


/**
 * \brief Destructor
 * 
 * \author Peter Grasch
 */
SettingsView::~SettingsView()
{
}


