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

#ifndef SIMON_NETWORKSETTINGS_H_697FC1EF2FF14D8DAED685C07ADE9CC2
#define SIMON_NETWORKSETTINGS_H_697FC1EF2FF14D8DAED685C07ADE9CC2

#include "ui_networksettingsdlg.h"
#include <KCModule>
#include <QVariantList>

/**
 * \class NetworkSettings
 * \author Peter Grasch
 * \date 14.08.2007
 * \version 0.1
 * \brief KCModule to control the addresses to julius
 */
class NetworkSettings : public KCModule
{
  Q_OBJECT
    private:
    Ui::NetworkSettingsDlg ui;

  private slots:
    void configureSimond();

  public:
    explicit NetworkSettings(QWidget* parent, const QVariantList& args=QVariantList());
    ~NetworkSettings();

};
#endif
