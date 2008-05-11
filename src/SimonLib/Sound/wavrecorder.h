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
	
	It uses a SoundControl to control the microphone, captures data (until its interrupted)
	Then it writes the data into a specified wav-file.
	It utilizes callback functions for optimal performance
	
	\date 27.05.2007
*/

#include <QObject>
#include "RtAudio.h"

class WAV;
class RtAudio;

class WavRecorder : public QObject {
	Q_OBJECT
private:
	WAV *wavData;
	RtAudio *audio;
	int chans;

signals:
	void currentProgress(int msecs);
	
public:
// int processData( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
// 	    double streamTime, RtAudioStreamStatus status, void *userData );
// 	int processData( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
// 		 double streamTime, RtAudioStreamStatus status, void *userData );
	WAV* getWav() { return wavData; }
	int getChannels() { return chans; }
	WavRecorder(QObject *parent=0);
	bool finish();
    	bool record(QString filename);
    	void publishTime(double time);
    

    ~WavRecorder();

};

#endif
