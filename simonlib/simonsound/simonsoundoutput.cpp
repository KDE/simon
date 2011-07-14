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
m_output(SoundBackend::createObject()),
m_activeOutputClient(0),
m_buffer(0)
{
  connect(m_output, SIGNAL(stateChanged(SimonSound::State)), this, SLOT(slotInputStateChanged(SimonSound::State)));
  connect(m_output, SIGNAL(stateChanged(SimonSound::State)), this, SIGNAL(outputStateChanged(SimonSound::State)));
}


qint64 SimonSoundOutput::readData(char *toRead, qint64 maxLen)
{
  if (!m_buffer) return -1;

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

QByteArray SimonSoundOutput::requestData(qint64 maxLen)
{
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


void SimonSoundOutput::registerOutputClient(SoundOutputClient* client)
{
  kDebug() << "Registering output client";
  if (m_activeOutputClient != 0)
    m_suspendedOutputClients.insert(0,m_activeOutputClient);

  m_activeOutputClient = client;
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

  killBuffer();

  return true;
}

bool SimonSoundOutput::startPlayback()
{
  killBuffer();
  m_buffer = new SoundOutputBuffer(this);
  connect(m_buffer, SIGNAL(started()), this, SLOT(startSoundPlayback()));

  // make sure we don't lose samples on slow maschines
  m_buffer->start(QThread::HighestPriority);

  return true;
}

void SimonSoundOutput::startSoundPlayback()
{
  m_output->startPlayback(this);
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

        if (!m_activeOutputClient && m_suspendedOutputClients.isEmpty())
          QTimer::singleShot(5, this, SLOT(stopPlayback()));
        break;

      case SimonSound::UnderrunError:
        kWarning() << i18n("Buffer underrun when processing the sound data.");
        break;

      case SimonSound::FatalError:
        //this should never happen with the new implementation, if it does, look below
        emit error(i18n("A fatal error occurred during playback.\n\nThe system will try to automatically recover."));

        //not much the user can do about that and with the current QtMultimedia that happens fairly regularly..
        //m_output->start(this);
        break;
      case SimonSound::BackendBusy:
        emit error(i18n("The backend is already playing."));
        break;
    }
  }
}


SimonSoundOutput::~SimonSoundOutput()
{
  kDebug() << "Deleting simon sound output";
  m_output->deleteLater();

  killBuffer();
  kDebug() << "Deleted buffer";
}


void SimonSoundOutput::killBuffer()
{
  if (m_buffer) {
	  kWarning() << "Stopping buffer...";
    m_buffer->stop();
	  kWarning() << "Stopping buffer: Done.";
    m_buffer->wait();
  }
  
  m_buffer = 0;
}


bool SimonSoundOutput::stopPlayback()
{
  kWarning() << "Stop playback...";

  kWarning() << "Killing buffer...";
  killBuffer();
  kWarning() << "Done killing buffer...";

  emit playbackFinished();
  return true;
}
