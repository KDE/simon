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

#ifndef CREATEEXECUTABLECOMMANDWIDGET_H
#define CREATEEXECUTABLECOMMANDWIDGET_H

#include <QWidget>
#include <commandpluginbase/createcommandwidget.h>
#include "ui_createexecutablecommandwidget.h"

class Command;
class ImportProgramWizard;

/**
 *	@class CreateExecutableCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an ExecutableCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreateExecutableCommandWidget : public CreateCommandWidget{
Q_OBJECT

private:
	Ui::CreateExecutableCommandWidget ui;
	ImportProgramWizard *importWizard;

private slots:
	void showImportWizard();
	void wizardInit(Command *command);

public:
	Command* createCommand(const QString& name, const QString& iconSrc);

	bool init(Command* command);
	bool isComplete();

	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CreateExecutableCommandWidget(QWidget *parent=0);


	virtual ~CreateExecutableCommandWidget();

};

#endif
