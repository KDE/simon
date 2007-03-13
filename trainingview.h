//
// C++ Interface: trainingview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRAININGVIEW_H
#define TRAININGVIEW_H

#include <QDialog>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include "trainingtext.h"
#include "trainingmanager.h"
#include "wordlistview.h"
#include "ui_trainmain.h"

/**
 *	@class TrainingView
 *	@brief Provides the UI for the Training process
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class TrainingView : public QDialog{
	Q_OBJECT
private:
	Ui::TrainMain ui;	//!< UI definition - made by uic from the QTDesigner .ui
	TrainingManager *trainMgr;
public slots:
	void switchToWordList();
	void loadList();
public:
	
    TrainingView(QWidget *parent = 0);
	
    ~TrainingView();

};

#endif
