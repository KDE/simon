//
// C++ Interface: command
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef COMMANDPREVIEWWIDGET_H
#define COMMANDPREVIEWWIDGET_H

#include <QModelIndex>
#include "ui_commandpreviewwidget.h"

class Command;

class CommandPreviewWidget : public QWidget {

Q_OBJECT

private:
	Ui::CommandPreviewDlg ui;
	Command *command;

private slots:
	void trigger();

public slots:
	void updateCommand(const QModelIndex &commandIdx);

public:
	CommandPreviewWidget(QWidget *parent=0);
	~CommandPreviewWidget() {}

};



#endif
