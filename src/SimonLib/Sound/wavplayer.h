//
// C++ Interface: wavplayer
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WAVPLAYER_H
#define WAVPLAYER_H

#include <QObject>
#include <QTimer>
#include "portaudio.h"

class QTimer;
class WAV;

/**
	\class WavPlayer
	
	\brief This class is used to get the needed data out of existing wav files to play them
	
	\author Peter Grasch
	\version 0.1
*/
class WavPlayer : public QObject {
	Q_OBJECT
	
private:
	PaStream* stream;
	long startTime;
	QTimer timeWatcher;
	
	WAV *wav;
	float *data;
	short channels;
	unsigned long length;
	
	bool stopTimer;
	unsigned long wavPosition;

signals:
	void currentProgress(int);
	void finished();
	
private:
	void closeStream();

public:
    WavPlayer(QObject *parent=0);
    bool play(QString filename);
    ~WavPlayer();
	
	float* getData() { return data; }
	unsigned long getLength() { return length; }
	unsigned long getPosition() { return wavPosition; }
	short getChannels() { return channels; }
	void advance(int amount) { wavPosition += amount; }
	
public slots:
    	void publishTime();
		void stop();
	


};

#endif
