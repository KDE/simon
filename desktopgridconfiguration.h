//
// C++ Interface: desktopgridconfiguration
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESKTOPGRIDCONFIGURATION_H
#define DESKTOPGRIDCONFIGURATION_H

#include "systemwidget.h"
#include "ui_desktopgridconfigurationdlg.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class DesktopGridConfiguration : public SystemWidget
{
Q_OBJECT
private:
	Ui::DesktopGridConfigurationDlg ui;
public slots:
	bool apply();
	bool reset();
	bool init();
public:
        DesktopGridConfiguration(QWidget* parent);


        ~DesktopGridConfiguration();

};

#endif
