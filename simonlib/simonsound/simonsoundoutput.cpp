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
#include <simonsound/soundbackend.h>
#include <QTimer>
#include <QMutexLocker>
#include <KLocalizedString>
#include <KDebug>
#include <KMessageBox>
#include <QFile>

SimonSoundOutput::SimonSoundOutput(QObject *parent) : QObject(parent),
popWhenBufferEmpty(false),
m_output(SoundBackend::createObject()),
m_activeOutputClient(0),
m_buffer(new SoundOutputBuffer(this))
{
  connect(m_output, SIGNAL(stateChanged(SimonSound::State)), this, SLOT(slotOutputStateChanged(SimonSound::State)));
  connect(m_output, SIGNAL(stateChanged(SimonSound::State)), this, SIGNAL(outputStateChanged(SimonSound::State)));
}

/**
 * \brief Read data from buffer to argument (called by backend)
 */
qint64 SimonSoundOutput::readData(char *toRead, qint64 maxLen)
{
  qint64 read = m_buffer->read(toRead, maxLen);

  if (m_activeOutputClient)
    m_activeOutputClient->advanceStreamTimeByBytes(read);
  else  {
    if (m_suspendedOutputClients.isEmpty()) {
      stopPlayback();
      return -1;
    }
  }

  return read;
}

/**
 * Read data from output client to buffer
 */
QByteArray SimonSoundOutput::requestData(qint64 maxLen)
{
  if (!m_activeOutputClient) {
    kDebug() << "No current output client\n";
    return QByteArray();
  }

  char* toRead = (char*) malloc(maxLen+1);
  qint64 read = m_activeOutputClient->read(toRead, maxLen);
  if (read <= 0) {
    popWhenBufferEmpty = true;
  } else popWhenBufferEmpty = false;
  
  QByteArray output = QByteArray().append(toRead, read);
  free(toRead);
  return output;
}

void SimonSoundOutput::popClient()
{
  if (!popWhenBufferEmpty || !m_activeOutputClient) return;

  popWhenBufferEmpty = false;
  SoundServer::getInstance()->deRegisterOutputClient(m_activeOutputClient);
}

int SimonSoundOutput::bufferSize()
{
  return m_output->bufferSize();
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


bool SimonSoundOutput::registerOutputClient(SoundOutputClient* client)
{
  kDebug() << "Registering output client";
  
  bool newOut = false;
  
  if (m_activeOutputClient != 0)
    m_suspendedOutputClients.insert(0,m_activeOutputClient);
  else
    //can only happen if this is a new output
    newOut = true;

  m_activeOutputClient = client;
  
  if (newOut) //start playback
    return m_output->startPlayback(this);
  
  return true;
}


bool SimonSoundOutput::deRegisterOutputClient(SoundOutputClient* client)
{
  kWarning() << "Deregister output client";
  
  if (client != m_activeOutputClient)
    //wasn't active anyways
    m_suspendedOutputClients.removeAll(client);
  else {
    m_activeOutputClient = 0;
  }
  if (!haveSomethingToPlay())
    stopPlayback();

  client->finish();
  return true;
}

bool SimonSoundOutput::preparePlayback(SimonSound::DeviceConfiguration& device)
{
  kDebug() << "Starting playback...";
  kDebug() << "Using device: " << device.name();
  kDebug() << "Channels: " << device.channels();
  kDebug() << "Samlerate: " << device.sampleRate();

  int channels = device.channels();
  int sampleRate = device.sampleRate();
  if (!m_output->preparePlayback(device.name(), channels, sampleRate)) {
    kWarning() << "Failed to setup recording...";
    return false;
  }

  kDebug() << "Prepared audio output";
  m_device = device;

  m_buffer->start();

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

void SimonSoundOutput::slotOutputStateChanged(SimonSound::State state)
{
  kDebug() << "Output state changed: " << state;

  if (m_activeOutputClient)
    m_activeOutputClient->outputStateChanged(state);

  if (state == SimonSound::IdleState) {
    kDebug() << "Error: " << m_output->error();
    switch (m_output->error()) {
      case SimonSound::NoError:
        kDebug() << "Output stopped without error";
        break;
      case SimonSound::OpenError:
        emit error(i18n("Failed to open the audio device.\n\nPlease check your sound configuration."));
        break;

      case SimonSound::IOError:
        if (m_activeOutputClient)
          emit error(i18n("An error occurred while writing data to the audio device."));
        break;

      case SimonSound::UnderrunError:
        kWarning() << i18n("Buffer underrun when processing the sound data.");
        break;

      case SimonSound::FatalError:
        emit error(i18n("A fatal error occurred during playback."));
        break;
      case SimonSound::BackendBusy:
        emit error(i18n("The backend is already playing."));
        break;
    }
  }
}

bool SimonSoundOutput::haveSomethingToPlay()
{
  return ((m_activeOutputClient != 0) || !m_suspendedOutputClients.isEmpty());
}

bool SimonSoundOutput::stopPlayback()
{
  if (!haveSomethingToPlay()) {
    emit playbackFinished();
    
    SoundServer::getInstance()->closeOutput(this);
    if (!haveSomethingToPlay()) {
      //we got something while closing the output. This is properly locked in the sound server
      //so we'll be in that list by now. Just don't die...
      deleteLater();
    }
  }
  kDebug() << "After playbackFinished";
  return true;
}

SimonSoundOutput::~SimonSoundOutput()
{
  kDebug() << "Deleting Simon sound output";
  m_output->stopPlayback();
  m_output->deleteLater();

  m_buffer->stop();
  m_buffer->wait();
  kDebug() << "Stopped buffer";
}

