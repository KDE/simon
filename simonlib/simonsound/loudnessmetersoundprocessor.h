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

#ifndef SIMON_LOUDNESSMETERSOUNDPROCESSOR_H_BAC60251BE6A419EA1236280815A2AAD
#define SIMON_LOUDNESSMETERSOUNDPROCESSOR_H_BAC60251BE6A419EA1236280815A2AAD

#include "simonsound_export.h"
#include "soundprocessor.h"
#include <QObject>

class SIMONSOUND_EXPORT LoudnessMeterSoundProcessor : public QObject, public SoundProcessor
{
  Q_OBJECT

  protected:
    int m_peak;
    int m_average;
    int m_absolutePeak;
    int m_absoluteMinAverage;
    bool m_clipping;

  public:
    LoudnessMeterSoundProcessor();

    void process(QByteArray& data, qint64& currentTime);

    int maxAmp() { return 32768; }
    int average() { return m_average; }
    int peak() { return m_peak; }
    int absoluteMinAverage() { return m_absoluteMinAverage; }
    int absolutePeak() { return m_absolutePeak; }
    bool clipping() { return m_clipping; }
    void reset();
};
#endif
