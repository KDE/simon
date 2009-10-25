/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CREATESHORTCUTCOMMANDWIDGET_H
#define CREATESHORTCUTCOMMANDWIDGET_H

#include <QWidget>
#include <simonscenarios/createcommandwidget.h>
#include "ui_createshortcutcommandwidget.h"


class Command;

/**
 *	@class CreateShortcutCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an ShortcutCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreateShortcutCommandWidget : public CreateCommandWidget{
Q_OBJECT

private:
	Ui::CreateShortcutCommandWidget ui;
	
private slots:
	#ifdef Q_OS_WIN
	void applySpecialShortcut();
	#endif
	
public:
	Command* createCommand(const QString& name, const QString& iconSrc);

	bool init(Command* command);
	bool isComplete();
	

	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CreateShortcutCommandWidget(QWidget *parent=0);


	virtual ~CreateShortcutCommandWidget();

};

#endif
