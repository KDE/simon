//
// C++ Interface: passwordsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PASSWORDSETTINGS_H
#define PASSWORDSETTINGS_H

#include <systemwidget.h>
#include "ui_passwordsettingsdlg.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class PasswordSettings : public SystemWidget
{
Q_OBJECT
private:
	Ui::PasswordSettingsDlg ui;
	bool checkIfPasswordsSane();
	bool isHashed;
	QString originalHash;
private slots:
	void checkPassword();
	void resetButtonClicked(bool isDown);
	void unsetHashed()
	{ isHashed=false; }
public:
    PasswordSettings(QWidget* parent);

    ~PasswordSettings();

public slots:
    bool apply();
    bool init();
    bool reset();

};

#endif
