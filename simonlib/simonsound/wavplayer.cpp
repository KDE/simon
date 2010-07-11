/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "wavplayer.h"
#include "wav.h"

#include "soundconfig.h"

#include <QTimer>
#include <KLocalizedString>
#include <QObject>
#include <stdlib.h>
#include <string.h>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

/**
 * \brief Constructor
 * \author Peter Grasch
 */
WavPlayer::WavPlayer(QObject* parent) : QObject(parent),
stream(0),
wav(0),
data(0)
{
  connect(&timeWatcher, SIGNAL(timeout()), this, SLOT(publishTime()));
}


int processOutputData( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags status, void *userData )
{
  Q_UNUSED(inputBuffer);
  Q_UNUSED(timeInfo);
  Q_UNUSED(status);
  WavPlayer *play = (WavPlayer*) userData;
  if (!play) return 1;

  int retVal = 0;

  if (play->getPosition()+framesPerBuffer > play->getLength()) {
    framesPerBuffer = play->getLength()-play->getPosition();
    retVal = 1;
    play->stop();
  }

  memcpy(outputBuffer, play->getData()+play->getPosition(), framesPerBuffer*sizeof(float));

  play->advance(framesPerBuffer);

  return retVal;
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayer::play( QString filename )
{
  //	delete stream;
  stream = 0;
}


delete wav;
wav = 0;

wavPosition = 0;
length = 0;
stopTimer = false;

wav = new WAV(filename);
short* shortData = (short*) wav->getRawData(this->length);
if (length==0) return false;

this->data = (float*) malloc(length*sizeof(float));

for( unsigned int i=0; i<length; i++ )            //convert to floats
{                                                 //the wav file is stored using signed 16bit integers, but the
  //audio device expects 32-bit floats.
  //this (ugly) code should be replaced with a nice integrated
  //solution like phonon (once it is able to record...)
  *data = ((float) *shortData) / 32768.0f;
  data++;
  shortData++;
}


data -= length;

PaStreamParameters  outputParameters;
PaError             err = paNoError;

err = Pa_Initialize();
if( err != paNoError ) return false;

bzero( &outputParameters, sizeof( outputParameters ) );

int channels = wav->getChannels();
int sampleRate = wav->getSampleRate();
//FIXME
outputParameters.device = 0;                      //SoundConfiguration::soundOutputDevice();

outputParameters.channelCount = channels;
outputParameters.sampleFormat = paFloat32;

const PaDeviceInfo *info = Pa_GetDeviceInfo( outputParameters.device );
if (!info)
  return false;

outputParameters.suggestedLatency = info->defaultLowInputLatency;
outputParameters.hostApiSpecificStreamInfo = 0;

/* Play some audio. -------------------------------------------- */

err = Pa_OpenStream(
&stream,
0,                                                /* &inputParameters, */
&outputParameters,
(double) sampleRate,
1024,                                             //frames per buffer
paClipOff,                                        /* we will not output out of range samples so do not bother clipping them */
processOutputData,
(void*) this );

if( err != paNoError ) return false;
this->startTime = Pa_GetStreamTime(stream);

err = Pa_StartStream( stream );
if( err != paNoError ) return false;

timeWatcher.start(100);

return true;
}


void  WavPlayer::publishTime()
{
  if (stopTimer) {
    timeWatcher.stop();
    closeStream();
  } else
  emit currentProgress((Pa_GetStreamTime(stream) - startTime)*1000);
}


void WavPlayer::closeStream()
{
  timeWatcher.stop();
  PaError err = Pa_StopStream( stream );
  if( err != paNoError ) return;

  err = Pa_CloseStream( stream );
  if( err != paNoError ) return;

  stream = 0;

  Pa_Sleep( 500 );

  err = Pa_Terminate();
  if( err != paNoError ) return;

  delete wav;                                     // data gets killed by WAV itself
  wav = 0;

  emit finished();
}


/**
 * \brief Stops the current playback
 *
 * \author Peter Grasch
 */
void WavPlayer::stop()
{
  stopTimer = true;                               // to work around the issue that you can not stop the timer from a different thread
  //which would be the case if we would stop it here (this is called from the callback thread)
  //this also triggers the closing of the stream as we can not stop it here because it would still be open
  //from the callback function
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayer::~WavPlayer()
{
  delete wav;
}
