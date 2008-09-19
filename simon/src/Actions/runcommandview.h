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
#ifndef RUNCOMMANDVIEW_H
#define RUNCOMMANDVIEW_H

#include "inlinewidget.h"
#include "ui_rundialog.h"

class Command;
class RunCommand;
class CommandPreviewWidget;
/**
 *	@class RunCommandView
 *	@brief Provides a graphical frontend to run commands and perform 
 * 	system actions
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class RunCommandView : public InlineWidget {
	Q_OBJECT

private:
	Ui::RunDlg ui;
	Command* getCommandToModify();

private slots:
	void addCommand();
	void deleteCommand();
	void editCommand();
	void reflectSelectionStatus(QModelIndex index);

public slots:
	void loadCommands();
	void setSettingsVisible();
	void setSettingsHidden();

public:
    RunCommandView(QWidget *parent);

    ~RunCommandView();

};

#endif
