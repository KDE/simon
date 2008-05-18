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

#include "../../../SimonLib/SpeechGUI/systemwidget.h"
#include "ui_desktopgridconfigurationdlg.h"

/**
 \class DesktopGridConfiguration
 \author Peter Grasch
 \version 0.1
 \date 16.9.2007
 \brief Configures the desktopgrid
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

	bool isComplete();

};

#endif
