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

#include "volumewidget.h"
#include "devicevolumewidget.h"
#include "soundconfig.h"
#include "soundserver.h"
#include "ui_volumewidget.h"
#include <simonsound/soundserver.h>
#include <KLocalizedString>
#include <QVBoxLayout>

VolumeWidget::VolumeWidget(QWidget* parent, SoundClient::SoundClientPriority inputPriority) : QWidget(parent),
ui(new Ui::VolumeWidgetUi()), priority(inputPriority)
{
  ui->setupUi(this);
  ui->wgStart->show();
  
  connect(ui->pbStartCalibration, SIGNAL(clicked()), this, SLOT(start()));
  connect(SoundServer::getInstance(), SIGNAL(devicesChanged()), this, SLOT(init()));
}

void VolumeWidget::enablePrompt(bool enabled)
{
  ui->tePrompt->setVisible(enabled);
}

void VolumeWidget::init()
{
  bool wasStarted = ui->wgStart->isHidden();
  kDebug() << "Reinitializing devices for volume widget";
  setPrompt(SoundConfiguration::volumePrompt());
  stop();

  qDeleteAll(devices);
  devices.clear();

  QList<SimonSound::DeviceConfiguration> devices = SoundServer::getTrainingInputDevices();
  foreach (const SimonSound::DeviceConfiguration& device, devices) {
    registerClient(device);
  }
  if (wasStarted)
    start();
}

void VolumeWidget::registerClient(const SimonSound::DeviceConfiguration& device)
{
  DeviceVolumeWidget *dw = new DeviceVolumeWidget(device, priority, this);

  QVBoxLayout *lay = dynamic_cast<QVBoxLayout*>(layout());
  Q_ASSERT(lay);

  lay->addWidget(dw);
  devices << dw;
}

void VolumeWidget::setPrompt(const QString& prompt)
{
  ui->tePrompt->setFont(SoundConfiguration::promptFont());
  ui->tePrompt->setText(prompt);

  if (ui->tePrompt->toPlainText().isEmpty())
    ui->tePrompt->setPlainText(i18n("Please say a few sentences. Try to pronounce them clearly but naturally and speak in a normal volume."));
}

void VolumeWidget::start()
{
  foreach (DeviceVolumeWidget *dw, devices) {
    dw->start();
    dw->show();
  }
  ui->wgStart->hide();
}

void VolumeWidget::stop()
{
  foreach (DeviceVolumeWidget *dw, devices) {
    dw->stop();
    dw->hide();
  }
  ui->wgStart->show();
}

VolumeWidget::~VolumeWidget()
{
  stop();
  delete ui;
}
