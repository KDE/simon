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

#include "loudnessmetersoundprocessor.h"
#include <QByteArray>

/**
 * \brief Constructor
 */

LoudnessMeterSoundProcessor::LoudnessMeterSoundProcessor() :
QObject(),
SoundProcessor(),
m_peak(0),
m_average(0),
m_absolutePeak(0),
m_absoluteMinAverage(maxAmp()),
m_clipping(false)
{
}


void LoudnessMeterSoundProcessor::process(QByteArray& data, qint64& currentTime)
{
  Q_UNUSED(currentTime);

  const short* frames = (short*) data.constData();
  m_peak = 0;
  m_average = 0;

  m_clipping = false;
  unsigned int i;
  for (i=0; i < (data.size() / sizeof(short)); i++) {
    int frame = qAbs(frames[i]);
    m_peak = qMax(m_peak, frame);
    //m_min = qMin(m_min, frame);
    m_average += frame;
  }

  m_average = m_average / i;

  m_absolutePeak = qMax(m_peak, m_absolutePeak);
  // 	m_absoluteMinAverage = qMin(m_average, m_absoluteMinAverage);
  m_absoluteMinAverage = qMin(m_peak, m_absoluteMinAverage);

  if (m_peak >= (maxAmp()-1))
    m_clipping = true;
}

void LoudnessMeterSoundProcessor::reset()
{
  m_peak = 0;
  m_average = 0;
  m_absolutePeak = 0;
  m_absoluteMinAverage = maxAmp();
  m_clipping = false;
}

