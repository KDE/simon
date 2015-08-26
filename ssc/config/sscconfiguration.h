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

#ifndef SIMON_SSCCONFIGURATION_H_CE1378CB6EFC48349C931195FD6112CA
#define SIMON_SSCCONFIGURATION_H_CE1378CB6EFC48349C931195FD6112CA

#include <QVariantList>
#include <KConfigWidgets/KCModule>

#include "ui_sscconfiguration.h"

class SSCConfiguration : public KCModule
{
  Q_OBJECT
    private:
    Ui::SSCConfiguration ui;

  private slots:
    void slotChanged();
  public:
    explicit SSCConfiguration(QWidget* parent, const QVariantList& args=QVariantList());
    void save();
    void load();
    virtual ~SSCConfiguration();
};
#endif
