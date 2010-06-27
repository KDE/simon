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


#ifndef SIMON_WAVPLAYERSUBCLIENT_H_272785B973C443B89098D25E583308C1
#define SIMON_WAVPLAYERSUBCLIENT_H_272785B973C443B89098D25E583308C1

#include <QObject>
#include <simonsound/simonsound.h>
#include "soundoutputclient.h"

class WAV;

class WavPlayerSubClient : public QIODevice, public SoundOutputClient {
	Q_OBJECT
	
private:
	WAV *wav;
	qint64 length;

signals:
	void currentProgress(int);
	void finished();

protected:
	qint64 readData(char *toRead, qint64 maxLen);
	qint64 writeData(const char *toWrite, qint64 len);
	
public:
	WavPlayerSubClient(SimonSound::DeviceConfiguration device, QObject *parent=0);
	~WavPlayerSubClient();

	bool play(QString filename);

	QIODevice* getDataProvider() { return this; }
	bool open (OpenMode mode);
	void close();
	void finish();
	int getChannelCount();
		
public slots:
	void stop();
	


};

#endif


