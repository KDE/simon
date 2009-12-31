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

#ifndef SIMON_CREATEPLACECOMMANDWIDGET_H_ACEC852968EA4E5292BEFBCDC5983F84
#define SIMON_CREATEPLACECOMMANDWIDGET_H_ACEC852968EA4E5292BEFBCDC5983F84

#include <QWidget>
#include <simonscenarios/createcommandwidget.h>
#include "ui_createplacecommandwidget.h"

class Command;
class ImportPlaceWizard;
class CommandManager;

/**
 *	@class CreatePlaceCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an PlaceCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreatePlaceCommandWidget : public CreateCommandWidget{
Q_OBJECT

private:
	Ui::CreatePlaceCommandWidget ui;
	ImportPlaceWizard *importWizard;

private slots:
	void showImportWizard();

public:
	Command* createCommand(const QString& name, const QString& iconSrc);

	bool init(Command* command);
	bool isComplete();

	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CreatePlaceCommandWidget(CommandManager *manager, QWidget *parent=0);


	virtual ~CreatePlaceCommandWidget();

};

#endif
