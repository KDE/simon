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

#ifndef CREATECOMMANDWIDGET_H
#define CREATECOMMANDWIDGET_H

#include "simoncommandpluginbase_export.h"

#include <QWidget>

class Command;
class KIcon;

/**
 *	@class CreateCommandWidget
 *	@brief Provides an abstract base-class for the widget that is going to be displayed in the New/Modify-Command-Dialog
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class SIMONCOMMANDPLUGINBASE_EXPORT CreateCommandWidget : public QWidget{
Q_OBJECT

signals:
	/**
	* \brief Emits true/false when the user entered all/not all mandatory fields
	* \author Peter Grasch
	*/
	void complete(bool);

public:
	/**
	* @brief Creates the command
	* @author Peter Grasch
	* 
	* This function gets the common parameters for all commands (name and iconSrc).
	* A plugin implementing this will then add the aditional parameters of his 
	* Command-derived class (like the path to the executable in an ExecutableCommand)
	* and create an instance of this derived class.
	* 
	* If you don't have enough information to create the command (i.e. the user didn't
	* enter all the required fields), return a Null-Pointer.
	*
	*/
	virtual Command* createCommand(const QString& name, const QString& iconSrc)=0;

	virtual bool init(Command* command)=0;

	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CreateCommandWidget(QWidget *parent=0) : QWidget(parent)
	{}


	virtual ~CreateCommandWidget() {}

};

#endif
