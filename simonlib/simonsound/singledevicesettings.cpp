/* *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "singledevicesettings.h"
#include "soundserver.h"

#include <KMessageBox>
#include <KIcon>

#include <KLocalizedString>
#include <KDebug>

#include "ui_singledeviceconfiguration.h"

SingleDeviceSettings::SingleDeviceSettings(SimonSound::SoundDeviceType type, QString deviceName, int channels,
int sampleRate, bool resampleEnabled, int resampleSampleRate, SimonSound::SoundDeviceUses selectedUses, 
SimonSound::SoundDeviceUses availableUses,
QString defaultSampleGroup,
SimonSound::SoundDeviceOptions options,
QWidget* parent): QWidget(parent),
enabled(true),
hasChanged(true),
m_type(type),
m_deviceName(deviceName),
m_uses(selectedUses),
m_options(options),
m_defaultSampleGroup(defaultSampleGroup)
{
  ui = new Ui::SingleDeviceConfiguration();

  ui->setupUi(this);

  if (!(availableUses & SimonSound::Training))
    ui->cbTraining->hide();
  if (!(availableUses & SimonSound::Recognition))
    ui->wgRecognition->hide();

#ifndef HAVE_LIBSAMPLERATE_H
  ui->sbResampleSampleRate->hide();
  ui->lbHzResample->hide();
  ui->lbResample->hide();
  ui->cbResample->hide();
#else
  ui->sbResampleSampleRate->setVisible((type == SimonSound::Input));
  ui->lbHzResample->setVisible((type == SimonSound::Input));
#endif

  ui->pbTest->setIcon(KIcon("help-hint"));
  ui->pbRemove->setIcon(KIcon("list-remove"));
  ui->pbAdvancedConfiguration->setIcon(KIcon("configure"));
  ui->pbAddCondition->setIcon(KIcon("list-add"));
  ui->pbEditCondition->setIcon(KIcon("document-edit"));
  ui->pbRemoveCondition->setIcon(KIcon("list-remove"));

  if (!(options & SimonSound::Removable))
    ui->pbRemove->hide();

  connect(ui->pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
  connect(ui->pbRemove, SIGNAL(clicked()), this, SLOT(sendRemoveRequest()));
  connect(ui->cbSoundDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));

  connect(ui->sbChannels, SIGNAL(valueChanged(int)), this, SLOT(slotChanged()));
  connect(ui->sbSampleRate, SIGNAL(valueChanged(int)), this, SLOT(slotChanged()));
  connect(ui->cbTraining, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui->cbRecognition, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));

  connect(ui->cbResample, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui->sbResampleSampleRate, SIGNAL(valueChanged(int)), this, SLOT(slotChanged()));

  connect(ui->leSampleGroup, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));

  load(deviceName, channels, sampleRate, resampleEnabled, resampleSampleRate, defaultSampleGroup);
  
  ui->wgAdvancedOptions->hide();
}

void SingleDeviceSettings::sendRemoveRequest()
{
  emit requestRemove(this);
}


void SingleDeviceSettings::checkWithSuccessMessage()
{
  if (!enabled) return;
  if (check())
    KMessageBox::information(this, i18n("The soundconfiguration has been tested successfully."));
}


void SingleDeviceSettings::refreshDevices()
{
  if (!getSelectedDeviceId().isEmpty())
    m_deviceName = getSelectedDeviceId();

  if (!getDefaultSampleGroup().isEmpty())
      m_defaultSampleGroup = getDefaultSampleGroup();

  load(m_deviceName, getChannels(), getSampleRate(), getResampleEnabled(), getResampleSampleRate(), m_defaultSampleGroup);
}


void SingleDeviceSettings::load(QString deviceName, int channels,
int sampleRate, bool resampleEnabled, int resampleSampleRate,
QString defaultSampleGroup)
{
  ui->cbSoundDevice->clear();
  ui->cbSoundDevice->addItems(SoundServer::getDevices(m_type));

  //select
  ui->cbSoundDevice->setCurrentIndex(ui->cbSoundDevice->findText(deviceName));

  ui->sbChannels->setValue(channels);
  ui->sbSampleRate->setValue(sampleRate);

  ui->cbResample->setChecked(resampleEnabled);
  ui->sbResampleSampleRate->setValue(resampleSampleRate);

  ui->cbRecognition->setChecked(m_uses & SimonSound::Recognition);
  ui->wgRecognitionConditions->setEnabled(ui->cbRecognition->isChecked());
  
  ui->cbTraining->setChecked(m_uses & SimonSound::Training);

  ui->leSampleGroup->setText(defaultSampleGroup);

  hasChanged=false;
  if ((!deviceName.isEmpty()) &&
  (ui->cbSoundDevice->currentText() != deviceName)) {
    if (KMessageBox::questionYesNoCancel(this, i18nc("%1 is the device name",
            "<html><head/><body><p>simon noticed that the sound device "
            "\"%1\" is no longer available.</p><p>This is perfectly normal if you are "
            "connected to simond or are otherwise using an application that blocks the "
            "device.</p><p><b>Did you plug / unplug a device or otherwise change your systems "
            "audio setup?</b></p><p>Selecting \"Yes\" will allow you to change your sound "
            "configuration, essentially deleting your previous configuration. Selecting "
            "\"No\" will temporarily deactivate the sound configuration in order to "
            "protect your previous configuration from being overwritten.</body></html>", 
            deviceName)) == KMessageBox::Yes) {
      QString defaultDevice; 

      if (m_type == SimonSound::Input)
        defaultDevice = SoundServer::defaultInputDevice();
      else
        defaultDevice = SoundServer::defaultOutputDevice();

      ui->cbSoundDevice->setCurrentIndex(
        ui->cbSoundDevice->findText(defaultDevice));
      hasChanged=true;
      KMessageBox::information(this, i18n("Please adjust your soundconfiguration accordingly."));
      enable();
    } else disable();

  } else enable();

  if (hasChanged) emit changed(true);
}


void SingleDeviceSettings::enable()
{
  ui->frmDevice->setEnabled(true);
  enabled=true;
}


void SingleDeviceSettings::disable()
{
  ui->frmDevice->setEnabled(false);
  enabled=false;
}

void SingleDeviceSettings::disableSampleGroup()
{
    ui->lbSampleGroup->setVisible(false);
    ui->leSampleGroup->setVisible(false);
}

void SingleDeviceSettings::enableSampleGroup()
{
    ui->lbSampleGroup->setVisible(true);
    ui->leSampleGroup->setVisible(true);
}


bool SingleDeviceSettings::check()
{
  if (!enabled) return true;

  QString device = getSelectedDeviceId();
  int channels = getChannels();
  int samplerate = getSampleRate();

  bool ok = SoundServer::getInstance()->check(m_type, device, channels, samplerate);

  if (!ok)
    KMessageBox::error(this, i18n("The selected sound configuration is not supported by the following device:\n%1\n\nPlease double-check your configuration and, if necessary, please contact your vendor.", device));

  return ok;
}


bool SingleDeviceSettings::isEnabled()
{
  return enabled;
}


QString SingleDeviceSettings::getSelectedDeviceId()
{
  if (!isEnabled())
    return m_deviceName;

  return ui->cbSoundDevice->currentText();
}


int SingleDeviceSettings::getSampleRate()
{
  return ui->sbSampleRate->value();
}


int SingleDeviceSettings::getChannels()
{
  return ui->sbChannels->value();
}


SimonSound::SoundDeviceType SingleDeviceSettings::getType()
{
  return m_type;
}


SimonSound::SoundDeviceUses SingleDeviceSettings::getUses()
{
  SimonSound::SoundDeviceUses uses = SimonSound::None;
  if (ui->cbRecognition->isChecked())
    uses = (SimonSound::SoundDeviceUses) (uses|SimonSound::Recognition);
  if (ui->cbTraining->isChecked())
    uses = (SimonSound::SoundDeviceUses) (uses|SimonSound::Training);

  return uses;
}

QString SingleDeviceSettings::getDefaultSampleGroup()
{
    if (!isEnabled())
      return m_defaultSampleGroup;

    return ui->leSampleGroup->text();
}


void SingleDeviceSettings::slotChanged()
{
  hasChanged = true;
  emit changed(true);
}

bool SingleDeviceSettings::getResampleEnabled()
{
  return ui->cbResample->isChecked();
}

int SingleDeviceSettings::getResampleSampleRate()
{
  return ui->sbResampleSampleRate->value();
}

SingleDeviceSettings::~SingleDeviceSettings()
{
  delete ui;
}
