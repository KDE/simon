//
// C++ Interface: externalprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EXTERNALPROGRAMMANAGER_H
#define EXTERNALPROGRAMMANAGER_H

#include "systemwidget.h"
#include "ui_externalprogramms.h"

/**
 * \class ExternalProgramManager
 * \author Peter Grasch
 * \brief SystemWidgets that manages the paths to external programs
 * \date 13.08.2007
 * \version 0.1
 */
class ExternalProgramManager : public SystemWidget
{
Q_OBJECT
private:
	Ui::ExternalProgrammsDlg ui;
public slots:
	bool apply();
	bool reset();
	bool init();
public:
    ExternalProgramManager(QWidget* parent);
	bool isComplete();
    ~ExternalProgramManager();

};

#endif
