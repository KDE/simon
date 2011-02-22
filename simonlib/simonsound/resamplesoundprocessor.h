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

#ifndef SIMON_RESAMPLESOUNDPROCESSOR_H_D0C0BA2429B04F65935956A32C79BB09
#define SIMON_RESAMPLESOUNDPROCESSOR_H_D0C0BA2429B04F65935956A32C79BB09

#include "soundprocessor.h"
#include <QByteArray>

typedef struct SRC_STATE_tag SRC_STATE;
  

class ResampleSoundProcessor : public SoundProcessor
{
  private:
    int m_channels;
    int m_sourceFreq;
    int m_targetFreq;

    QByteArray internalBuffer;

    SRC_STATE* state;

  public:
    ResampleSoundProcessor(int channels, int sourceFreq, int targetFreq);
    void process(QByteArray& data, qint64& currentTime);
    ~ResampleSoundProcessor();
};

#endif

