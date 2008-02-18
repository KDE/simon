//
// C++ Interface: generalsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include "systemwidget.h"
#include "ui_generalsettingsdlg.h"

/**
 \class GeneralSettings
 \author Peter Grasch
 \version 0.1
 \date 12.08.2007

 \brief Used to control the genernal system settings (like paths to the language model, etc.)

*/
class GeneralSettings : public SystemWidget
{
Q_OBJECT

private:
	Ui::GeneralSettingsDlg ui; //!< UI definition - made by uic from the QTDesigner .ui
	
public slots:
	bool apply();
	bool init();
	bool reset();
public:
    GeneralSettings(QWidget* parent);
	bool isComplete();

    ~GeneralSettings();

};

#endif
