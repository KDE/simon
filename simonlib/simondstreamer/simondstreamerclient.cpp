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

#include <simoncontextdetection/compoundcondition.h>
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
  vad(new VADSoundProcessor(device)),
  streamingState(Idle),
  m_condition(0)
{
  this->id = id;
  registerSoundProcessor(vad);
  QString conditions = device.conditions();
  
  if (!conditions.isEmpty()) {
    QDomDocument doc;
    doc.setContent(conditions.toUtf8());
    m_condition = CompoundCondition::createInstance(doc.documentElement());
    if (m_condition) 
      connect(m_condition, SIGNAL(conditionChanged(bool)), this, SLOT(conditionChanged()));
  }
  if (!m_condition || m_condition->isSatisfied())
    streamingState |= ContextReady;
}


void SimondStreamerClient::inputStateChanged(SimonSound::State state)
{
  if (state == SimonSound::IdleState)
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

bool SimondStreamerClient::shouldStream ( SimondStreamerClient::StreamingState state ) const
{
  return ((state & VADReady) && (state & ContextReady));
}

void SimondStreamerClient::conditionChanged()
{
  StreamingState oldState = streamingState;
  
  if (m_condition->isSatisfied())
    streamingState |= ContextReady;
  else
    streamingState &= ~ContextReady;
  
  if (streamingState & Streaming && shouldStream(oldState) && !shouldStream(streamingState)) {
    streamingState &= ~Streaming;
    sender->recognizeSample(id);
  }
}

void SimondStreamerClient::processPrivate(const QByteArray& data, qint64 currentTime)
{
  Q_UNUSED(currentTime);

  StreamingState oldState = streamingState;
  
  if (vad->startListening())
    streamingState |= VADReady;

  sender->clientLoudness(id, ((float) vad->peak()) / ((float) vad->maxAmp()));
  
  if (!shouldStream(streamingState)) {
    if (vad->doneListening())
      streamingState &= ~VADReady;
    return;
  }
  
  streamingState |= Streaming;
  
  if (!shouldStream(oldState)) { // start sample 
    kDebug() << "Starting sample";
    sender->startSampleToRecognize(id, m_deviceConfiguration.channels(),
      m_deviceConfiguration.resample() ? m_deviceConfiguration.resampleSampleRate() : 
      m_deviceConfiguration.sampleRate());
  }
  
  sender->sendSampleToRecognize(id, data);

  if (vad->doneListening()) {
    streamingState &= ~VADReady;
    streamingState &= ~Streaming;
    kDebug() << "Stopping sample";
    sender->recognizeSample(id);
  }
}

bool SimondStreamerClient::stop()
{
  bool succ = true;
  succ = SoundServer::getInstance()->deRegisterInputClient(this);
  if (succ) {
    m_isRunning = false;
    emit stopped();
  }
  
  if (streamingState & Streaming)
    sender->recognizeSample(id); // finish up
  
  streamingState = Idle;
  if (!m_condition || m_condition->isSatisfied())
    streamingState |= ContextReady;
  return succ;
}
