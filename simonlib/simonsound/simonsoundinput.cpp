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

#include "simonsoundinput.h"
#include <simonsound/soundinputclient.h>
#include <simonsound/soundserver.h>
#include <simonsound/soundbackend.h>

#include <KDebug>
#include <KLocalizedString>
#include "soundinputbuffer.h"

SimonSoundInput::SimonSoundInput(QObject *parent) : QObject(parent),
m_input(SoundBackend::createObject()), m_buffer(0)
{
  connect(m_input, SIGNAL(stateChanged(SimonSound::State)), this, SLOT(slotInputStateChanged(SimonSound::State)));
  connect(m_input, SIGNAL(stateChanged(SimonSound::State)), this, SIGNAL(inputStateChanged(SimonSound::State)));
}


int SimonSoundInput::bufferSize()
{
  return m_input->bufferSize();
}

qint64 SimonSoundInput::writeData(const char *toWrite, qint64 len)
{
  m_buffer->write(toWrite, len);
  return len;
}

void SimonSoundInput::processData(const QByteArray& data)
{
  //length is in ms
  qint64 length = SoundServer::getInstance()->byteSizeToLength(data.count(), m_device);

  //pass data on to all registered, active clients
  QList<SoundInputClient*> activeInputClientsKeys = m_activeInputClients.keys();
  foreach (SoundInputClient *c, activeInputClientsKeys) {
    qint64 streamTime = m_activeInputClients.value(c)+length;
    c->process(data, streamTime);
    //update time stamp
    m_activeInputClients.insert(c, streamTime);
  }
}


bool SimonSoundInput::prepareRecording(SimonSound::DeviceConfiguration& device)
{
  kDebug() << "Starting recording";
  int channels = device.channels();
  int sampleRate = device.sampleRate();
  if (!m_input->prepareRecording(device.name(), channels, sampleRate)) {
    kWarning() << "Failed to setup recording...";
    return false;
  }
  //TODO: move channels / samplerate to member

  killBuffer();

  m_device = device;
  return true;
}

bool SimonSoundInput::startRecording()
{
  killBuffer();
  m_buffer = new SoundInputBuffer(this);

  m_input->startRecording(this);
  kDebug() << "Started audio input";
  return true;
}


void SimonSoundInput::suspendInputClients()
{
  QHashIterator<SoundInputClient*, qint64> j(m_activeInputClients);
  while (j.hasNext()) {
    j.next();
    suspend(j.key());
  }
}


void SimonSoundInput::suspend(SoundInputClient* client)
{
  client->suspend();
  m_suspendedInputClients.insert(client, m_activeInputClients.value(client));
  m_activeInputClients.remove(client);
}


void SimonSoundInput::resume(SoundInputClient* client)
{
  m_activeInputClients.insert(client, m_suspendedInputClients.value(client));
  m_suspendedInputClients.remove(client);
  client->resume();
}


void SimonSoundInput::registerInputClient(SoundInputClient* client)
{
  m_activeInputClients.insert(client, 0);
}


bool SimonSoundInput::deRegisterInputClient(SoundInputClient* client)
{
  kDebug() << "Deregistering input client";

  if (m_activeInputClients.remove(client) == 0) {
    //wasn't active anyways
    /*return */
    m_suspendedInputClients.remove(client);
  }

  bool success = true;

                                                  //do not need to record any longer
  if (m_activeInputClients.isEmpty() && m_suspendedInputClients.isEmpty()) {
    //then stop recording
    kDebug() << "No active clients available... Stopping recording";
    success = stopRecording();
    if (success)
      emit recordingFinished();                     // destroy this sound input
  }

  return success;
}


SoundClient::SoundClientPriority SimonSoundInput::getHighestPriority()
{
  SoundClient::SoundClientPriority priority = SoundClient::Background;
  QHashIterator<SoundInputClient*, qint64> j(m_activeInputClients);
  while (j.hasNext()) {
    j.next();
    priority = qMax(priority, j.key()->priority());
  }
  QHashIterator<SoundInputClient*, qint64> i(m_suspendedInputClients);
  while (i.hasNext()) {
    i.next();
    priority = qMax(priority, i.key()->priority());
  }
  return priority;
}


bool SimonSoundInput::activate(SoundClient::SoundClientPriority priority)
{
  kDebug() << "Activating priority: " << priority;
  bool activated = false;
  QHashIterator<SoundInputClient*, qint64> j(m_activeInputClients);
  while (j.hasNext()) {
    j.next();
    if (j.key()->priority() == priority) {
      if (priority == SoundClient::Exclusive) {
        if (activated)
          suspend(j.key());
        else activated = true;
      }
    }
    if (j.key()->priority() < priority) {
      kDebug() << "Suspending key...";
      suspend(j.key());
    }
  }

  if (activated && priority == SoundClient::Exclusive)
    return true;                                  // never more than one exclusive client

  QHashIterator<SoundInputClient*, qint64> i(m_suspendedInputClients);
  while (i.hasNext()) {
    i.next();
    if (priority == i.key()->priority()) {
      resume(i.key());
      activated = true;
      if (i.key()->priority() == SoundClient::Exclusive)
        return true;
    }

  }
  return activated;
}


void SimonSoundInput::slotInputStateChanged(SimonSound::State state)
{
  kDebug() << "Input state changed: " << state;

  QList<SoundInputClient*> activeInputClientsKeys = m_activeInputClients.keys();
  foreach (SoundInputClient *c, activeInputClientsKeys)
    c->inputStateChanged(state);

  if (state == SimonSound::IdleState) {
    switch (m_input->error()) {
      case SimonSound::NoError:
        kDebug() << "Input stopped without error";
        break;
      case SimonSound::OpenError:
        emit error(i18n("Failed to open the input audio device.\n\nPlease check your sound configuration."));
        break;

      case SimonSound::IOError:
        emit error(i18n("An error occurred while reading data from the audio device."));
        break;

      case SimonSound::UnderrunError:
        emit error(i18n("Buffer underrun when processing the sound data."));
        break;

      case SimonSound::FatalError:
        emit error(i18n("A fatal error occurred during recording."));
        break;

      case SimonSound::BackendBusy:
        emit error(i18n("The backend is already recording."));
        break;
    }
  }
}


bool SimonSoundInput::stopRecording()
{
  kDebug() << "Stopping recording";
  if (!m_input || (m_input->state() == SimonSound::IdleState))
    return true;

  m_input->stopRecording();
  
  kDebug() << "Now stopping buffer";
  killBuffer();
  kDebug() << "Done";
  
  return true;
}

void SimonSoundInput::killBuffer()
{
  if (m_buffer) {
    m_buffer->stop();
    m_buffer->wait();
  }
  
  m_buffer = 0;
}

SimonSoundInput::~SimonSoundInput()
{
  if (m_input->state() != SimonSound::IdleState)
    stopRecording();

  killBuffer();
  delete m_input;
}
