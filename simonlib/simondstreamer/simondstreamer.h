/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_WAVRECORDERCLIENT_H_0AC60651BE6A419EA6256220815A2AAD
#define SIMON_WAVRECORDERCLIENT_H_0AC60651BE6A419EA6256220815A2AAD


#include "simonsender.h"
#include "simondstreamer_export.h"
#include <simonsound/soundinputclient.h>
#include <QObject>

class WAV;
class LoudnessMeterSoundProcessor;

class SIMONDSTREAMER_EXPORT SimondStreamer : public QObject, public SoundInputClient {
	Q_OBJECT

private:
	// when the system detects that the current input is over the
	// silence threshold it will fill up this buffer to a certain
	// extend before we can be sure that it wasen't just a "blib".
	//
	// When this happens we will begin to empty this buffer by sending
	// it to the server, adding live input
	int levelCrossCount;
	QByteArray currentSample;

	SimonSender *sender;

	LoudnessMeterSoundProcessor *loudness;

signals:
	void clippingOccured();
	
public:
	SimondStreamer(SimonSender *sender, QObject *parent=0);
	bool stop();
    	bool start();

	void processPrivate(const QByteArray& data, qint64 currentTime);
    
	virtual ~SimondStreamer();

};

#endif

