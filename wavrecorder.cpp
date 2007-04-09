//
// C++ Implementation: wavrecorder
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wavrecorder.h"

/**
 * \brief Constructor
 */
WavRecorder::WavRecorder(QWidget *parent) : QThread(parent)
{
	this->mic = new SoundControl();
	
}

void WavRecorder::record(QString filename, short channels, int sampleRate)
{
	this->wavData = new WAV(filename, sampleRate);
	mic->initializeMic(channels, sampleRate);
	
	progressTimer = new QTimer();
	connect(progressTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
	progressTimer->start(100);
	
	killMe=false;
	start();
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
void WavRecorder::finish()
{
	killMe = true;
	wait(1000);
	wavData->writeFile();
	mic->close();
	progressTimer->stop();
}

/**
 * \brief Starts the thread
 * 
 * \author Peter Grasch
 * \see exec()
 */
void WavRecorder::run()
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
void WavRecorder::increaseProgress()
{
	progress+=100;
	emit currentProgress(progress);
}


/**
 * \brief Main execution loop
 * 
 * \author Peter Grasch
 */
void WavRecorder::exec()
{
	if (!mic || !wavData) return;
	int msecs=0; //current timecode
	while (!killMe)
	{
		unsigned long length=0;
		char* data = mic->capture( 100, length );
		wavData->addData( data, length );
	}
}


/**
 * \brief Destructor
 */
WavRecorder::~WavRecorder()
{
	delete mic;
	delete wavData;
}


