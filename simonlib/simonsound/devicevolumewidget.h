/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_DEVICEVOLUMEWIDGET_H_33F50DCCCC3D401FADDFBFD80B4E16F4
#define SIMON_DEVICEVOLUMEWIDGET_H_33F50DCCCC3D401FADDFBFD80B4E16F4

#include "simonsound_export.h"
#include "soundclient.h"
#include <QWidget>
#include <QTimer>

namespace Ui
{
  class DeviceVolumeWidgetUi;
}


namespace SimonSound
{
  class DeviceConfiguration;
}


class NullRecorderClient;

/**
 * \class DeviceVolumeWidget
 * \author Peter Grasch
 * \date 2.06.2010
 */
class SIMONSOUND_EXPORT DeviceVolumeWidget : public QWidget
{
  Q_OBJECT
  
  public:
    explicit DeviceVolumeWidget( const SimonSound::DeviceConfiguration& device, SoundClient::SoundClientPriority inputPriority, QWidget* parent = 0 );
    ~DeviceVolumeWidget();

    void start();
    void stop();

  private:
    Ui::DeviceVolumeWidgetUi *ui;
    NullRecorderClient *rec;
    
    QString m_deviceName;
    
    qint32 lastClip;
    qint32 lastStartedSample;
    qint32 lastCompletedSample;
    
    QTimer labelUpdater;

  private slots:
    void deviceReportedLevel(qint64 time, float level);

    void clipping();
    void started();
    void completed();
    
    void tooLoud();
    void volumeOk();
    void tooLow();
    
    void updateLabel();

};
#endif
