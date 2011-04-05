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

#include "simondstreamerclient.h"

#include <simonsound/soundserver.h>
#include <simonsound/vadsoundprocessor.h>
#include <simonwav/wav.h>

#include <QObject>

#include <KDebug>


/**
 * \brief Constructor
 */
SimondStreamerClient::SimondStreamerClient(qint8 id, SimonSender *s, SimonSound::DeviceConfiguration device, QObject *parent) :
QObject(parent),
SoundInputClient(device, SoundClient::Background),
m_isRunning(false),
sender(s),
vad(new VADSoundProcessor(device))
{
  this->id = id;
  registerSoundProcessor(vad);
}


void SimondStreamerClient::inputStateChanged(SimonSound::State state)
{
  if (state == SimonSound::StoppedState)
    stop();
}


bool SimondStreamerClient::isRunning()
{
  return m_isRunning;
}


bool SimondStreamerClient::start()
{
  vad->reset();
  bool succ =  SoundServer::getInstance()->registerInputClient(this);

  kDebug() << "Registered input client: " << succ;
  if (succ) {
    m_isRunning = true;
    emit started();
  }
  else {
    m_isRunning = false;
    emit stopped();
  }

  return succ;
}


void SimondStreamerClient::processPrivate(const QByteArray& data, qint64 currentTime)
{
  Q_UNUSED(currentTime);

  if (vad->startListening()) {
//     kDebug() << "Starting listening!";
    sender->startSampleToRecognize(id, m_deviceConfiguration.channels(),
      m_deviceConfiguration.resample() ? m_deviceConfiguration.resampleSampleRate() : 
      m_deviceConfiguration.sampleRate());
  }

  sender->clientLoudness(id, ((float) vad->peak()) / ((float) vad->maxAmp()));
  sender->sendSampleToRecognize(id, data);

  if (vad->doneListening()) {
//     kDebug() << "Stopping listening!";
    sender->recognizeSample(id);
  }
}


/**
 * \brief This will stop the current recording
 *
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool SimondStreamerClient::stop()
{
  bool succ = true;
  succ = SoundServer::getInstance()->deRegisterInputClient(this);
  if (succ) {
    m_isRunning = false;
    emit stopped();
  }
  return succ;
}
