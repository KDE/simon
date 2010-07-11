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

#ifndef SIMON_DEVICESETTINGS_H_D0C3C07429B04F65935956A32C79AB09
#define SIMON_DEVICESETTINGS_H_D0C3C07429B04F65935956A32C79AB09

#include <QWidget>
#include <QList>
#include "simonsound_export.h"

namespace Ui
{
  class DeviceConfiguration;
}


class SingleDeviceSettings;
class QScrollArea;

class SIMONSOUND_EXPORT DeviceSettings : public QWidget
{
  Q_OBJECT

    signals:
  void changed(bool);

  private:
    Ui::DeviceConfiguration *ui;

    QList<SingleDeviceSettings*> inputDevices;
    QList<SingleDeviceSettings*> outputDevices;

    QString getSelectedInputDeviceId();
    QString getSelectedOutputDeviceId();

    bool enabled;

    void enable();
    void disable();

    void registerDevice(SingleDeviceSettings*, QScrollArea*);
    SingleDeviceSettings* getDevice(int index, QScrollArea*);

    void registerInputDevice(SingleDeviceSettings*);
    SingleDeviceSettings* getInputDevice(int index);

    void registerOutputDevice(SingleDeviceSettings*);
    SingleDeviceSettings* getOutputDevice(int index);

  public slots:
    void load();
    void save();

  private slots:
    void refreshDevices();

    bool check();
    void checkWithSuccessMessage();
    void slotChanged();

    void addInputDevice();
    void addOutputDevice();

    void removeDevice(SingleDeviceSettings*);

  public:
    DeviceSettings(QWidget* parent);
    ~DeviceSettings();

};
#endif
