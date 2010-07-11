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

#ifndef SIMON_KSIMONDSETTINGS_H_9157063F17C942C6BB790CDFB3C5B800
#define SIMON_KSIMONDSETTINGS_H_9157063F17C942C6BB790CDFB3C5B800

#include <KCModule>
#include <QVariantList>

#include "ui_ksimondconfiguration.h"

class KSimondSettings : public KCModule
{
  Q_OBJECT
    private:
    Ui::KSimondSettings ui;
  public:
    KSimondSettings(QWidget* parent, const QVariantList& args=QVariantList());
    void save();
    ~KSimondSettings();
};
#endif
