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

#ifndef SIMON_GENERALSETTINGS_H_DBC2F6C6A45D47759852693EE9A3A8CD
#define SIMON_GENERALSETTINGS_H_DBC2F6C6A45D47759852693EE9A3A8CD

#include "ui_generalsettingsdlg.h"
#include <KCModule>
#include <QVariantList>

/**
 \class GeneralSettings
 \author Peter Grasch
 \version 0.1
 \date 12.08.2007

 \brief Used to control the general system settings

*/
class GeneralSettings : public KCModule
{
  Q_OBJECT

  private:
    Ui::GeneralSettingsDlg ui;                    //!< UI definition - made by uic from the QTDesigner .ui

  private slots:
    void slotChanged();

  public:
    explicit GeneralSettings(QWidget* parent, const QVariantList& args=QVariantList());
    void load();
    void save();
};
#endif
