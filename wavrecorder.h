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

class WAV;
class QTimer;
class RtAudio;

class WavRecorder : public QObject {
	Q_OBJECT
private:
	WAV *wavData;
	QTimer *progressTimer;
	int progress;
	RtAudio *audio;
	int chans;
	
	static int processWrapper(char *buffer, int bufferSize, void* rec);

signals:
	void currentProgress(int msecs);
public slots:
	void increaseProgress();
	
public:
	WAV* getWav() { return wavData; }
	int getChannels() { return chans; }
	WavRecorder(QObject *parent=0);
	bool finish();
    	bool record(QString filename, short channels, int sampleRate);
    

    ~WavRecorder();

};

#endif
