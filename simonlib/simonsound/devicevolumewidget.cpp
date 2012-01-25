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

#include "devicevolumewidget.h"
#include "simonsound.h"
#include "soundconfig.h"
#include "nullrecorderclient.h"
#include "ui_devicevolumewidget.h"
#include <KIcon>
#include <KLocalizedString>
#include <KMessageBox>

DeviceVolumeWidget::DeviceVolumeWidget(const SimonSound::DeviceConfiguration& device, QWidget *parent) : QWidget(parent),
ui(new Ui::DeviceVolumeWidgetUi()),
rec(new NullRecorderClient(device, this)),
m_deviceName(device.name()),
m_isTooLoud(false)
{
  ui->setupUi(this);
  if (m_deviceName.contains("CARD="))
    m_deviceName = m_deviceName.mid(m_deviceName.indexOf("CARD=")+5);
  ui->lbDeviceName->setText(i18nc("%1 is the devices name", "Device \"%1\":", m_deviceName));
  connect(rec, SIGNAL(level(qint64,float)), this, SLOT(deviceReportedLevel(qint64,float)));
  connect(rec, SIGNAL(clippingOccured()), this, SLOT(clipping()));

  connect(ui->pbVolumeChanged, SIGNAL(clicked()), this, SLOT(reset()));

  tooLow();

  ui->pbVolumeChanged->setIcon(KIcon("view-refresh"));
}


void DeviceVolumeWidget::reset()
{
  m_isTooLoud = false;
  tooLow();
}


void DeviceVolumeWidget::deviceReportedLevel(qint64 time, float level)
{
  Q_UNUSED(time);
  ui->pbVolume->setValue(qRound(100*level));

  if ((level > (SoundConfiguration::calibrateMinVolume() / 100.0f)) &&
  !m_isTooLoud) {
    if (level < (SoundConfiguration::calibrateMaxVolume() / 100.0f)) {
      volumeOk();
    }
    else {
      tooLoud();
      m_isTooLoud = true;
    }
  }
}


void DeviceVolumeWidget::clipping()
{
  m_isTooLoud = true;
  tooLoud();
}


void DeviceVolumeWidget::tooLoud()
{
  ui->lbStatus->setText(i18n("Lower the volume."));
  ui->lbIcon->setPixmap(KIcon("go-down").pixmap(24,24));
}


void DeviceVolumeWidget::volumeOk()
{
  ui->lbStatus->setText(i18n("Volume correct!"));
  ui->lbIcon->setPixmap(KIcon("dialog-ok-apply").pixmap(24,24));
}


void DeviceVolumeWidget::tooLow()
{
  ui->lbStatus->setText(i18n("Raise the volume."));
  ui->lbIcon->setPixmap(KIcon("go-up").pixmap(24,24));
}


void DeviceVolumeWidget::start()
{
  if (!rec->start())
    KMessageBox::error(this, i18nc("%1 is device name", "Recording could not be started for device: %1.", m_deviceName));
}


void DeviceVolumeWidget::stop()
{
  if (!rec->finish())
    KMessageBox::error(this, i18nc("%1 is device name", "Recording could not be stopped for device: %1.", m_deviceName));
}


DeviceVolumeWidget::~DeviceVolumeWidget()
{
  delete ui;
  rec->deleteLater();
}
