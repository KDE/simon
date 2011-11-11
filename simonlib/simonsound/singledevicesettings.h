/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SINGLEDEVICESETTINGS_H_D0C0BA2429B04F65935956A32C79AB09
#define SIMON_SINGLEDEVICESETTINGS_H_D0C0BA2429B04F65935956A32C79AB09

#include <QWidget>

#include "simonsound.h"

namespace Ui
{
  class SingleDeviceConfiguration;
}


class SingleDeviceSettings : public QWidget
{
  Q_OBJECT

    signals:
  void changed(bool);
  void requestRemove(SingleDeviceSettings*);

  private:
    Ui::SingleDeviceConfiguration *ui;
    bool enabled;
    bool hasChanged;

    SimonSound::SoundDeviceType m_type;
    QString m_deviceName;
    SimonSound::SoundDeviceUses m_uses;
    SimonSound::SoundDeviceOptions m_options;
    QString m_defaultSampleGroup;

    void load(QString deviceName, int channels,
      int sampleRate, bool resampleEnabled, int resampleSampleRate,
      QString defaultSampleGroup);

  private slots:
    void slotChanged();
    void sendRemoveRequest();

  public slots:
    bool check();
    void checkWithSuccessMessage();
    void disableSampleGroup();
    void enableSampleGroup();

  public:
    SingleDeviceSettings(SimonSound::SoundDeviceType type, QString deviceName, int channels,
      int sampleRate, bool resampleEnabled, int resampleSampleRate, 
      SimonSound::SoundDeviceUses selectedUses,
      SimonSound::SoundDeviceUses availableUses,
      QString defaultSampleGroup,
      SimonSound::SoundDeviceOptions options=SimonSound::NoOptions,
      QWidget* parent=0);
    ~SingleDeviceSettings();

    void enable();
    void disable();

    void refreshDevices();

    bool isEnabled();
    QString getSelectedDeviceId();
    int getSampleRate();
    int getChannels();
    bool getResampleEnabled();
    int getResampleSampleRate();
    QString getDefaultSampleGroup();
    SimonSound::SoundDeviceType getType();
    SimonSound::SoundDeviceUses getUses();

    bool getHasChanged() { return hasChanged; }
    void stored() { hasChanged = false; }
};
#endif                                            // SINGLEDEVICESETTINGS_H
