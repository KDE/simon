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

#include "soundoutputclient.h"
#include "soundprocessor.h"
#ifdef HAVE_LIBSAMPLERATE_H
#include "resamplesoundprocessor.h"
#endif

/**
 * \brief Constructor
 */
SoundOutputClient::SoundOutputClient(const SimonSound::DeviceConfiguration& deviceConfiguration) :
SoundClient(deviceConfiguration, SoundClient::Normal)
{
  localBuffer.open(QIODevice::ReadOnly);
}

qint64 SoundOutputClient::read(char *data, qint64 maxlen)
{
  qint64 currentStreamTime = 0;
  //processors and relay
  while (localBuffer.bytesAvailable() < maxlen) {
    QByteArray processedData = getDataProvider()->read(maxlen);

    if (processedData.isEmpty())
      break; // no more data from this device

    foreach (SoundProcessor *p, processors) {
      p->process(processedData, currentStreamTime);
      if (processedData.isEmpty()) break; //sound processors ate everything
    }
    localBuffer.buffer().append(processedData);
  }
  
  qint64 length = localBuffer.read(data, maxlen);
  localBuffer.buffer().remove(0, length);
  localBuffer.seek(0);

  return length;
}

void SoundOutputClient::initToSampleRate(int contentSampleRate)
{
#ifdef HAVE_LIBSAMPLERATE_H
  if (m_deviceConfiguration.resample() && 
          m_deviceConfiguration.sampleRate() != contentSampleRate)
      registerSoundProcessor(new ResampleSoundProcessor(
                m_deviceConfiguration.channels(),
                contentSampleRate,
                m_deviceConfiguration.sampleRate()));
#endif
}

/**
 * \brief Destructor
 */
SoundOutputClient::~SoundOutputClient()
{
}

