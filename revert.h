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
 \class Revert
 \author Peter Grasch
 \brief Provides a SystemWidget to restore a previous state of the system
 \version 0.1
 \date 13.8.2007
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
