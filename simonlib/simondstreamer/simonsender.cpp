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

#include "simonsender.h"
#include "simonsenderadaptor.h"
#include <QDBusConnection>
#include <QDebug>

SimonSender::SimonSender() : m_state(SimonSender::Idle)
{
  new SimonSenderAdaptor(this);
  QDBusConnection dbus = QDBusConnection::sessionBus();
  dbus.registerObject("/SimonSender", this);
  dbus.registerService("org.simon-listens.SimonSender");

  connect(this, SIGNAL(processing()), this, SLOT(slotProcessing()));
  connect(this, SIGNAL(listening()), this, SLOT(slotListening()));
  connect(this, SIGNAL(receivedResults()), this, SLOT(slotReceivedResults()));
  connect(&loudnessTimer, SIGNAL(timeout()), this, SLOT(relayLoudness()));
  
  loudnessTimer.start(500);
}

void SimonSender::relayLoudness()
{
//   qDebug() << "Loudness: " << m_currentPeak;
  emit recordingLevel(m_currentPeak);
  m_currentPeak = 0;
}
void SimonSender::clientLoudness(qint8 id, float currentPeak)
{
  Q_UNUSED(id);
  m_currentPeak = qMax(m_currentPeak, currentPeak);
}

//use slots to also handle signals from subclasses
void SimonSender::slotListening()
{
  m_state = SimonSender::Listening;
}
void SimonSender::slotProcessing()
{
  m_state = SimonSender::Processing;
}
void SimonSender::slotReceivedResults()
{
  m_state = SimonSender::Idle;
}

void SimonSender::recognizeSample(qint8 id)
{
  emit processing();
  recognizeSamplePrivate(id);
}

void SimonSender::sendSampleToRecognize(qint8 id, const QByteArray& data)
{
  sendSampleToRecognizePrivate(id, data);

}
void SimonSender::startSampleToRecognize(qint8 id, qint8 channels, qint32 sampleRate)
{
  emit listening();
  startSampleToRecognizePrivate(id, channels, sampleRate);
}
