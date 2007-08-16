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
#include <QVariant>
#include "settings.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * \todo Fix the hardcoded stereo
 */
WavPlayer::WavPlayer(QWidget *parent) : QObject(parent)
{
	stopTimer = false;
	progressTimer = new QTimer();
	connect(progressTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
	audio = new RtAudio();
	position=0;
	chans=1;
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayer::play( QString filename )
{
	Logger::log(tr("[INF] Abspielen von %1").arg(filename)); 
	progress = 0;
	position=0;
	
	int bufferSize=512, nBuffers=4;
	WAV *file = new WAV(filename); 
	
	this->data = file->getRawData(this->length);
	if (length==0) return false;
	
	
	try {
		audio = new RtAudio(Settings::get("OutputDevice").toInt(), chans, 0, 0,
				    RTAUDIO_SINT16, file->getSampleRate(), &bufferSize, nBuffers);
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
	
	return true;
}


int WavPlayer::processWrapper(char* buffer, int bufferSize, void *play)
{
	char* data = ((WavPlayer*) play)->getData();
	
	long position = ((WavPlayer*) play)->getPosition();
	int channels = ((WavPlayer*) play)->getChannels();
	long realBufferLength = bufferSize*channels*sizeof(signed short);
	
	if (((WavPlayer*) play)->getLength() <= position+realBufferLength)
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
 * Also watches the killflag "stopTimer" which is used to stop the timer
 * and close the stream when finished
 * 
 * \see stop()
 * 
 * \author Peter Grasch
 */
void WavPlayer::increaseProgress()
{
	if (stopTimer)	{ 
        progressTimer->stop();
        try {
            // Stop and close the stream
            //audio->abortStream();
            audio->stopStream();
            audio->closeStream();
            delete audio;
        }
        catch (RtError &error) {
            Logger::log(tr("[ERR]")+" "+error.getMessageString());
            error.printMessage();
        }

        delete data;
        emit finished();

        stopTimer=false; 
        return;
    }
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
}


