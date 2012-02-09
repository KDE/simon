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
dying(false),
killBufferLock(QMutex::Recursive),
m_output(SoundBackend::createObject()),
m_activeOutputClient(0),
m_buffer(0)
{
  connect(m_output, SIGNAL(stateChanged(SimonSound::State)), this, SLOT(slotOutputStateChanged(SimonSound::State)));
  connect(m_output, SIGNAL(stateChanged(SimonSound::State)), this, SIGNAL(outputStateChanged(SimonSound::State)));
}


qint64 SimonSoundOutput::readData(char *toRead, qint64 maxLen)
{
  QMutexLocker m(&killBufferLock);
  if (!m_buffer) return -1;

  qint64 read = m_buffer->read(toRead, maxLen);

  clientsLock.lock();
  if (m_activeOutputClient)
    m_activeOutputClient->advanceStreamTimeByBytes(read);
  else  {
    if (m_suspendedOutputClients.isEmpty()) {
      stopPlayback();
      clientsLock.unlock();
      return -1;
    }
  }
  clientsLock.unlock();

  return read;
}

QByteArray SimonSoundOutput::requestData(qint64 maxLen)
{
  clientsLock.lock();
  if (!m_activeOutputClient) {
    kDebug() << "No current output client\n";
    clientsLock.unlock();
    return QByteArray();
  }

  char* toRead = (char*) malloc(maxLen+1);
  qint64 read = m_activeOutputClient->read(toRead, maxLen);
  clientsLock.unlock();
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


bool SimonSoundOutput::registerOutputClient(SoundOutputClient* client)
{
  kDebug() << "Registering output client";
  QMutexLocker l(&clientsLock);
  if (dying) return false;
  
  bool newOut = false;
  if (m_activeOutputClient != 0)
    m_suspendedOutputClients.insert(0,m_activeOutputClient);
  else
    //can only happen if this is a new output
    newOut = true;

  m_activeOutputClient = client;
  
  if (newOut) startPlayback();
  return true;
}


bool SimonSoundOutput::deRegisterOutputClient(SoundOutputClient* client)
{
  kWarning() << "Deregister output client";
  clientsLock.lock();
  if (client != m_activeOutputClient)
    //wasn't active anyways
    m_suspendedOutputClients.removeAll(client);
  else {
    m_activeOutputClient = 0;
  }
  clientsLock.unlock();

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
  QMutexLocker m(&killBufferLock);
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
  
  clientsLock.lock();
  if (m_activeOutputClient)
    priority = m_activeOutputClient->priority();

  foreach (SoundOutputClient* client, m_suspendedOutputClients)
    priority = qMax(priority, client->priority());
  clientsLock.unlock();

  return priority;
}

bool SimonSoundOutput::activate(SoundClient::SoundClientPriority priority)
{
  kDebug() << "Activating priority: " << priority;

  QMutexLocker l(&clientsLock);
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

  clientsLock.lock();
  if (m_activeOutputClient)
    m_activeOutputClient->outputStateChanged(state);
  clientsLock.unlock();

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

void SimonSoundOutput::killBuffer()
{
  QMutexLocker m(&killBufferLock);
  SoundOutputBuffer *old = m_buffer;
  
  m_buffer = 0;
  if (old) {
    kWarning() << "Stopping buffer...";
    old->stop();
    kWarning() << "Stopping buffer: Done.";
  }
}

//clientsLock is locked when entering this function
bool SimonSoundOutput::stopPlayback()
{
  kWarning() << "Stop playback: Killing buffer...";
  killBuffer();
  kWarning() << "Done killing buffer...";

  if (!haveSomethingToPlay()) {
    dying = true;
    emit playbackFinished();
    
    SoundServer::getInstance()->closeOutput(this);
  }
  kDebug() << "After playbackFinished";
  return true;
}

SimonSoundOutput::~SimonSoundOutput()
{
  kDebug() << "Deleting simon sound output";
  m_output->stopPlayback();
  m_output->deleteLater();

  killBuffer();
  kDebug() << "Deleted buffer";
}