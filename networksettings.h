//
// C++ Interface: networksettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include "systemwidget.h"
#include "ui_networksettingsdlg.h"

/**
 * \class NetworkSettings
 * \author Peter Grasch
 * \date 14.08.2007
 * \version 0.1
 * \brief SystemWidget to control the addresses to julius
 */
class NetworkSettings : public SystemWidget
{
Q_OBJECT
private:
	Ui::NetworkSettingsDlg ui;
private slots:
	void deleteAddress();
	void addAddress();
	void insertAddress(QString host, int port);
	void enableButtons();
	void moveUp();
	void moveDown();
	
public slots:
	bool apply();
	bool reset();
	bool init();
	
public:
        NetworkSettings(QWidget* parent);

        ~NetworkSettings();

};

#endif
