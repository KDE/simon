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

#ifndef CREATETEXTMACROCOMMANDWIDGET_H
#define CREATETEXTMACROCOMMANDWIDGET_H

#include <QWidget>
#include <createcommandwidget.h>
#include "ui_createtextmacrocommandwidget.h"

class Command;

/**
 *	@class CreateTextMacroCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an TextMacroCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreateTextMacroCommandWidget : public CreateCommandWidget{
Q_OBJECT

private:
	Ui::CreateTextMacroCommandWidget ui;

public:
	Command* createCommand(const QString& name, const QString& iconSrc);

	bool init(Command* command);
	bool isComplete();

	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CreateTextMacroCommandWidget(QWidget *parent=0);


	virtual ~CreateTextMacroCommandWidget();

};

#endif
