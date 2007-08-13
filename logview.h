//
// C++ Interface: logview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "systemwidget.h"
#include "ui_logmanagerdlg.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LogView : public SystemWidget
{
	Q_OBJECT
	
private: 
	Ui::LogManagerDlg ui;

public slots:
	void enter();
	void leave();
	bool apply();
	bool init();
	bool reset();

public:
    LogView(QWidget* parent);

    ~LogView();

};

#endif
