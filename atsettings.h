//
// C++ Interface: atsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ATSETTINGS_H
#define ATSETTINGS_H

#include "systemwidget.h"
#include "ui_atsettings.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ATSettings : public SystemWidget
{
private:
	Ui::ATSettingsDlg ui;
public:
    ATSettings(QWidget* parent);

    ~ATSettings();

    virtual bool apply();
    virtual bool init();
    virtual bool isComplete();
    virtual bool reset();

};

#endif
