/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SIMONDNETWORKCONFIGURATION_H_932B7362E7CC428398A5F279795080B6
#define SIMON_SIMONDNETWORKCONFIGURATION_H_932B7362E7CC428398A5F279795080B6

#include <KCModule>
#include <QVariantList>

#include "ui_simondnetworkconfiguration.h"

class SimondNetworkConfiguration : public KCModule
{
  Q_OBJECT
    private:
    Ui::NetworkConfiguration ui;
  private slots:
    void slotChanged();
  public:
    explicit SimondNetworkConfiguration(QWidget* parent, const QVariantList& args=QVariantList());
    void save();
    void load();
    ~SimondNetworkConfiguration();
};
#endif
