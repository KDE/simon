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

#include "vadsoundprocessor.h"
#include "soundserver.h"
#include <QByteArray>
#include <QtGlobal>
#include <KDebug>

/**
 * \brief Constructor
 */
VADSoundProcessor::VADSoundProcessor(SimonSound::DeviceConfiguration deviceConfiguration, bool passAll) :
LoudnessMeterSoundProcessor(),
m_deviceConfiguration(deviceConfiguration),
m_passAll(passAll),
lastLevel(0),
lastTimeUnderLevel(0),
lastTimeOverLevel(0),
sampleStartTime(0),
waitingForSampleToInit(true),
waitingForSampleToStart(true),
waitingForSampleToFinish(false),
currentlyRecordingSample(false),
m_startListening(false),
m_doneListening(false)
{
}


void VADSoundProcessor::process(QByteArray& data, qint64& currentTime)
{
  qint64 thisTime = SoundServer::getInstance()->byteSizeToLength(data.count(), m_deviceConfiguration);
  LoudnessMeterSoundProcessor::process(data, currentTime);

  int levelThreshold = SoundServer::getLevelThreshold();
  int headMargin = SoundServer::getHeadMargin();
  int tailMargin = SoundServer::getTailMargin();
  int shortSampleCutoff = SoundServer::getShortSampleCutoff();

  bool passDataThrough = false;

  m_startListening = false;
  m_doneListening = false;

  if (peak() > levelThreshold) {
    if (lastLevel > levelThreshold) {
      #ifdef SIMOND_DEBUG
      qDebug() << "Still above level - now for : " << currentTime - lastTimeUnderLevel << "ms";
      #endif

      currentSample += data;                      // cache data (waiting for sample) or send it (if already sending)
      #ifdef SIMOND_DEBUG
      qDebug() << "Adding data to sample...";
      #endif

      //stayed above level
      if (waitingForSampleToStart) {
        if (currentTime + thisTime - lastTimeUnderLevel > shortSampleCutoff) {
          #ifdef SIMOND_DEBUG
          qDebug() << "Sending started...";
          #endif
          waitingForSampleToStart = false;
          waitingForSampleToFinish = true;
          if (!currentlyRecordingSample) {
            m_startListening = true;
            emit listening();
            passDataThrough = true;
            currentlyRecordingSample = true;
          }
        }
      }
      else {
        passDataThrough = true;
      }
    }
    else {
      //crossed upward
      #ifdef SIMOND_DEBUG
      qDebug() << "Crossed level upward...";
      #endif
      if (waitingForSampleToInit) {
        sampleStartTime = lastTimeUnderLevel - headMargin;
        #ifdef SIMOND_DEBUG
        qDebug() << "... Sending sample start time:" << sampleStartTime;
        #endif
        waitingForSampleToInit = false;
      }
      currentSample += data;
      if (waitingForSampleToFinish)
        passDataThrough = true;
    }
    lastTimeOverLevel = currentTime + thisTime;
  }
  else {
    if (lastLevel < levelThreshold) {
      //stayed below level
      #ifdef SIMOND_DEBUG
      qDebug() << "Still below level - now for : " << currentTime + thisTime - lastTimeOverLevel << "ms";
      #endif
    } else {
      //crossed downward
      #ifdef SIMOND_DEBUG
      qDebug() << "Crossed level downward...";
      #endif
    }
    bool silentLongerThanTailMargin = (currentTime + thisTime - lastTimeOverLevel > tailMargin);
    if (waitingForSampleToFinish) {
      //still append data during tail margin
      currentSample += data;
      passDataThrough = true;
      if (silentLongerThanTailMargin) {
        m_doneListening = true;
        currentlyRecordingSample = false;
        waitingForSampleToFinish = false;
        waitingForSampleToStart  = true;
        waitingForSampleToInit = true;
        #ifdef SIMOND_DEBUG
        qDebug() << "Sample finalized and sent.";
        #endif
      }
    } else if (waitingForSampleToInit) {
      //get a bit of data before the first level cross
      currentSample += data;
      currentSample = currentSample.right(SoundServer::getInstance()->lengthToByteSize(headMargin, m_deviceConfiguration));
    } else if (waitingForSampleToStart && !waitingForSampleToInit) {
      if (silentLongerThanTailMargin) {
        waitingForSampleToInit = true;
        waitingForSampleToStart = true;
        currentSample = currentSample.right(SoundServer::getInstance()->lengthToByteSize(headMargin, m_deviceConfiguration));
      } else {
        currentSample += data;
      }
    }

    lastTimeUnderLevel = currentTime + thisTime;
  }

  lastLevel = peak();

  if (passDataThrough || m_passAll) {
    if (currentSample.count() > data.count()) {
      //contained cached data as such must be the first sending
      #ifdef SIMOND_DEBUG
      qDebug() << "STARTED!";
      #endif
      currentTime = sampleStartTime;
    }
    data = currentSample;
    currentSample.clear();
  }
  else {
    data.clear();
  }

  if (m_doneListening) {
    #ifdef SIMOND_DEBUG
    qDebug() << "Emitting complete";
    #endif
    emit complete(sampleStartTime, currentTime + thisTime);
  }
}


void VADSoundProcessor::reset()
{
  lastTimeOverLevel = -1;
  lastTimeUnderLevel = -1;
  waitingForSampleToInit = true;
  waitingForSampleToStart = true;
  waitingForSampleToFinish = false;
  currentlyRecordingSample = false;
}
