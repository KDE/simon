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

#include "soundserver.h"
#include "soundconfig.h"
#include "soundinputclient.h"
#include "soundoutputclient.h"
#include "simonsoundinput.h"
#include "simonsoundoutput.h"
#include "soundbackend.h"

#include <QObject>
#include <QCoreApplication>

#include <KDebug>
#include <KLocalizedString>

SoundServer* SoundServer::instance;

/**
 * \brief Constructor
 */
SoundServer::SoundServer(QObject* parent) : QObject(parent)
{
  qRegisterMetaType<SimonSound::State>("SimonSound::State");
  qRegisterMetaType<SimonSound::Error>("SimonSound::Error");
  backend = SoundBackend::createObject();
}


SoundServer* SoundServer::getInstance()
{
  if (!instance) {
    instance = new SoundServer(0);
    connect(qApp, SIGNAL(aboutToQuit()), instance, SLOT(deleteLater()));
  }
  return instance;
}

QString SoundServer::defaultInputDevice()
{
  return SoundServer::getInstance()->defaultInputDevicePrivate();
}

QString SoundServer::defaultSampleGroup()
{
    return "default";
}


QString SoundServer::defaultOutputDevice()
{
  return SoundServer::getInstance()->defaultOutputDevicePrivate();
}
QString SoundServer::defaultInputDevicePrivate()
{
  return backend->getDefaultInputDevice();
}

QString SoundServer::defaultOutputDevicePrivate()
{
  return backend->getDefaultOutputDevice();
}



bool SoundServer::registerInputClient(SoundInputClient* client)
{
  kDebug() << "Register input client for device " << client->deviceConfiguration().name();

  bool succ = true;
  bool isNew = false;

  SimonSound::DeviceConfiguration clientRequestedSoundConfiguration = client->deviceConfiguration();
                                                  //recording not currently running
  if (!inputs.contains(client->deviceConfiguration())) {
    kDebug() << "No input for this particular configuration... Creating one";

    SimonSoundInput *soundInput = new SimonSoundInput(0);
    connect(soundInput, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(soundInput, SIGNAL(recordingFinished()), this, SLOT(slotRecordingFinished()));
    //then start recording
    succ = soundInput->prepareRecording(clientRequestedSoundConfiguration);
    if (!succ)
      //we had to adjust the format slightly and _that_ is already loaded
      soundInput->deleteLater();
    else {
      if (inputs.contains(clientRequestedSoundConfiguration))
        soundInput->deleteLater();
      else {
        inputs.insert(clientRequestedSoundConfiguration, soundInput);
        isNew = true;
      }

      if (! (client->deviceConfiguration() == clientRequestedSoundConfiguration) )
                                                  // found something supported that is very close
        client->setDeviceConfiguration(clientRequestedSoundConfiguration);
    }
  }

  if (succ) {
    SimonSoundInput *input = inputs.value(clientRequestedSoundConfiguration);
    input->registerInputClient(client);
    if (isNew)
      input->startRecording();
  }
  applyInputPriorities();

  return succ;
}


void SoundServer::slotRecordingFinished()
{
  SimonSoundInput *input = dynamic_cast<SimonSoundInput*>(sender());
  Q_ASSERT(input);

  if (input->isActive()) {
    //apparently we resumed operations :)
    return;
  }

  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundInput*> i(inputs);

  while (i.hasNext()) {
    i.next();
    if (i.value() == input)
      inputs.remove(i.key());
  }
  input->deleteLater();
  applyInputPriorities();
}


void SoundServer::slotPlaybackFinished()
{
  SimonSoundOutput *output = dynamic_cast<SimonSoundOutput*>(sender());
  Q_ASSERT(output);

  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundOutput*> i(outputs);

  while (i.hasNext()) {
    i.next();
    if (i.value() == output)
      outputs.remove(i.key());
  }
  output->deleteLater();
  applyOutputPriorities();
}


void SoundServer::applyInputPriorities()
{
  SoundClient::SoundClientPriority priority = SoundClient::Background;

  //1. find highest priority
  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundInput*> i(inputs);
  while (i.hasNext()) {
    i.next();
    priority = qMax(priority, i.value()->getHighestPriority());
  }

  kDebug() << "Highest priority: " << priority;

  i.toFront();

  bool activated;
  while (i.hasNext()) {
    i.next();
    activated = i.value()->activate(priority);

    if (activated && priority == SoundClient::Exclusive)
      //find first exclusive client, activate it and return
      return;
  }
}


void SoundServer::applyOutputPriorities()
{
  SoundClient::SoundClientPriority priority = SoundClient::Background;

  //1. find highest priority
  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundOutput*> i(outputs);
  while (i.hasNext()) {
    i.next();
    priority = qMax(priority, i.value()->getHighestPriority());
  }

  kDebug() << "Highest priority: " << priority;

  i.toFront();

  bool activated;
  while (i.hasNext()) {
    i.next();
    activated = i.value()->activate(priority);

    if (activated && priority == SoundClient::Exclusive)
      return;
  }
}


bool SoundServer::deRegisterInputClient(SoundInputClient* client)
{
  kDebug() << "Deregistering input client";

  bool success = true;

  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundInput*> i(inputs);
  while (i.hasNext()) {
    i.next();
    if (i.value()->deRegisterInputClient(client))
      success = true;
  }

  applyInputPriorities();
  return success;
}

bool SoundServer::registerOutputClient(SoundOutputClient* client)
{
  kDebug() << "Register output client";
  SimonSound::DeviceConfiguration clientRequestedSoundConfiguration = client->deviceConfiguration();

  bool succ = true;
  bool isNew = false;
  if (!outputs.contains(clientRequestedSoundConfiguration)) {
    //create output for this configuration
    kDebug() << "No output for this particular configuration... Creating one";

    SimonSoundOutput *soundOutput = new SimonSoundOutput(0);
    connect(soundOutput, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(soundOutput, SIGNAL(playbackFinished()), this, SLOT(slotPlaybackFinished()));
    //then start playback
    succ = soundOutput->preparePlayback(clientRequestedSoundConfiguration);
    if (!succ) {
      //failed
      soundOutput->deleteLater();
    }
    else {
      //we had to adjust the format slightly and _that_ is already loaded
      if (outputs.contains(clientRequestedSoundConfiguration)) {
        soundOutput->deleteLater();
      } else {
        outputs.insert(clientRequestedSoundConfiguration, soundOutput);
        isNew = true;
      }

      if (! (client->deviceConfiguration() == clientRequestedSoundConfiguration) )
                                                  // found something supported that is very close
        client->setDeviceConfiguration(clientRequestedSoundConfiguration);
    }
  }

  if (succ) {
    SimonSoundOutput *output = outputs.value(clientRequestedSoundConfiguration);
    output->registerOutputClient(client);
    if (isNew)
      output->startPlayback();
  }

  applyOutputPriorities();
  return succ;
}


bool SoundServer::deRegisterOutputClient(SoundOutputClient* client)
{
  Q_UNUSED(client);

  kDebug() << "Deregistering output client";

  bool success = true;

  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundOutput*> i(outputs);
  while (i.hasNext()) {
    i.next();
    success = (i.value()->deRegisterOutputClient(client) && success);
  }

  applyOutputPriorities();
  
  return success;
}


qint64 SoundServer::getDeviceLengthFactor(SimonSound::DeviceConfiguration device)
{
  return (device.channels() * 2 /* 16 bit */ * ((float)device.sampleRate() / 1000.0f));
}

qint64 SoundServer::byteSizeToLength(qint64 bytes, SimonSound::DeviceConfiguration device)
{
  return bytes / getDeviceLengthFactor(device);
}


qint64 SoundServer::lengthToByteSize(qint64 length, SimonSound::DeviceConfiguration device)
{
  return length * getDeviceLengthFactor(device);
}


bool SoundServer::reinitializeDevices()
{
  kDebug() << "Reinitialize devices..." << this;
  
  bool succ = true;

  emit devicesChanged();
  return succ;
}


int SoundServer::getInputDeviceCount()
{
  return SoundConfiguration::soundInputDevices().count();
}


int SoundServer::getOutputDeviceCount()
{
  return SoundConfiguration::soundOutputDevices().count();
}


bool SoundServer::getCalibrateVolume()
{
  return SoundConfiguration::calibrateVolume();
}


bool SoundServer::getDefaultToPowerTraining()
{
  return SoundConfiguration::defaultToPowerTraining();
}


int SoundServer::getLevelThreshold()
{
  return SoundConfiguration::level();
}


int SoundServer::getHeadMargin()
{
  return SoundConfiguration::headMargin();
}


int SoundServer::getTailMargin()
{
  return SoundConfiguration::tailMargin();
}


int SoundServer::getShortSampleCutoff()
{
  return SoundConfiguration::skipSamples();
}

QStringList SoundServer::getDevices(SimonSound::SoundDeviceType type)
{
  return SoundServer::getInstance()->getDevicesPrivate(type);
}

QStringList SoundServer::getDevicesPrivate(SimonSound::SoundDeviceType type)
{
  if (type == SimonSound::Input)
    return backend->getAvailableInputDevices();
  else
    return backend->getAvailableOutputDevices();
}

bool SoundServer::check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
  return backend->check(type, device, channels, samplerate);
}

QStringList SoundServer::getInputDeviceDefaultSampleGroups()
{
    return SoundConfiguration::soundInputDefaultSampleGroups();
}

QList<SimonSound::DeviceConfiguration> SoundServer::getInputDevices(SimonSound::SoundDeviceUses uses)
{
  QList<SimonSound::DeviceConfiguration> devices;

  QStringList soundInputDevices = SoundConfiguration::soundInputDevices();
  QList<int> soundInputChannels = SoundConfiguration::soundInputChannels();
  QList<int> soundInputSampleRates = SoundConfiguration::soundInputSampleRates();
  QList<int> soundInputResampleEnabled = SoundConfiguration::soundInputResampleEnabled();
  QList<int> soundInputResampleSampleRates = SoundConfiguration::soundInputResampleSampleRates();
  QList<int> soundInputUses = SoundConfiguration::soundInputUses();
  QStringList soundInputDefaultSampleGroups = SoundConfiguration::soundInputDefaultSampleGroups();

  for (int i=0; i < soundInputDevices.count(); i++) {
    if (!(soundInputUses[i] & uses))
      continue;

    devices << SimonSound::DeviceConfiguration(soundInputDevices[i], soundInputChannels[i], soundInputSampleRates[i],
        soundInputResampleEnabled[i], soundInputResampleSampleRates[i], soundInputDefaultSampleGroups[i]);
  }

  return devices;
}


QList<SimonSound::DeviceConfiguration> SoundServer::getOutputDevices(SimonSound::SoundDeviceUses uses)
{
  QList<SimonSound::DeviceConfiguration> devices;

  QStringList soundOutputDevices = SoundConfiguration::soundOutputDevices();
  QList<int> soundOutputChannels = SoundConfiguration::soundOutputChannels();
  QList<int> soundOutputSampleRates = SoundConfiguration::soundOutputSampleRates();
  QList<int> soundOutputUses = SoundConfiguration::soundOutputUses();
  QList<int> soundOutputResampleEnabled = SoundConfiguration::soundOutputResampleEnabled();

  for (int i=0; i < soundOutputDevices.count(); i++) {
    if (!(soundOutputUses[i] & uses))
      continue;

    devices << SimonSound::DeviceConfiguration(soundOutputDevices[i], soundOutputChannels[i], soundOutputSampleRates[i],
        soundOutputResampleEnabled[i], 0);
  }

  return devices;
}


QList<SimonSound::DeviceConfiguration> SoundServer::getTrainingInputDevices()
{
  return getInputDevices(SimonSound::Training);
}


QList<SimonSound::DeviceConfiguration> SoundServer::getRecognitionInputDevices()
{
  return getInputDevices(SimonSound::Recognition);
}


QList<SimonSound::DeviceConfiguration> SoundServer::getTrainingOutputDevices()
{
  return getOutputDevices(SimonSound::Training);
}

void SoundServer::uninitializeSoundSystem()
{
  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundInput*> i(inputs);
  while (i.hasNext()) {
    i.next();
    i.value()->stopRecording();
  }
  inputs.clear();

  QHashIterator<SimonSound::DeviceConfiguration, SimonSoundOutput*> j(outputs);
  while (j.hasNext()) {
    j.next();
    j.value()->stopPlayback();
  }
  outputs.clear();
}


/**
 * \brief Destructor
 */
SoundServer::~SoundServer()
{
  instance = 0;
  uninitializeSoundSystem();
  delete backend;
}
