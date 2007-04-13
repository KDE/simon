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
 */
WavPlayer::WavPlayer(QWidget *parent) : QThread(parent)
{
	this->sound = new SoundControl();
	progress = 0;
}


/**
 * \brief Plays back the given file
 * \author Peter Grasch
 */
bool WavPlayer::play( QString filename )
{
	WAV *file = new WAV(filename); 
	
	this->data = file->getRawData(this->length);
	if (length==0) return false;
	
	if (!sound->initializeSpeaker(2,44100))
		return false;
	
	progressTimer = new QTimer();
	connect(progressTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
	progressTimer->start(100);
	connect (this, SIGNAL(finished()), this, SLOT(stop())); //to disable the progressTimer
	
	start(); //start the thread
	setPriority(QThread::LowestPriority);
	
	return true;
}


/**
 * \brief Starts the thread
 * 
 * \author Peter Grasch
 */
void WavPlayer::run()
{
	exec();
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
	progress+=104;
	emit currentProgress(progress);
}


/**
 * \brief Main execution loop
 * 
 * \author Peter Grasch
 */
void WavPlayer::exec()
{
	if (!sound || !data) return;
	
	sound->playback ( data, length );
}

/**
 * \brief Stops the current playback
 * 
 * \author Peter Grasch
 */
void WavPlayer::stop()
{
	//this function is also called when the thread is finished to stop the timer

	sound->close();
	progressTimer->stop();
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavPlayer::~WavPlayer()
{
	//delete sound;
	delete progressTimer;
	delete data;
}


