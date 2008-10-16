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
#include <QTimer>
#include "portaudio.h"

class WAV;

class WavRecorder : public QObject {
	Q_OBJECT
private:
	PaStream* stream;
	WAV *wavData;
	int chans/*, samplerate*/;
	long startTime;
	QTimer timeWatcher;

signals:
	void currentProgress(int msecs);
	
private slots:
	void publishTime();
	
	
public:
	WAV* getWav() { return wavData; }
	int getChannels() { return chans; }

	WavRecorder(QObject *parent=0);
	bool finish();
    	bool record(QString filename);
    

    ~WavRecorder();

};

#endif
