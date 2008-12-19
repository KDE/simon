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

#include "wavrecorder.h"
#include "wav.h"

#include "soundconfig.h"

#include <QObject>
#include <KDebug>

#include <stdlib.h>
#include <string.h>

#ifdef USE_WITH_SIMON
#include <adinstreamer/adinstreamer.h>
#endif

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

/**
 * \brief Constructor
 */
WavRecorder::WavRecorder(QObject *parent) : QObject(parent)
{
	wavData=0;
	stream =0;
	
	connect(&timeWatcher, SIGNAL(timeout()), this, SLOT(publishTime()));
}


int processInputData( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
		 const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags status, void *userData )
{
	Q_UNUSED(timeInfo);
	Q_UNUSED(status);

	if (!inputBuffer) return paContinue;

	(void) outputBuffer; //prevent unused-variable-message


	WavRecorder *rec = (WavRecorder*) userData;
	if (!rec) return paAbort;

	WAV *wav = rec->getWav();

	float* audioData = (float*) inputBuffer;
	short *converted = (short*) malloc(framesPerBuffer*sizeof(short));
	if (!converted) return paAbort;

	memset(converted, 0, framesPerBuffer*sizeof(short));

	for (unsigned long i=0; i<framesPerBuffer; i++)
	{
		*converted  = (int) (*audioData * 32768.0f);
		audioData++;
		converted++;
	}

	wav->addData(converted-framesPerBuffer, framesPerBuffer);


	return paContinue;
}


/**
 * \brief Records a WAV file to the given filename
 * \author Peter Grasch
 * \param QString filename
 * Filename to write to
 */
bool WavRecorder::record(QString filename)
{
	#ifdef USE_WITH_SIMON
	if (AdinStreamer::hasInstance())
	{
		AdinStreamer::getInstance()->stopSoundStream();
	}
	#endif


	if (stream)
	{
// 		delete stream;
		stream = 0;
	}
	if (wavData)
	{
		delete wavData;
		wavData = 0;
	}


	PaStreamParameters  inputParameters;
	PaError             err = paNoError;

	err = Pa_Initialize();
	if( err != paNoError ) {
#ifdef USE_WITH_SIMON
		if (AdinStreamer::hasInstance())
			AdinStreamer::getInstance()->restartSoundStream();
#endif
		return false;
	}

	bzero( &inputParameters, sizeof( inputParameters ) );

	int channels = SoundConfiguration::soundChannels();
	int sampleRate = SoundConfiguration::soundSampleRate();
	inputParameters.device = SoundConfiguration::soundInputDevice();

	kDebug() << inputParameters.device << SoundConfiguration::soundInputDevice() << channels << sampleRate;

	inputParameters.channelCount = channels;
	inputParameters.sampleFormat = paFloat32;

	const PaDeviceInfo *info = Pa_GetDeviceInfo( inputParameters.device );
	if (!info)
	{
#ifdef USE_WITH_SIMON
		if (AdinStreamer::hasInstance())
			AdinStreamer::getInstance()->restartSoundStream();
#endif
		return false;
	}
	inputParameters.suggestedLatency = info->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	/* Record some audio. -------------------------------------------- */
	
	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,                  /* &outputParameters, */
		(double) sampleRate,
		1024, //frames per buffer
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		processInputData,
		(void*) this );

	if( err != paNoError ) {
#ifdef USE_WITH_SIMON
		if (AdinStreamer::hasInstance())
			AdinStreamer::getInstance()->restartSoundStream();
#endif
		return false;
	}
	
	startTime = Pa_GetStreamTime(stream);

	this->wavData = new WAV(filename, channels, sampleRate);
	
	wavData->beginAddSequence();
	err = Pa_StartStream( stream );
	if( err != paNoError ) {
#ifdef USE_WITH_SIMON
		if (AdinStreamer::hasInstance())
			AdinStreamer::getInstance()->restartSoundStream();
#endif
		return false;
	}
	
	timeWatcher.start(100);

	return true;
}


void WavRecorder::publishTime()
{
	emit currentProgress((Pa_GetStreamTime(stream) - startTime)*1000);
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
bool WavRecorder::finish()
{
	bool succ = true;
	timeWatcher.stop();
	PaError err = Pa_StopStream( stream );
	
	wavData->endAddSequence();
	if( err != paNoError ) succ = false;
	else {
		err = Pa_CloseStream( stream );
		if( err != paNoError ) succ = false;
	}
	
	stream = 0;
	if (! wavData->writeFile()) succ = false;

	Pa_Sleep( 500 );
	err = Pa_Terminate();

	if( err != paNoError ) succ=false;
	
	delete wavData;
	wavData = 0;

	#ifdef USE_WITH_SIMON
	if (AdinStreamer::hasInstance())
	{
		AdinStreamer::getInstance()->restartSoundStream();
	}
	#endif

	return succ;
}


/**
 * \brief Destructor
 */
WavRecorder::~WavRecorder()
{
    if (wavData) delete wavData;
}


