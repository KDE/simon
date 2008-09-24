//
// C++ Interface: modellsettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MODELSETTINGS_H
#define MODELSETTINGS_H

#include "ui_modelsettingsdlg.h"
#include <QWidget>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ModelSettings : public QWidget
{
Q_OBJECT
private:
        Ui::ModelSettingsDlg ui;

// public slots:
//         bool apply();
//         bool init();
//         bool reset();
// 	bool isComplete();
public:
        ModelSettings(QWidget* parent);

        ~ModelSettings();


};

#endif
