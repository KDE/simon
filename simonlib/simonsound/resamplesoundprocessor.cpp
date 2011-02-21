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

#include "resamplesoundprocessor.h"
#include <KDebug>

extern "C"
{
#include <samplerate.h>
}


ResampleSoundProcessor::ResampleSoundProcessor(int channels, int sourceFreq, int targetFreq) :
  m_channels(channels), m_sourceFreq(sourceFreq), m_targetFreq(targetFreq)
{
  int err;
  //state = src_new(SRC_SINC_BEST_QUALITY, channels, &err);
  state = src_new(SRC_SINC_MEDIUM_QUALITY, channels, &err);
  if (!state)
    kWarning() << "Couldn't initialize libsamplerate: " << src_strerror(err);
}

void ResampleSoundProcessor::process(QByteArray& data, qint64& currentTime)
{
  Q_UNUSED(currentTime);

  internalBuffer.append(data);

  int inputFrames = internalBuffer.length() / 2 /* 16 bit format */;
  int targetFrames = qRound(((double) inputFrames /  (double) m_sourceFreq) * m_targetFreq);

  float* outData = (float*) malloc(sizeof(float)*targetFrames);
  float* inData = (float*) malloc(sizeof(float)*inputFrames);

  src_short_to_float_array((const short int*) internalBuffer.data(), inData, inputFrames);

  SRC_DATA *resampleData = new SRC_DATA;

  resampleData->data_in = inData;
  resampleData->data_out = outData;
  resampleData->input_frames = inputFrames;
  resampleData->output_frames = targetFrames;
  resampleData->src_ratio = ((double) m_targetFreq) / ((double) m_sourceFreq);
  resampleData->end_of_input = 0; 
  kDebug() << "Input frames: " << inputFrames;
  kDebug() << "Targetframes: " << targetFrames;

  int error = src_process(state, resampleData);
  if (error != 0) {
    kWarning() << "Resample error: " << src_strerror(error);
    return;
  }

  int processedFrames = resampleData->output_frames_gen;
  int processedFramesLength = processedFrames*2;
  //char* outputData = (char*) malloc(processedFramesLength);
  data.resize(processedFramesLength);
  src_float_to_short_array(resampleData->data_out, (short int*) data.data(), processedFrames);
  //data = QByteArray::fromRawData(outputData, processedFramesLength);
  internalBuffer.remove(0, resampleData->input_frames_used * 2 /* 16 bit */);
  kDebug() << "Resampled " << resampleData->input_frames_used << " frames to " << processedFrames;
  kDebug() << "Remaining buffer: " << internalBuffer.length();
  free(inData);
  free(outData);
  //free(outputData);
}

ResampleSoundProcessor::~ResampleSoundProcessor()
{
  if (state)
    src_delete(state);
}
