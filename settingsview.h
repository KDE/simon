//
// C++ Interface: settingsview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QDialog>
#include <QMessageBox>
#include "ui_settings.h"
#include "settingsmanager.h"

/**
	\class SettingsView
	
	\author Peter Grasch
	\version 0.1
	\todo Pretty much everything
	
	This Dialog provides an interface to all the not-so-commonly needed
	system settings/actions.
	Here you can configure simon, set up the paths to the components,
	view the logs, Browse through all the commands, add new or change and
	even delete them; or you can "undo" your changes
*/
class SettingsView : public QDialog
{
	Q_OBJECT
private:
	Ui::SettingsDialog ui; //!< The user interface
    SettingsManager *sm;
    void readConfig();
public slots:
	void unsetAllTabs();
	void switchToSystem();
	void switchToSound();
	void switchToCommands();
	void switchToProtocols();
	void switchToHistory();
	void apply();
	
public:
    SettingsView(QWidget *parent);
    
    
    ~SettingsView();

};



#endif
