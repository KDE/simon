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
#include <QAudioInput>

#include <KDebug>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef USE_WITH_SIMON
#include <adinstreamer/adinstreamer.h>
#endif

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

float recording_level=0;

/**
 * \brief Constructor
 */
WavRecorder::WavRecorder(QObject* parent) : QObject(parent),
	input(NULL),
	wavData(0),
	timeCounter(0)
{
	connect(&timeWatcher, SIGNAL(timeout()), this, SLOT(publishTime()));
}


#if 0
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

	qint16* audioData = (qint16*) inputBuffer;
	qint16* audio_ptr = audioData;
	wav->addData(audioData, framesPerBuffer);

	/*float* audioData = (float*) inputBuffer;
	short *converted = (short*) malloc(framesPerBuffer*sizeof(short));
	if (!converted) return paAbort;

	memset(converted, 0, framesPerBuffer*sizeof(short));
	wav->addData(converted-framesPerBuffer, framesPerBuffer);*/

	qint64 all=0;
	for (unsigned long i=0; i<framesPerBuffer; i++)
	{
		all += *audio_ptr;
		audio_ptr++;
	}
	float diff = abs(((float)all / (float)framesPerBuffer));
	if (diff > 25) 
		recording_level += log(diff) / 10.0/* / 32767.f*/;
	recording_level_counter++;


	return paContinue;
}
#endif


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
		AdinStreamer::getInstance()->stopSoundStream();
	#endif

	if (input)
	{
		input->deleteLater();
		input = NULL;
	}

	if (wavData)
	{
		delete wavData;
		wavData = NULL;
	}


#ifdef USE_WITH_SIMON
	if (AdinStreamer::hasInstance())
		AdinStreamer::getInstance()->restartSoundStream();
#endif

	int sampleRate = SoundConfiguration::soundSampleRate();
	int channels = SoundConfiguration::soundChannels();

	QAudioFormat format;
	format.setFrequency(sampleRate);
	format.setChannels(channels);
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	QString inputDevice = SoundConfiguration::soundInputDevice();
	QAudioDeviceInfo selectedInfo = QAudioDeviceInfo::defaultInputDevice();
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
		if (deviceInfo.deviceName() == SoundConfiguration::soundInputDevice())
			selectedInfo = deviceInfo;

	if (!selectedInfo.isFormatSupported(format))
	{
#ifdef USE_WITH_SIMON
		if (AdinStreamer::hasInstance())
			AdinStreamer::getInstance()->restartSoundStream();
#endif
		return false;
	}

	input = new QAudioInput(selectedInfo, format, this);

	wavData = new WAV(filename, channels, sampleRate);
	wavData->beginAddSequence();
	input->start(wavData);

	timeCounter = 0;
	timeWatcher.start(100);

	return true;
}


void WavRecorder::publishTime()
{
	//FIXME: level (second argument)
	emit currentProgress(timeCounter*100, timeCounter);
	timeCounter++;

//	if (timeCounter == 0) return;
	//float recording_diff = recording_level / (float) recording_level_counter;
//	emit currentProgress((Pa_GetStreamTime(stream) - startTime)*1000, recording_diff);
//	recording_level_counter=0;
//	recording_level=0;
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

	input->stop();

	delete input;
	input = NULL;

	timeWatcher.stop();

	wavData->endAddSequence();
	if (! wavData->writeFile()) succ = false;
	delete wavData;
	wavData = 0;

	#ifdef USE_WITH_SIMON
	if (AdinStreamer::hasInstance())
		AdinStreamer::getInstance()->restartSoundStream();
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


