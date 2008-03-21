//
// C++ Implementation: wavplayer
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wavplayer.h"
#include "logger.h"
#include "settings.h"
#include <QTimer>
#include <QObject>
#include "wav.h"
#include "RtError.h"
#include "RtAudio.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 */
WavPlayer::WavPlayer(QObject *parent) : QObject(parent)
{
	audio =0;
	data = 0;
	progressTimer = new QTimer();
	connect(progressTimer, SIGNAL(timeout()), this, SLOT(closeStream()));
}


int process( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *userData )
{
	WavPlayer *play = (WavPlayer*) userData;
	if (!play) return 1;

	long position = play->getWavPosition();
	int samplesToWrite = nBufferFrames*play->getChannels()*sizeof(short);
	if (play->getLength()<(position+samplesToWrite))
	{
		play->stop();
		return 1;
	}
	play->addSamplesToCounter(samplesToWrite);
	play->publishTime(streamTime);
	
	char* data = play->getData();
	outputBuffer =(char*)  memcpy(outputBuffer, data+position, samplesToWrite);

	return 0;
}



void WavPlayer::publishTime(double time)
{
	emit currentProgress(time*1000);
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayer::play( QString filename )
{
	Logger::log(tr("[INF] Abspielen von %1").arg(filename)); 
	wavPosition = 0;
	if (audio) {
		delete audio;
		audio =0;
	}
	audio = new RtAudio();

	WAV *file = new WAV(filename); 
	this->data = file->getRawData(this->length);
	if (length==0) return false;


	RtAudio::StreamParameters parameters;
	parameters.deviceId = Settings::getI("Sound/OutputDevice");
	parameters.nChannels = this->chans = file->getChannels();
	parameters.firstChannel = 0;
	unsigned int sampleRate = file->getSampleRate();
	unsigned int bufferFrames = 256; // 256 sample frames
	try {
		audio->openStream( &parameters, NULL, RTAUDIO_SINT16,
				    sampleRate, &bufferFrames, &process, (void *) this );
		
		stopTimer = false;
		progressTimer->start(100);
		
		audio->startStream();
	}
	catch ( RtError& e ) {
		e.printMessage();
		return false;
  	}

	return true;
}



// int WavPlayer::processWrapper(char* buffer, int bufferSize, void *play)
// {
// 	char* data = ((WavPlayer*) play)->getData();
// 	
// 	long position = ((WavPlayer*) play)->getPosition();
// 	int channels = ((WavPlayer*) play)->getChannels();
// 	long realBufferLength = bufferSize*channels*sizeof(signed short);
// 	
// 	if (((WavPlayer*) play)->getLength() <= position+realBufferLength)
// 	{
// 		((WavPlayer*) play)->stop();
// 		return 1;
// 	}
// 	
// 	buffer =(char*)  memcpy(buffer, data+position, realBufferLength);
// 	((WavPlayer*) play)->setPosition(position+realBufferLength);
// 	return 0;
// }

/**
 * \brief Increases the progress by 100 msecs
 * 
 * emits the currentProgress signal
 * Also watches the killflag "stopTimer" which is used to stop the timer
 * and close the stream when finished
 * 
 * \see stop()
 * 
 * \author Peter Grasch
 */
void WavPlayer::closeStream()
{
	if ( stopTimer )
	{
		progressTimer->stop();
		try
		{
			// Stop and close the stream
			//audio->abortStream();
			audio->stopStream();
			audio->closeStream();
			delete audio;
			audio =0;
		}
		catch ( RtError &error )
		{
	//             Logger::log(tr("[ERR]")+" "+error.getMessageString());
			error.printMessage();
		}
	
		delete data;
		data = 0;
		emit finished();
	
		stopTimer=false;
		return;
	}
}

/**
 * \brief Stops the current playback
 * 
 * \author Peter Grasch
 */
void WavPlayer::stop()
{
	stopTimer = true; // to work around the issue that you can't stop the timer from a different thread
	//which would be the case if we would stop it here (this is called from the callback thread)
    //this also triggers the closing of the stream as we can't stop it here because it would still be open
    //from the callback function
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayer::~WavPlayer()
{
    progressTimer->deleteLater();
    if (data) delete data;
    if (audio) delete audio;
}


