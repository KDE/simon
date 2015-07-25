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

#include "wavrecorderclient.h"
#include <simonwav/wav.h>
#include <soundconfig.h>

#include "soundserver.h"
#include "vadsoundprocessor.h"

#include <QObject>

#include <KDebug>

/**
 * \brief Constructor
 */
WavRecorderClient::WavRecorderClient(const SimonSound::DeviceConfiguration& deviceConfiguration, QObject* parent) :
QObject(parent),
SoundInputClient(deviceConfiguration),
wavData(0),
vad(new VADSoundProcessor(deviceConfiguration, true))
{
  registerSoundProcessor(vad);
  connect(vad, SIGNAL(listening()), this, SIGNAL(speaking()));
  connect(vad, SIGNAL(complete(qint64, qint64)), this, SIGNAL(speakingStopped()));
}


/**
 * \brief Records a WAV file to the given filename
 * \author Peter Grasch
 * \param QString filename
 * Filename to write to
 */
bool WavRecorderClient::record(QString filename)
{
  if (wavData) {
    wavData->deleteLater();
    wavData = 0;
  }

  wavData = new WAV(filename, m_deviceConfiguration.channels(),
    m_deviceConfiguration.resample() ? m_deviceConfiguration.resampleSampleRate() :
                                       m_deviceConfiguration.sampleRate());
  wavData->beginAddSequence();

  bool succ =  SoundServer::getInstance()->registerInputClient(this);

  return succ;
}


void WavRecorderClient::processPrivate(const QByteArray& data, qint64 currentTime)
{
  wavData->write(data);

  float peak = vad->peak() / 32768.0f;
  emit currentProgress(currentTime, peak);
  if (vad->clipping())
    emit clippingOccured();
}


/**
 * \brief This will stop the current recording
 *
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool WavRecorderClient::finish()
{
  bool succ = true;

  succ = SoundServer::getInstance()->deRegisterInputClient(this);

  kDebug() << "Min: " << vad->absoluteMinAverage();
  kDebug() << "Max: " << vad->absolutePeak();
  kDebug() << "Theoretical max: " << vad->maxAmp();
  int absoluteMinAverage = vad->absoluteMinAverage();

  if (absoluteMinAverage == 0)
    absoluteMinAverage = 1;

  //ratio is in percent
  float ratio = ((float) vad->absolutePeak() / (float) absoluteMinAverage) * 100;
  kDebug() << "Ratio: " << ratio;

  if (ratio < SoundConfiguration::minimumSNR())
    emit signalToNoiseRatioLow();

  wavData->endAddSequence();
  if (!wavData->writeFile())
    succ = false;
  wavData->deleteLater();
  wavData = 0;

  vad->reset();

  return succ;
}


/**
 * \brief Destructor
 */
WavRecorderClient::~WavRecorderClient()
{
  SoundServer::getInstance()->deRegisterInputClient(this);
  if (wavData) wavData->deleteLater();
}
