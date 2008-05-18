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

class QTimer;
class RtAudio;

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
	long wavPosition;
	bool stopTimer;
	RtAudio *audio;


signals:
	void currentProgress(int);
	void finished();
public:
	char* getData() { return this->data; }
	int getChannels() { return chans; }

	long getWavPosition() { return this->wavPosition; }

	int getLength() { return this->length; }
	void addSamplesToCounter(long samples) { this->wavPosition += samples; }
    WavPlayer(QObject *parent=0);
    bool play(QString filename);
    ~WavPlayer();
private slots:
	void closeStream();
public slots:
    	void publishTime(double time);
	void stop();
	


};

#endif
