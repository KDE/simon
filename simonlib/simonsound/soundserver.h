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


#include <simonsound/simonsound.h>
#include <QIODevice>
#include <QHash>
#include <QList>
#include <qaudio.h>
#include "simonsound_export.h"

class QAudioInput;
class SoundInputClient;
class QAudioOutput;
class SoundOutputClient;
class SimonSoundInput;
class SimonSoundOutput;


class SIMONSOUND_EXPORT SoundServer : public QIODevice {
	Q_OBJECT

signals:
	void error(const QString& str);
	void inputStateChanged(QAudio::State state);
	void outputStateChanged(QAudio::State state);


private:
	static SoundServer* instance;

	//QAudioInput *input;
	QHash<SimonSound::DeviceConfiguration, SimonSoundInput*> inputs;
	/*
	QHash<DeviceConfiguration, QAudioInput*> inputs;
	QHash<SoundInputClient*, qint64> activeInputClients;
	QHash<SoundInputClient*, qint64> suspendedInputClients;
	*/
	bool startRecording(SimonSound::DeviceConfiguration& device);
	bool stopRecording(SimonSound::DeviceConfiguration device);

	void suspendRecording();
	void resumeRecording();

	QHash<SimonSound::DeviceConfiguration, SimonSoundOutput*> outputs;
	//SoundOutputClient* currentOutputClient;
	/*QAudioOutput *output;
	QList<SoundOutputClient*> suspendedOutputClients;
	*/
	bool startPlayback(SimonSound::DeviceConfiguration& device);
	bool stopPlayback(SimonSound::DeviceConfiguration device);

	void suspendPlayback();
	void resumePlayback();

private slots:
	void slotInputStateChanged(QAudio::State state);
	void slotOutputStateChanged(QAudio::State state);

protected:
	qint64 readData(char *toRead, qint64 maxLen);
	qint64 writeData(const char *toWrite, qint64 len);

public:
	static SoundServer* getInstance()
	{
		if (!instance) instance = new SoundServer(NULL);
		return instance;
	}

	SoundServer(QObject *parent=0);

	bool registerInputClient(SoundInputClient* client);
	bool deRegisterInputClient(SoundInputClient* client);

	bool registerOutputClient(SoundOutputClient* client);
	bool deRegisterOutputClient(SoundOutputClient* client);

	qint64 byteSizeToLength(qint64 bytes, SimonSound::DeviceConfiguration device);
	qint64 lengthToByteSize(qint64 length, SimonSound::DeviceConfiguration device);

	bool reinitializeDevices();

	int getInputDeviceCount();
	int getOutputDeviceCount();

	static bool getDefaultToPowerTraining();

	static int getLevelThreshold();
	static int getHeadMargin();
	static int getTailMargin();
	static int getShortSampleCutoff();

	static QString defaultInputDevice();
	static QString defaultOutputDevice();

    virtual ~SoundServer();

};

#endif

