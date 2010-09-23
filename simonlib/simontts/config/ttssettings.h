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

#ifndef SIMON_TTSSETTINGS_H_D92C3FEDF32B415094257DB891621383
#define SIMON_TTSSETTINGS_H_D92C3FEDF32B415094257DB891621383

#include "ui_simonttsconfigurationdlg.h"
#include <KCModule>
#include <QVariantList>

/**
  @author Peter Grasch <grasch@simon-listens.org>
*/
class TTSSettings : public KCModule
{
  Q_OBJECT
  private:
    Ui::TTSConfiguration ui;
    QString translateBackend(const QString& backend);

  private slots:
    void slotChanged();

  public:
    explicit TTSSettings(QWidget* parent, const QVariantList& args=QVariantList());

    void load();
    void save();
    void defaults();

    ~TTSSettings();

};
#endif
