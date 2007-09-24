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
#ifndef MODELLSETTINGS_H
#define MODELLSETTINGS_H

#include "systemwidget.h"
#include "ui_modelsettingsdlg.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ModelSettings : public SystemWidget
{
Q_OBJECT
private:
        Ui::ModelSettingsDlg ui;
private slots:
	void deleteFilter();
	void addFilter();
	void insertFilter(QString command);
	void enableButtons();
	void moveUp();
	void moveDown();

public slots:
        bool apply();
        bool init();
        bool reset();
public:
        ModelSettings(QWidget* parent);

        ~ModelSettings();


};

#endif
