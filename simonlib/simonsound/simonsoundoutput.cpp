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

#include "simonsoundoutput.h"
#include "soundoutputbuffer.h"

#include <unistd.h>
#include <simonsound/soundoutputclient.h>
#include <simonsound/soundserver.h>
#include <QAudioOutput>
#include <QTimer>
#include <QMutexLocker>
#include <KLocalizedString>
#include <KDebug>
#include <KMessageBox>
#include <QFile>

SimonSoundOutput::SimonSoundOutput(QObject *parent) : QIODevice(parent),
// initialRound(true),
m_output(0),
m_activeOutputClient(0),
m_buffer(0)
{
  open(QIODevice::ReadWrite);
}


qint64 SimonSoundOutput::readData(char *toRead, qint64 maxLen)
{
  kDebug() << "readData()";
  if (!m_buffer) return -1;

  qint64 read = m_buffer->read(toRead, maxLen);

  if (!m_activeOutputClient && m_suspendedOutputClients.isEmpty()) {
    stopPlayback();
    return -1;
  }

  return read;
}

QByteArray SimonSoundOutput::requestData(qint64 maxLen)
{
  kDebug() << "requestData()";

  if (!m_activeOutputClient) {
    kDebug() << "No current output client\n";

    return QByteArray();
  }

  char* toRead = (char*) malloc(maxLen);
  qint64 read = m_activeOutputClient->read(toRead, maxLen);
  QByteArray output = QByteArray().append(toRead, read);
  free(toRead);
  return output;
}

void SimonSoundOutput::popClient()
{
  if (!m_activeOutputClient) return;

  SoundServer::getInstance()->deRegisterOutputClient(m_activeOutputClient);
}

int SimonSoundOutput::bufferSize()
{
  int bufferSize = (m_output) ? m_output->bufferSize() : QAudioOutput().bufferSize();
  return qMax(bufferSize, 2048);
}

qint64 SimonSoundOutput::bufferTime()
{
  return SoundServer::getInstance()->byteSizeToLength(bufferSize(), m_device);
}

qint64 SimonSoundOutput::writeData(const char *toWrite, qint64 len)
{
  Q_UNUSED(toWrite);
  Q_UNUSED(len);
  return 0;
}


void SimonSoundOutput::registerOutputClient(SoundOutputClient* client)
{
  kDebug() << "Registering output client";
  if (m_activeOutputClient != 0)
    m_suspendedOutputClients.insert(0,m_activeOutputClient);

  m_activeOutputClient = client;
}


bool SimonSoundOutput::deRegisterOutputClient(SoundOutputClient* client)
{
  kDebug() << "Deregister output client";
  if (client != m_activeOutputClient)
    //wasn't active anyways
    m_suspendedOutputClients.removeAll(client);
  else {
    m_activeOutputClient = 0;
  }

  client->finish();

  return true;
}

bool SimonSoundOutput::preparePlayback(SimonSound::DeviceConfiguration& device)
{
  kDebug() << "Starting playback...";
  kDebug() << "Using device: " << device.name();

  kDebug() << "Channels: " << device.channels();
  kDebug() << "Samlerate: " << device.sampleRate();
  QAudioFormat format;
  format.setFrequency(device.sampleRate());
  format.setChannels(device.channels());
  format.setSampleSize(16);                       // 16 bit
  format.setSampleType(QAudioFormat::SignedInt);  // SignedInt currently
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setCodec("audio/pcm");

  bool deviceFound = false;
  QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultOutputDevice();
  kDebug() << "Looking for device: " << device.name();
  foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
    if (deviceInfo.deviceName() == device.name()) {
      selectedInfo = deviceInfo;
      deviceFound = true;
    }
  }
  if (!deviceFound) {
    emit error(i18n("The selected sound output device \"%1\" is not available.", device.name()));
    return false;
  }

  if (!selectedInfo.isFormatSupported(format)) {
    kDebug() << "Format not supported; Trying something similar";
    format = selectedInfo.nearestFormat(format);
  }

  if(format.sampleSize() != 16) {
    kDebug() << "Sample size is not 16 bit. Aborting.";
    emit error(i18n("Sample size not equal to 16 bit."));
    emit outputStateChanged(QAudio::StoppedState);
    return false;
  }

  device.setChannels(format.channels());
  device.setSampleRate(format.frequency());

  reset();

//   initialRound = true;
  
  kDebug() << "Using device: " << selectedInfo.deviceName();
  m_output = new QAudioOutput(selectedInfo, format, this);
  connect(m_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(slotOutputStateChanged(QAudio::State)));
  connect(m_output, SIGNAL(stateChanged(QAudio::State)), this, SIGNAL(outputStateChanged(QAudio::State)));
  m_output->setBufferSize(8192);

  kDebug() << "Started audio output";
  m_device = device;

  killBuffer();

  return true;
}

bool SimonSoundOutput::startPlayback()
{
  killBuffer();
  m_buffer = new SoundOutputBuffer(this);

  m_output->start(this);
  return true;
}


SoundClient::SoundClientPriority SimonSoundOutput::getHighestPriority()
{
  SoundClient::SoundClientPriority priority = SoundClient::Background;
  if (m_activeOutputClient)
    priority = m_activeOutputClient->priority();

  foreach (SoundOutputClient* client, m_suspendedOutputClients)
    priority = qMax(priority, client->priority());

  return priority;
}

bool SimonSoundOutput::activate(SoundClient::SoundClientPriority priority)
{
  kDebug() << "Activating priority: " << priority;

  if (m_activeOutputClient &&
    (m_activeOutputClient->priority() == priority))
    return true;

  bool activated = false;
  foreach (SoundOutputClient* client, m_suspendedOutputClients) {
    if (priority == client->priority()) {
      m_activeOutputClient = client;
      m_suspendedOutputClients.removeAll(client);
      return true;
    }

  }
  return activated;
}


void SimonSoundOutput::slotOutputStateChanged(QAudio::State state)
{
  kDebug() << "Output state changed: " << state;

  if (m_activeOutputClient)
    m_activeOutputClient->outputStateChanged(state);

  if (state == QAudio::StoppedState) {
    kDebug() << "Error: " << m_output->error();
    switch (m_output->error()) {
      case QAudio::NoError:
        kDebug() << "Output stopped without error";
        break;
      case QAudio::OpenError:
        emit error(i18n("Failed to open the audio device.\n\nPlease check your sound configuration."));
        break;

      case QAudio::IOError:
        if (m_activeOutputClient)
          emit error(i18n("An error occurred while writing data to the audio device."));

        if (!m_activeOutputClient && m_suspendedOutputClients.isEmpty())
          QTimer::singleShot(5, this, SLOT(stopPlayback()));
        break;

      case QAudio::UnderrunError:
        kWarning() << i18n("Buffer underrun when processing the sound data.");
        break;

      case QAudio::FatalError:
	//not much the user can do about that and with the current QtMultimedia that happens fairly regularly..
//         emit error(i18n("A fatal error occurred during playback.\n\nThe system will try to automatically recover."));
// 	initialRound = true;
        m_output->start(this);
        break;
    }
  }
}


SimonSoundOutput::~SimonSoundOutput()
{
  kDebug() << "Deleting simon sound output";
  if (m_output)
    m_output->deleteLater();

  killBuffer();
  kDebug() << "Deleted buffer";
}


void SimonSoundOutput::suspendOutput()
{
  if (!m_output) return;
  m_output->suspend();
}


void SimonSoundOutput::resumeOutput()
{
  if (!m_output) return;
  m_output->resume();
}

void SimonSoundOutput::killBuffer()
{
  kDebug() << "Killing buffer";
  if (m_buffer) {
    kDebug() << "Stopping buffer";
    m_buffer->stop();
    kDebug() << "Waiting on buffer";
    m_buffer->wait();
  }
  
  m_buffer = 0;
}


bool SimonSoundOutput::stopPlayback()
{
  kDebug() << "Stop playback...";
  if (!m_output) return true;

  m_output->reset();
  m_output->stop();
  m_output->disconnect(this);
  m_output->deleteLater();
  m_output = 0;

  killBuffer();

  reset();

  emit playbackFinished();
  return true;
}
