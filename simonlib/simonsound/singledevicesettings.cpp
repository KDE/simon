/* *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include <simoncontextdetection/compoundcondition.h>

#include <QIcon>

#include <KLocalizedString>
#include <QDebug>
#include <KMessageBox>
#include "ui_singledeviceconfiguration.h"

SingleDeviceSettings::SingleDeviceSettings(SimonSound::SoundDeviceType type, QString deviceName, int channels,
                                            int sampleRate, bool resampleEnabled, int resampleSampleRate, 
                                            SimonSound::SoundDeviceUses selectedUses, SimonSound::SoundDeviceUses availableUses,
                                            QString defaultSampleGroup, const QString& conditions,
                                            SimonSound::SoundDeviceOptions options, QWidget* parent) : 
  QWidget(parent),
  enabled(true),
  hasChanged(true),
  m_type(type),
  m_deviceName(deviceName),
  m_uses(selectedUses),
  m_options(options),
  m_defaultSampleGroup(defaultSampleGroup),
  m_conditions(0)
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

  ui->pbTest->setIcon(QIcon::fromTheme("help-hint"));
  ui->pbRemove->setIcon(QIcon::fromTheme("list-remove"));
  ui->pbAdvancedConfiguration->setIcon(QIcon::fromTheme("configure"));

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

  load(deviceName, channels, sampleRate, resampleEnabled, resampleSampleRate, defaultSampleGroup, conditions);

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
    KMessageBox::information(this, i18n("The sound configuration has been tested successfully."));
}


void SingleDeviceSettings::refreshDevices()
{
  if (!getSelectedDeviceId().isEmpty())
    m_deviceName = getSelectedDeviceId();

  if (!getDefaultSampleGroup().isEmpty())
      m_defaultSampleGroup = getDefaultSampleGroup();

  load(m_deviceName, getChannels(), getSampleRate(), getResampleEnabled(), getResampleSampleRate(), m_defaultSampleGroup, getConditions());
}


void SingleDeviceSettings::load(const QString& deviceName, int channels,
                                int sampleRate, bool resampleEnabled, int resampleSampleRate,
                                const QString& defaultSampleGroup, const QString& conditions)
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
  
  //instantiate conditions
  ui->wgRecognitionConditions->setConditions(0);
  if (!conditions.isEmpty()) {
    QDomDocument doc;
    doc.setContent(conditions.toUtf8());
    CompoundCondition *temp = m_conditions;
    m_conditions = CompoundCondition::createInstance(doc.documentElement());
    temp->deleteLater();
  } else m_conditions = new CompoundCondition;
  
  ui->wgRecognitionConditions->setConditions(m_conditions);
  
  if (m_conditions)
    connect(m_conditions, SIGNAL(modified()), this, SLOT(slotChanged()), Qt::UniqueConnection);

  hasChanged=false;
  if ((!deviceName.isEmpty()) &&
  (ui->cbSoundDevice->currentText() != deviceName)) {
    if (KMessageBox::questionYesNo(this, i18nc("%1 is the device name",
            "<html><head/><body><p>Simon noticed that the sound device "
            "\"%1\" is no longer available.</p><p>This is perfectly normal if you are "
            "connected to Simond or are otherwise using an application that blocks the "
            "device.</p><p><b>Did you plug / unplug a device or otherwise change your systems "
            "audio setup?</b></p><p>Selecting \"Yes\" will allow you to change your sound "
            "configuration, essentially deleting your previous configuration. Selecting "
            "\"No\" will temporarily deactivate the sound configuration in order to "
            "protect your previous configuration from being overwritten.</body></html>", deviceName))) {
      QString defaultDevice;

      if (m_type == SimonSound::Input)
        defaultDevice = SoundServer::defaultInputDevice();
      else
        defaultDevice = SoundServer::defaultOutputDevice();

      ui->cbSoundDevice->setCurrentIndex(
        ui->cbSoundDevice->findText(defaultDevice));
      hasChanged=true;
      KMessageBox::information(this, i18n("Please adjust your sound configuration accordingly."));
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


bool SingleDeviceSettings::isEnabled() const
{
  return enabled;
}


QString SingleDeviceSettings::getSelectedDeviceId() const
{
  if (!isEnabled())
    return m_deviceName;

  return ui->cbSoundDevice->currentText();
}


int SingleDeviceSettings::getSampleRate() const
{
  return ui->sbSampleRate->value();
}


int SingleDeviceSettings::getChannels() const
{
  return ui->sbChannels->value();
}


SimonSound::SoundDeviceType SingleDeviceSettings::getType() const
{
  return m_type;
}


SimonSound::SoundDeviceUses SingleDeviceSettings::getUses() const
{
  SimonSound::SoundDeviceUses uses = SimonSound::None;
  if (ui->cbRecognition->isChecked())
    uses = (SimonSound::SoundDeviceUses) (uses|SimonSound::Recognition);
  if (ui->cbTraining->isChecked())
    uses = (SimonSound::SoundDeviceUses) (uses|SimonSound::Training);

  return uses;
}

QString SingleDeviceSettings::getDefaultSampleGroup() const
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

bool SingleDeviceSettings::getResampleEnabled() const
{
  return ui->cbResample->isChecked();
}

int SingleDeviceSettings::getResampleSampleRate() const
{
  return ui->sbResampleSampleRate->value();
}

QString SingleDeviceSettings::getConditions() const
{
  if (!m_conditions) return QString();
  
  QDomDocument doc;
  QDomElement elem = m_conditions->serialize(&doc);
  doc.appendChild(elem);
  return doc.toString();
}

SingleDeviceSettings::~SingleDeviceSettings()
{
  delete ui;
}
