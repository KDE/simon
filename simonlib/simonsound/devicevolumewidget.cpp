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

#include "devicevolumewidget.h"
#include "simonsound.h"
#include "soundconfig.h"
#include "nullrecorderclient.h"
#include "ui_devicevolumewidget.h"
#include <QDateTime>
#include <KIcon>
#include <KLocalizedString>
#include <KMessageBox>

DeviceVolumeWidget::DeviceVolumeWidget( const SimonSound::DeviceConfiguration& device, SoundClient::SoundClientPriority inputPriority, QWidget* parent ) : QWidget(parent),
    ui(new Ui::DeviceVolumeWidgetUi()),
    rec(new NullRecorderClient(device, inputPriority, this)),
    m_deviceName(device.name()),
    lastClip(0),
    lastStartedSample(0),
    lastCompletedSample(0)
{
  ui->setupUi(this);
  ui->lbDeviceName->setText(i18nc("%1 is the devices name", "Device: %1", m_deviceName.remove(QRegExp("\\(.*\\)"))));
  connect(rec, SIGNAL(level(qint64,float)), this, SLOT(deviceReportedLevel(qint64,float)));
  connect(rec, SIGNAL(clippingOccured()), this, SLOT(clipping()));
  connect(rec, SIGNAL(sampleStarted()), this, SLOT(started()));
  connect(rec, SIGNAL(sampleCompleted()), this, SLOT(completed()));
  
  tooLow();
}

void DeviceVolumeWidget::deviceReportedLevel(qint64 time, float level)
{
  Q_UNUSED(time);
  ui->pbVolume->setValue(qRound(100*level));

  listOfLevels.append( qMakePair<qint64,float>(time,level) );

  /*
   * The frequency of the calls to deviceReportedLevel() is almost arbitrary and depends on hardware. Let's assume,
   * for example, that your hardware has a 200ms buffer. Then, to look at 10 or more levels of volume, you'd need
   * to monitor 2000 ms or 2 seconds of speech data. If the device uses a larger, 1 second buffer, you'd be looking at 10 seconds.
   * Now, to get the highest and the lowest volume level we need to have a minimum good number of data samples to look at (10 or more).
   * So, I'd say around 10 seconds would be reasonable so that the above condition (10 or more data samples) is satisfied.
   * So let's remove all records that are older than 10 seconds.
   */
  if( listOfLevels.last().first - listOfLevels.first().first >= 10000 )
    listOfLevels.takeFirst();

  highLevel = lowLevel = listOfLevels[0].second;

  for(int i=1; i<listOfLevels.length(); i++){
    highLevel = qMax<float>(listOfLevels[i].second,highLevel);
    lowLevel = qMin<float>(listOfLevels[i].second,lowLevel);
  }

  updateLabel();

}

void DeviceVolumeWidget::completed()
{
  lastCompletedSample = QDateTime::currentDateTime().toTime_t();
  updateLabel();
}

void DeviceVolumeWidget::started()
{
  lastStartedSample = QDateTime::currentDateTime().toTime_t();
  updateLabel();
}

void DeviceVolumeWidget::updateLabel()
{
  int window = 5;
  qint32 cur = QDateTime::currentDateTime().toTime_t();
  if (lastClip + window > cur) {
    tooLoud();
  } else {
    //let's check if we haven't detected a sample for at least 5 seconds - then the volume is
    //apparently too low. Otherwise it's perfect.
    if (lastCompletedSample < lastStartedSample){
      //we currently have a sample

      if(highLevel/lowLevel * 100 < SoundConfiguration::minimumSNR()){
        snrLow();
      }else{
        snrOk();
      }
    }
    else
      if (lastCompletedSample + window < cur)
        tooLow();
  }
}


void DeviceVolumeWidget::clipping()
{
  lastClip = QDateTime::currentDateTime().toTime_t();
  updateLabel();
}


void DeviceVolumeWidget::tooLoud()
{
  ui->lbStatus->setText(i18n("Lower the volume."));
  ui->lbIcon->setPixmap(KIcon("go-down").pixmap(24,24));
}


void DeviceVolumeWidget::snrOk()
{
  ui->lbStatus->setText(i18n("Volume correct."));
  ui->lbIcon->setPixmap(KIcon("dialog-ok-apply").pixmap(24,24));
}


void DeviceVolumeWidget::snrLow()
{
  ui->lbStatus->setText(i18n("High levels of background noise detected. Please check your microphone and disable \"Mic Boost\" if enabled."));
  ui->lbIcon->setPixmap(KIcon("dialog-error").pixmap(24,24));
}


void DeviceVolumeWidget::tooLow()
{
  ui->lbStatus->setText(i18n("Please speak (volume too low)."));
  ui->lbIcon->setPixmap(KIcon("go-up").pixmap(24,24));
}


void DeviceVolumeWidget::start()
{
  if (!rec->start())
    KMessageBox::error(this, i18nc("%1 is device name", "Recording could not be started for device: %1.", m_deviceName));
  else
    labelUpdater.start(1000);
}


void DeviceVolumeWidget::stop()
{
  if (!rec->finish())
    KMessageBox::error(this, i18nc("%1 is device name", "Recording could not be stopped for device: %1.", m_deviceName));
  else
    labelUpdater.stop();
}


DeviceVolumeWidget::~DeviceVolumeWidget()
{
  delete ui;
  rec->deleteLater();
}
