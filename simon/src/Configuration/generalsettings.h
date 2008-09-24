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

#include "ui_generalsettingsdlg.h"
#include <QWidget>

/**
 \class GeneralSettings
 \author Peter Grasch
 \version 0.1
 \date 12.08.2007

 \brief Used to control the genernal system settings (like paths to the language model, etc.)

*/
class GeneralSettings : public QWidget
{
Q_OBJECT

private:
	Ui::GeneralSettingsDlg ui; //!< UI definition - made by uic from the QTDesigner .ui
	
public:
    GeneralSettings(QWidget* parent);

    ~GeneralSettings();

};

#endif
