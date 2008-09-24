//
// C++ Interface: externalprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDSETTINGS_H
#define COMMANDSETTINGS_H

#include "ui_commandsettingsdlg.h"
#include <QWidget>

/**
 * \class CommandSettings
 * \author Peter Grasch
 * \brief SystemWidgets that manages all the commands
 * \date 13.08.2007
 * \version 0.1
 */
class CommandSettings : public QWidget
{
Q_OBJECT
private:
	Ui::CommandSettingsDlg ui;
// public slots:
// 	bool apply();
// 	bool reset();
// 	bool init();
public:
    CommandSettings(QWidget* parent);
// 	bool isComplete();
    ~CommandSettings();

};

#endif
