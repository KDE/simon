//
// C++ Interface: internetextensionsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INTERNETEXTENSIONSETTINGS_H
#define INTERNETEXTENSIONSETTINGS_H

#include "systemwidget.h"
#include "ui_internetextensiondlg.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class InternetExtensionSettings : public SystemWidget
{
private:
	Ui::InternetExtensionDlg ui;
public:
    InternetExtensionSettings(QWidget* parent);

    ~InternetExtensionSettings();

    virtual bool apply();
    virtual bool init();
    virtual bool isComplete();
    virtual bool reset();

};

#endif
