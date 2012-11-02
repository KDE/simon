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

#include "deviceinformationwidget.h"
#include "ui_deviceinformation.h"
#include "sscconfig.h"

#include <sscdaccess/sscdaccess.h>
#include <sscobjects/microphone.h>
#include <sscobjects/soundcard.h>

#include <QSettings>
#include <sscdaccess/sscdaccesssingleton.h>

DeviceInformationWidget::DeviceInformationWidget(QWidget* parent) :
QWidget(parent),
ui(new Ui::DeviceInformationWidget())
{
  ui->setupUi(this);
  connect(ui->cbModel, SIGNAL(editTextChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui->cbType, SIGNAL(editTextChanged(QString)), this, SIGNAL(completeChanged()));
}


void DeviceInformationWidget::setup(const SimonSound::DeviceConfiguration& device)
{
  bool soundCardOk;
  bool microphoneOk;
  QList<SoundCard*> soundCards = SSCDAccessSingleton::getInstance()->getSoundCards(&soundCardOk);
  QList<Microphone*> microphones = SSCDAccessSingleton::getInstance()->getMicrophones(&microphoneOk);

  if (soundCardOk) {
    foreach (SoundCard* card, soundCards) {
      ui->cbModel->addItem(card->model(), card->id());
      ui->cbType->addItem(card->type(), card->id());
    }
  }
  if (microphoneOk) {
    foreach (Microphone* mic, microphones) {
      ui->cbMicModel->addItem(mic->model(), mic->id());
      ui->cbMicType->addItem(mic->type(), mic->id());
    }
  }
  qDeleteAll(soundCards);
  qDeleteAll(microphones);

  QStringList keys = SSCConfig::deviceNameKeys();
  QStringList values = SSCConfig::deviceNameValues();
  QStringList types = SSCConfig::deviceNameTypes();
  QStringList microphoneValues = SSCConfig::deviceNameMicrophoneValues();
  QStringList microphoneTypes = SSCConfig::deviceNameMicrophoneTypes();

  Q_ASSERT(keys.count() == values.count());
  Q_ASSERT(keys.count() == types.count());
  Q_ASSERT(keys.count() == microphoneValues.count());
  Q_ASSERT(keys.count() == microphoneTypes.count());

  QString deviceName = device.name();
  QString deviceType;
  QString microphoneName;
  QString microphoneType;

  for (int i=0; i < keys.count(); i++) {
    if (keys[i] == device.name()) {
      deviceName = values[i];
      deviceType = types[i];
      microphoneName = microphoneValues[i];
      microphoneType = microphoneTypes[i];
      break;
    }
  }

  m_deviceName = device.name();
  m_channels = device.channels();
  m_sampleRate = device.sampleRate();
  m_resample = device.resample();
  m_resampleSampleRate = device.resampleSampleRate();

  ui->lbDevice->setText(device.name());
  ui->cbModel->setEditText(deviceName);
  ui->cbType->setEditText(deviceType);
  ui->cbMicModel->setEditText(microphoneName);
  ui->cbMicType->setEditText(microphoneType);
}


void DeviceInformationWidget::deleteLater()
{
  QObject::deleteLater();
}


void DeviceInformationWidget::storeConfig()
{
  QStringList keys = SSCConfig::deviceNameKeys();
  QStringList values = SSCConfig::deviceNameValues();
  QStringList types = SSCConfig::deviceNameTypes();
  QStringList microphoneValues = SSCConfig::deviceNameMicrophoneValues();
  QStringList microphoneTypes = SSCConfig::deviceNameMicrophoneTypes();

  int keyIndex = keys.indexOf(m_deviceName);
  if (keyIndex == -1) {
    keyIndex = 0;
    keys.insert(0, m_deviceName);
    values.insert(0, "");
    types.insert(0, "");
    microphoneValues.insert(0, "");
    microphoneTypes.insert(0, "");
  }

  values.replace(keyIndex, ui->cbModel->currentText());
  types.replace(keyIndex, ui->cbType->currentText());
  microphoneValues.replace(keyIndex, ui->cbMicModel->currentText());
  microphoneTypes.replace(keyIndex, ui->cbMicType->currentText());

  SSCConfig::setDeviceNameKeys(keys);
  SSCConfig::setDeviceNameValues(values);
  SSCConfig::setDeviceNameTypes(types);
  SSCConfig::setDeviceNameMicrophoneValues(microphoneValues);
  SSCConfig::setDeviceNameMicrophoneTypes(microphoneTypes);
}


QString DeviceInformationWidget::getModel() const
{
  return ui->cbModel->currentText();
}


QString DeviceInformationWidget::getType() const
{
  return ui->cbType->currentText();
}


QString DeviceInformationWidget::getMicModel() const
{
  return ui->cbMicModel->currentText();

}


QString DeviceInformationWidget::getMicType() const
{
  return ui->cbMicType->currentText();
}


bool DeviceInformationWidget::isComplete() const
{
  return (!ui->cbModel->currentText().isEmpty()) && (!ui->cbType->currentText().isEmpty());
}


void DeviceInformationWidget::serialize(QSettings& ini) const
{
  ini.setValue("InternalDeviceName", m_deviceName);
  ini.setValue("DeviceName", getModel());
  ini.setValue("DeviceType", getType());
  ini.setValue("MicName", getMicModel());
  ini.setValue("MicType", getMicType());
  ini.setValue("Channels", m_channels);
  ini.setValue("SampleRate", m_sampleRate);

  ini.setValue("Resample", m_resample);
  ini.setValue("ResampleSampleRate", m_resampleSampleRate);
}


void DeviceInformationWidget::deserialize(QSettings& ini)
{
  m_deviceName = ini.value("InternalDeviceName").toString();
  ui->lbDevice->setText(m_deviceName);
  ui->cbModel->setEditText(ini.value("DeviceName").toString());
  ui->cbType->setEditText(ini.value("DeviceType").toString());
  ui->cbMicModel->setEditText(ini.value("MicName").toString());
  ui->cbMicType->setEditText(ini.value("MicType").toString());
  m_channels = ini.value("Channels").toInt();
  m_sampleRate = ini.value("SampleRate").toInt();

  m_resample = ini.value("Resample").toInt();
  m_resampleSampleRate = ini.value("ResampleSampleRate").toInt();
}

SimonSound::DeviceConfiguration DeviceInformationWidget::buildDevice()
{
  return SimonSound::DeviceConfiguration(m_deviceName, m_channels, m_sampleRate, m_resample, m_resampleSampleRate);
}

DeviceInformationWidget::~DeviceInformationWidget()
{
  delete ui;
}
