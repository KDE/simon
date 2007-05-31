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

/**
 * \brief Constructor
 * \author Peter Grasch
 * \todo Fix the hardcoded stereo
 */
WavPlayer::WavPlayer(QWidget *parent) : QObject(parent)
{
// 	this->sound = new SoundControl();
	progressTimer = new QTimer();
	connect(progressTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
	audio = new RtAudio();
	position=0;
	chans=2;
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayer::play( QString filename )
{
	progress = 0;
	position=0;
	
	int device=0, bufferSize=512, nBuffers=4;
	WAV *file = new WAV(filename); 
	
	this->data = file->getRawData(this->length);
	if (length==0) return false;
	
	
	try {
		audio = new RtAudio(device, chans, 0, 0, RTAUDIO_SINT16,
				    file->getSampleRate(), &bufferSize, nBuffers);
	}
	catch (RtError &error) {
		error.printMessage();
		return false;
	}

	try {
		audio->setStreamCallback(&processWrapper, (void*) this);
		audio->startStream();
	}
	catch (RtError &error) {
		error.printMessage();
		delete audio;
	}
	progressTimer->start(100);

// 	stop();
}


int WavPlayer::processWrapper(char* buffer, int bufferSize, void *play)
{
	char* data = ((WavPlayer*) play)->getData();
	
	long position = ((WavPlayer*) play)->getPosition();
	int channels = ((WavPlayer*) play)->getChannels();
	long realBufferLength = bufferSize*channels*sizeof(signed short);
	
	if (((WavPlayer*) play)->getLength() <= 
		     position+realBufferLength)
	{
		((WavPlayer*) play)->stop();
		return 1;
	}
	
	buffer =(char*)  memcpy(buffer, data+position, realBufferLength);
	((WavPlayer*) play)->setPosition(position+realBufferLength);
	return 0;
}

/**
 * \brief Increases the progress by 100 msecs
 * 
 * emits the currentProgress signal
 * 
 * \author Peter Grasch
 */
void WavPlayer::increaseProgress()
{
	progress+=100;
	emit currentProgress(progress);
}

/**
 * \brief Stops the current playback
 * 
 * \author Peter Grasch
 */
void WavPlayer::stop()
{
	//this function is also called when the thread is finished to stop the timer
	try {
    		// Stop and close the stream
		audio->stopStream();
		audio->closeStream();
		delete audio;
	}
	catch (RtError &error) {
		error.printMessage();
	}
	
	progressTimer->stop();
	delete data;
	emit finished();
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayer::~WavPlayer()
{
}


