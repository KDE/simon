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

#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include "simoninfo.h"
#include "wav.h"
#include "RtError.h"
#include "RtAudio.h"
#include <stdio.h>
#include <iostream>

/**
	\class WavPlayer
	
	\brief This class is used to get the needed data out of existing wav files to play them
	
	\author Peter Grasch
	\version 0.1
*/
class WavPlayer : public QObject {
	Q_OBJECT
private:
	QTimer *progressTimer;
	char* data;
	
	int length;
	int chans;
	int progress;
	long position;
	bool stopTimer;
	RtAudio *audio;
	
	static int processWrapper(char *buffer, int bufferSize, void* play);
signals:
	void currentProgress(int);
	void finished();
public:
	char* getData() { return this->data; }
	int getChannels() { return chans; }
	long getPosition() { return this->position; }
	void setPosition(long position) { this->position = position; }
	int getLength() { return this->length; }
    WavPlayer(QWidget *parent=0);
    bool play(QString filename);
    ~WavPlayer();
public slots:
	void increaseProgress();
	void stop();
	


};

#endif
