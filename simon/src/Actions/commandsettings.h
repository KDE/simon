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

#ifndef COMMANDSETTINGS_H
#define COMMANDSETTINGS_H

#include "ui_commandsettingsdlg.h"
#include <QWidget>

/**
 * \class CommandSettings
 * \author Peter Grasch
 * \brief SystemWidgets that manages all the commands
 * \date 13.08.2007
 * \version 0.1
 */
class CommandSettings : public QWidget
{
Q_OBJECT
private:
	Ui::CommandSettingsDlg ui;
// public slots:
// 	bool apply();
// 	bool reset();
// 	bool init();
public:
    CommandSettings(QWidget* parent=0);
// 	bool isComplete();
    ~CommandSettings();

};

#endif
