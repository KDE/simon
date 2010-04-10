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


#ifndef SIMON_SOUNDSERVER_H_BAC60651BE6A419EA6156220815A2AAD
#define SIMON_SOUNDSERVER_H_BAC60651BE6A419EA6156220815A2AAD


#include <QObject>
#include <QHash>
#include "sounddata.h"

class QAudioInput;
class SoundClient;

class SoundServer : public QObject {
	Q_OBJECT

private:
	static SoundServer* instance;

	int channels, sampleRate;

	QAudioInput *input;
//	QBuffer inputData;
	SoundData inputData;
	QHash<SoundClient*, qint64> activeInputClients;
	QHash<SoundClient*, qint64> suspendedInputClients;
	bool startRecording();
	bool stopRecording();

private slots:
	void inputDataAvailable(qint64);

public:
	static SoundServer* getInstance()
	{
		if (!instance) instance = new SoundServer(NULL);
		return instance;
	}

	SoundServer(QObject *parent=0);

	int getChannels() { return channels; }
	int getSampleRate() { return sampleRate; }

	bool registerInputClient(SoundClient* client);
	bool deRegisterInputClient(SoundClient* client);
    

    virtual ~SoundServer();

};

#endif

