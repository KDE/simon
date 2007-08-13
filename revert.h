//
// C++ Interface: revert
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef REVERT_H
#define REVERT_H

#include "systemwidget.h"
#include "ui_revertdlg.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class Revert : public SystemWidget {
Q_OBJECT
private:
	Ui::RevertDlg ui;
public slots:
	bool init();
	bool apply();
	bool reset();
public:
    Revert(QWidget *parent);

    ~Revert();

};

#endif
