//
// C++ Interface: wavrecorder
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WAVRECORDER_H
#define WAVRECORDER_H

/**
	\class WavRecorder
	
	\brief This class is used to capture wave input from the mic.
	
	It uses a MicControl to control the microphone, captures data (until its interrupted)
	Then it writes the data into a specified wav-file.
	All this is done in a seperate thread;
	
	\todo Implementing; The class does not care much about buffering so we could easily run out of RAM;
*/

#include <QThread>
#include "miccontrol.h"
#include "wav.h"

class WavRecorder : public QThread {
	Q_OBJECT
private:
	WAV *wavData;
	MicControl *mic;
signals:
	void currentProgress(int msecs);
public:
	WavRecorder();
	void run();
	void exec();
	void finish();
    	void record(QString filename, short channels, int sampleRate);
    

    ~WavRecorder();

};

#endif
