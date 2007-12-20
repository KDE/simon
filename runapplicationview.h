//
// C++ Interface: runapplicationview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RUNAPPLICATIONVIEW_H
#define RUNAPPLICATIONVIEW_H

#include "inlinewidget.h"
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include "simoninfo.h"
#include "ui_rundialog.h"

#include "runcommand.h"

/**
 *	@class RunApplicationView
 *	@brief Provides a graphical frontend to run commands and perform 
 * 	system actions
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class RunApplicationView : public InlineWidget {
	Q_OBJECT

signals:
	void hidden();
	
private:
	Ui::RunDlg ui;
	RunCommand *run;
	void hideEvent(QHideEvent *event) { emit hidden(); return QWidget::hideEvent(event); }
public slots:
	void insertCommands(CommandList list);
	void runCommand(QTableWidgetItem* command);
	void runSelectedCommand();
	void clearSearchText();
	void filterByPattern();
	void filterByPattern(QString filter, CommandList *commlist=NULL);
	CommandList filterByCategory(int commandtype, CommandList *commlist=NULL);
	CommandList filterByStrCategory(QString commandtype, CommandList *commlist=NULL);
	int getCategory(QString commandtype);
    void loadCommands();
public:
    RunApplicationView(RunCommand *run, QWidget *parent);

    ~RunApplicationView();

};

#endif
