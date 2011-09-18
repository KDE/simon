/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include "soundinput.h"
#include "settings.h"
#include <QDebug>
#include <QAudioInput>
#include <QAudioFormat>
#include <QMutexLocker>
#include <QAudioDeviceInfo>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

SoundInput::SoundInput(int channels, int sampleRate, QObject *parent) :
    QIODevice(parent),
    input(0),
    m_channels(channels),
    m_sampleRate(sampleRate),
    m_currentTime(0),
    m_peak(0),
    m_average(0),
    m_absolutePeak(0),
    m_absoluteMinAverage(maxAmp()),
    m_clipping(false),
    lastLevel(0),
    lastTimeUnderLevel(0),
    lastTimeOverLevel(0),
    waitingForSampleToStart(true),
    waitingForSampleToFinish(false),
    currentlyRecordingSample(false)
{
    open(QIODevice::ReadWrite|QIODevice::Unbuffered);
}

bool SoundInput::init()
{
    delete input;

    QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultInputDevice();
    QAudioFormat format = selectedInfo.preferredFormat();
    format.setFrequency(m_sampleRate);
    format.setChannels(m_channels);
    format.setSampleSize(16);                       // 16 bit
    format.setSampleType(QAudioFormat::SignedInt);  // SignedInt currently
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    if (!selectedInfo.isFormatSupported(format)) {
        emit error(tr("Required audio format not supported by sound hardware"));
        return false;
    }

    input = new QAudioInput(selectedInfo, format, this);
    connect(input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateChanged()));
    input->setBufferSize(2048);

    emit microphoneLevel(0, 0, maxAmp());

    input->start(this);
    return true;
}

void SoundInput::stateChanged()
{
    switch (input->error()) {
    case QAudio::NoError:
        return;
    case QAudio::OpenError:
        emit error(tr("Failed to open audio device"));
        break;
    case QAudio::UnderrunError:
        //overclock at this point :P
        break;
    case QAudio::IOError:
        emit error(tr("Failed to read data from microphone"));
        break;
    case QAudio::FatalError:
        emit error(tr("A fatal error occured during recording."));
        init();
        break;
    }
}

void SoundInput::dropCache()
{
    QMutexLocker l(&bufferLock);
    localBuffer.clear();
}

qint64 SoundInput::readData(char* data, qint64 maxSize)
{
    QMutexLocker l(&bufferLock);
    int length = qMin((int) maxSize, localBuffer.count());
    memcpy(data, localBuffer.data(), length);
    localBuffer.clear();
    return length;
}

void SoundInput::analyzeSoundProperties(const QByteArray &data)
{
    const short* frames = (short*) data.constData();
    m_peak = 0;
    m_average = 0;

    m_clipping = false;
    unsigned int i;
    for (i=0; i < (data.size() / sizeof(short)); i++) {
        int frame = qAbs(frames[i]);
        m_peak = qMax(m_peak, frame);
        m_average += frame;
    }

    m_average = m_average / i;

    m_absolutePeak = qMax(m_peak, m_absolutePeak);
    m_absoluteMinAverage = qMin(m_peak, m_absoluteMinAverage);

    if (m_peak >= (maxAmp()-1))
        m_clipping = true;

    emit microphoneLevel(m_peak);
}

int SoundInput::lengthToBytes(int length)
{
    return length * getLengthFactor();
}

int SoundInput::bytesToLength(int bytes)
{
    return bytes / getLengthFactor();
}

int SoundInput::getLengthFactor()
{
    return m_channels * 2 /*16 bit*/ * (m_sampleRate / 1000);
}

void SoundInput::process(QByteArray& data)
{
    analyzeSoundProperties(data);

    if (!Settings::voiceActivityDetection())
        return;

    m_currentTime += bytesToLength(data.count());

    int levelThreshold = Settings::cutoffLevel();
    int headMargin = Settings::headMargin();
    int tailMargin = Settings::tailMargin();
    int shortSampleCutoff = Settings::minimumSampleLength();

    bool passDataThrough = false;


    if (peak() > levelThreshold) {
      if (lastLevel > levelThreshold) {
        currentSample += data;                      // cache data (waiting for sample) or send it (if already sending)

        //stayed above level
        if (waitingForSampleToStart) {
          if (m_currentTime - lastTimeUnderLevel > shortSampleCutoff) {
            waitingForSampleToStart = false;
            waitingForSampleToFinish = true;
            if (!currentlyRecordingSample) {
              emit listening();
              passDataThrough = true;
              currentlyRecordingSample = true;
            }
          }
        } else {
          passDataThrough = true;
        }
      } else {
        //crossed upward
        currentSample += data;
      }
      lastTimeOverLevel = m_currentTime;
    } else {
      waitingForSampleToStart = true;
      if (lastLevel < levelThreshold) {
        //stayed below level
        if (waitingForSampleToFinish) {
          //still append data during tail margin
          currentSample += data;
          passDataThrough = true;
          if (m_currentTime - lastTimeOverLevel > tailMargin) {
            currentlyRecordingSample = false;
            waitingForSampleToFinish = false;
            qDebug() << "Sample finalized and sent.";
            emit complete();
          }
        } else {
          //get a bit of data before the first level cross
          currentSample += data;
          currentSample = currentSample.right(lengthToBytes(headMargin));
        }
      } else {
        //crossed downward
        currentSample += data;
      }
      lastTimeUnderLevel = m_currentTime;
    }

    lastLevel = peak();

    if (passDataThrough) {
      data = currentSample;
      currentSample.clear();
    } else
        data.clear();
}

qint64 SoundInput::writeData(const char* data, qint64 maxSize)
{
    QByteArray samples = QByteArray::fromRawData(data, maxSize);
    process(samples);

    bufferLock.lock();
    localBuffer.append(samples);
    bufferLock.unlock();
    emit readyRead();
    return maxSize;
}
