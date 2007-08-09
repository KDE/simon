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
#include <QSettings>
#include "soundcontrol.h"
#include "sounddevice.h"
#include <QList>
#include <QInputDialog>
#include "xmlcommand.h"
#include <QStringList>

#include "logmanager.h"

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
    QSettings *settings;
    SoundControl *sc;
	XMLCommand *commandLoader;
    void readConfig();
	
	LogManager *manager;


private slots:
	void initCommands(QString path="conf/commands.xml");
	void saveCommands();
	//void editCommands();

public slots:
	void unsetAllTabs();
	void switchToSystem();
	void switchToSound();
	void switchToCommands();
	void switchToProtocols();
	void switchToHistory();
	void apply();
	void refreshDeviceCapabilities();
	void deleteAddress();
	void addAddress();
	void insertEntries(LogEntryList entries);
	LogEntryList  getEntries(QDate *day);
	void onlyDay();
	void newCommand();
	void deleteCommand();
	void activateCb();
	void deactivateCb(int currRow, int currCol, int prevRow, int prevCol);
	void editCommand();
	void showOnlyCommands();
	void reloadCommands();
	void leaveProtocol();
	
	void logReadFinished(int value);

	void clearSearchLineEdit();
	void searchCommandList();
	
public:
    SettingsView(QWidget *parent);
    
    ~SettingsView();

signals:
	void logReadStop();

};



#endif
