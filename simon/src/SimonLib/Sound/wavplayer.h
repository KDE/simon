/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


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
