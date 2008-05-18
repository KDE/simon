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
#include "command.h"
#include "ui_rundialog.h"

class RunCommand;
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

private:
	Ui::RunDlg ui;
	RunCommand *run;

private slots:
	void addCommand();
	void deleteCommand();
	void editCommand();
	void insertCommands(CommandList list);
	void runSelectedCommand();
	void filterByPattern(QString filter="");

public slots:
	void loadCommands();
	void setSettingsVisible();
	void setSettingsHidden();

public:
    RunApplicationView(QWidget *parent);

    ~RunApplicationView();

};

#endif
