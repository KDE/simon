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


#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include "ui_networksettingsdlg.h"
#include <QWidget>

/**
 * \class NetworkSettings
 * \author Peter Grasch
 * \date 14.08.2007
 * \version 0.1
 * \brief SystemWidget to control the addresses to julius
 */
class NetworkSettings : public QWidget
{
Q_OBJECT
private:
	Ui::NetworkSettingsDlg ui;
	
public:
        NetworkSettings(QWidget* parent);
        ~NetworkSettings();

};

#endif
