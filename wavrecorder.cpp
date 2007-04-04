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
WavRecorder::WavRecorder() : QThread()
{
	this->mic = new MicControl();
	
}

void WavRecorder::record(QString filename, short channels, int sampleRate)
{
	if (wavData) delete wavData;
	this->wavData = new WAV(filename, sampleRate);
	mic->initializeMic(channels, sampleRate);
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
void WavRecorder::finish()
{
	//QMessageBox::information(0,"otecdi","otceidstcoeid");
	wavData->writeFile();
	this->terminate();
}

/**
 * \brief Starts the thread
 * 
 * \author Peter Grasch
 */
void WavRecorder::run()
{
	exec();
	wait(5000);
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
	while (true)
	{
		unsigned long length=0;
		char* data = mic->capture( 100, length );
		wavData->addData( data, length );
		
		msecs+=100;
		emit currentProgress(msecs);
		//prog->setMaximum(msecs/100);
		//this->textprog->setText("00:00 / "+QString::number(msecs/1000)+":"+QString::number(msecs/10));
	}
	mic->closeMic();
}


/**
 * \brief Destructor
 */
WavRecorder::~WavRecorder()
{
}


