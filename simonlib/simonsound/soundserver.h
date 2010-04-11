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


#include <QIODevice>
#include <QHash>
#include <QList>
#include <qaudio.h>

class QAudioInput;
class SoundInputClient;
class QAudioOutput;
class SoundOutputClient;

class SoundServer : public QIODevice {
	Q_OBJECT

signals:
	void error(const QString& str);

private:
	static SoundServer* instance;

	int channels, sampleRate;

	QAudioInput *input;
	QHash<SoundInputClient*, qint64> activeInputClients;
	QHash<SoundInputClient*, qint64> suspendedInputClients;
	bool startRecording();
	bool stopRecording();

	QAudioOutput *output;
	SoundOutputClient* currentOutputClient;
	QList<SoundOutputClient*> suspendedOutputClients;
	bool startPlayback();
	bool stopPlayback();

private slots:
	void inputStateChanged(QAudio::State state);
	void outputStateChanged(QAudio::State state);

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

	int getChannels() { return channels; }
	int getSampleRate() { return sampleRate; }

	bool registerInputClient(SoundInputClient* client);
	bool deRegisterInputClient(SoundInputClient* client);

	bool registerOutputClient(SoundOutputClient* client);
	bool deRegisterOutputClient(SoundOutputClient* client);

	qint64 byteSizeToLength(qint64 bytes);
    

    virtual ~SoundServer();

};

#endif

