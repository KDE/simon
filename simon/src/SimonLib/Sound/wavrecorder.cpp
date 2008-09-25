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
#include <QObject>
#include "wav.h"
#include <stdlib.h>
#include <string.h>
#include "../Logging/logger.h"
#include "../Settings/settings.h"
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
	if( err != paNoError ) return false;

	bzero( &inputParameters, sizeof( inputParameters ) );

	int channels = Settings::getI("Sound/Channels");
	int sampleRate = Settings::getI("Sound/SampleRate");
	inputParameters.device = Settings::getI("Sound/InputDevice");

	inputParameters.channelCount = channels;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
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

	if( err != paNoError ) return false;
	
	startTime = Pa_GetStreamTime(stream);

	this->wavData = new WAV(filename, channels, sampleRate);
	
	wavData->beginAddSequence();
	err = Pa_StartStream( stream );
	if( err != paNoError ) return false;
	
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
	timeWatcher.stop();
	PaError err = Pa_StopStream( stream );
	
	wavData->endAddSequence();
	if( err != paNoError ) return false;
	
	
	err = Pa_CloseStream( stream );
	if( err != paNoError ) return false;
	

	stream = 0;
	if (! wavData->writeFile()) return false;
	

	Pa_Sleep( 500 );

	err = Pa_Terminate();
	if( err != paNoError ) return false;
	
	
	delete wavData;
	wavData = 0;
	return true;
}


/**
 * \brief Destructor
 */
WavRecorder::~WavRecorder()
{
    if (wavData) delete wavData;
}


